cmake_minimum_required(VERSION 3.12)
project(RealTimeStockAnalysis VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(RdKafka REQUIRED)
find_package(cppkafka REQUIRED)
find_package(MySQL REQUIRED)
find_package(hiredis REQUIRED)
find_package(redis++ REQUIRED)
find_package(Threads REQUIRED)

# Add the executable
add_executable(stock_analyzer 
    src/main.cpp
    src/kafka_consumer.cpp
    src/redis_cache.cpp
    src/mysql_storage.cpp
    src/stock_analyzer.cpp
)

# Include directories
target_include_directories(stock_analyzer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${RDKAFKA_INCLUDE_DIR}
    ${MYSQL_INCLUDE_DIR}
    ${HIREDIS_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(stock_analyzer PRIVATE
    cppkafka
    ${MYSQL_LIBRARIES}
    ${HIREDIS_LIBRARIES}
    redis++
    Threads::Threads
)

# Installation
install(TARGETS stock_analyzer
    RUNTIME DESTINATION bin
)