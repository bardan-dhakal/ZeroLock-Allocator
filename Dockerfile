FROM ubuntu:22.04

# Install system dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    librdkafka-dev \
    libcppkafka-dev \
    libmysqlclient-dev \
    libhiredis-dev \
    libredis++-dev \
    git \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy source code
COPY . .

# Build the project
RUN mkdir build && cd build && \
    cmake .. && \
    make -j$(nproc)

# Set environment variables
ENV KAFKA_BROKER=kafka:9092
ENV REDIS_HOST=redis
ENV MYSQL_HOST=mysql

# Run the application
CMD ["./build/stock_analyzer"]