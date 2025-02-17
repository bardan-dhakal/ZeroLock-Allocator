#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <cppkafka/cppkafka.h>
#include <sw/redis++/redis++.h>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/prepared_statement.h>

class StockData {
public:
    std::string symbol;
    double price;
    int volume;
    std::string timestamp;
    
    StockData(std::string sym, double p, int vol, std::string time)
        : symbol(sym), price(p), volume(vol), timestamp(time) {}
};

class KafkaConsumer {
private:
    cppkafka::Consumer consumer;
    std::queue<StockData> dataQueue;
    std::mutex queueMutex;
    std::condition_variable queueCV;
    bool running;

public:
    KafkaConsumer(const std::string& brokers, const std::string& topic) 
        : consumer({
            {"metadata.broker.list", brokers},
            {"group.id", "stock_analysis_group"},
            {"auto.offset.reset", "latest"}
        }), running(true) {
        consumer.subscribe({topic});
    }

    void start() {
        std::thread consumerThread(&KafkaConsumer::consume, this);
        consumerThread.detach();
    }

    void consume() {
        while (running) {
            auto msg = consumer.poll();
            if (msg && !msg.is_eof()) {
                // Parse message and create StockData object
                // This is a simplified version - you'd need proper JSON parsing
                StockData data("AAPL", 150.0, 1000, "2024-02-17 10:00:00");
                
                std::lock_guard<std::mutex> lock(queueMutex);
                dataQueue.push(data);
                queueCV.notify_one();
            }
        }
    }

    StockData getNextData() {
        std::unique_lock<std::mutex> lock(queueMutex);
        queueCV.wait(lock, [this] { return !dataQueue.empty(); });
        
        StockData data = dataQueue.front();
        dataQueue.pop();
        return data;
    }
};

class RedisCache {
private:
    sw::redis::Redis redis;

public:
    RedisCache(const std::string& host, int port) 
        : redis(sw::redis::ConnectionOptions(host, port)) {}

    void cacheStockData(const StockData& data) {
        std::string key = data.symbol + ":" + data.timestamp;
        redis.hset(key,
                  std::unordered_map<std::string, std::string>{
                      {"price", std::to_string(data.price)},
                      {"volume", std::to_string(data.volume)}
                  });
        redis.expire(key, std::chrono::seconds(3600)); // 1 hour expiration
    }
};

class MySQLStorage {
private:
    std::unique_ptr<sql::Connection> conn;
    std::unique_ptr<sql::PreparedStatement> insertStmt;

public:
    MySQLStorage(const std::string& host, const std::string& user, const std::string& password) {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        conn.reset(driver->connect(host, user, password));
        conn->setSchema("stock_analysis");
        
        insertStmt.reset(conn->prepareStatement(
            "INSERT INTO stock_data (symbol, price, volume, timestamp) VALUES (?, ?, ?, ?)"
        ));
    }

    void storeStockData(const StockData& data) {
        insertStmt->setString(1, data.symbol);
        insertStmt->setDouble(2, data.price);
        insertStmt->setInt(3, data.volume);
        insertStmt->setString(4, data.timestamp);
        insertStmt->execute();
    }
};

class StockAnalyzer {
private:
    static constexpr int ANALYSIS_WINDOW = 100; // Number of data points for analysis
    std::vector<double> priceHistory;
    std::vector<int> volumeHistory;

public:
    struct AnalysisResult {
        double priceMovingAverage;
        double volumeMovingAverage;
        bool bullishTrend;
    };

    AnalysisResult analyze(const StockData& newData) {
        priceHistory.push_back(newData.price);
        volumeHistory.push_back(newData.volume);

        if (priceHistory.size() > ANALYSIS_WINDOW) {
            priceHistory.erase(priceHistory.begin());
            volumeHistory.erase(volumeHistory.begin());
        }

        AnalysisResult result;
        result.priceMovingAverage = calculateMovingAverage(priceHistory);
        result.volumeMovingAverage = calculateMovingAverage(volumeHistory);
        result.bullishTrend = detectTrend();

        return result;
    }

private:
    template<typename T>
    double calculateMovingAverage(const std::vector<T>& data) {
        if (data.empty()) return 0.0;
        return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
    }

    bool detectTrend() {
        if (priceHistory.size() < 2) return false;
        
        // Simple trend detection based on last few prices
        size_t n = priceHistory.size();
        return priceHistory[n-1] > priceHistory[n-2];
    }
};

class RealTimeStockAnalysisSystem {
private:
    KafkaConsumer kafkaConsumer;
    RedisCache redisCache;
    MySQLStorage mysqlStorage;
    StockAnalyzer analyzer;
    bool running;
    std::vector<std::thread> workerThreads;

public:
    RealTimeStockAnalysisSystem(
        const std::string& kafkaBrokers,
        const std::string& kafkaTopic,
        const std::string& redisHost,
        int redisPort,
        const std::string& mysqlHost,
        const std::string& mysqlUser,
        const std::string& mysqlPassword
    ) : kafkaConsumer(kafkaBrokers, kafkaTopic),
        redisCache(redisHost, redisPort),
        mysqlStorage(mysqlHost, mysqlUser, mysqlPassword),
        running(true) {}

    void start(int numThreads = 4) {
        kafkaConsumer.start();

        for (int i = 0; i < numThreads; ++i) {
            workerThreads.emplace_back(&RealTimeStockAnalysisSystem::processData, this);
        }
    }

    void stop() {
        running = false;
        for (auto& thread : workerThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

private:
    void processData() {
        while (running) {
            try {
                StockData data = kafkaConsumer.getNextData();
                
                // Cache in Redis for quick access
                redisCache.cacheStockData(data);
                
                // Persist to MySQL
                mysqlStorage.storeStockData(data);
                
                // Analyze the data
                auto analysis = analyzer.analyze(data);
                
                // Log or act on analysis results
                if (analysis.bullishTrend) {
                    std::cout << "Bullish trend detected for " << data.symbol 
                              << " MA: " << analysis.priceMovingAverage << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Error processing data: " << e.what() << std::endl;
            }
        }
    }
};

int main() {
    try {
        RealTimeStockAnalysisSystem system(
            "localhost:9092",      // Kafka broker
            "stock_data",          // Kafka topic
            "localhost",           // Redis host
            6379,                  // Redis port
            "localhost:3306",      // MySQL host
            "user",                // MySQL user
            "password"             // MySQL password
        );

        system.start(4);  // Start with 4 worker threads

        // Keep the main thread running
        std::cout << "System running. Press Enter to exit." << std::endl;
        std::cin.get();

        system.stop();
    } catch (const std::exception& e) {
        std::cerr << "System error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}