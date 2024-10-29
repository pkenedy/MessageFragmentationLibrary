#include "MessageFragmenter.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

// Constructor
MessageFragmenter::MessageFragmenter(size_t chunkSize, int timeoutDuration, MissingChunkCallback callback)
    : chunkSize(chunkSize), timeoutDuration(timeoutDuration), missingChunkCallback(callback) {
    startTime = std::chrono::steady_clock::now();
}

// Fragment message into chunks
std::vector<std::string> MessageFragmenter::fragmentMessage(const std::string& message) {
    std::vector<std::string> chunks;
    for (size_t i = 0; i < message.size(); i += chunkSize) {
        chunks.push_back(message.substr(i, chunkSize));
    }
    return chunks;
}

// Reassemble message from chunks
std::string MessageFragmenter::reassembleMessage(const std::vector<std::pair<int, std::string>>& chunks) {
    for (const auto& chunk : chunks) {
        receivedChunks[chunk.first] = chunk.second;
    }

    // Check for missing chunks
    checkForMissingChunks();

    std::ostringstream reassembledMessage;
    for (int i = 0; i < (int)receivedChunks.size(); ++i) {
        if (receivedChunks.find(i) != receivedChunks.end()) {
            reassembledMessage << receivedChunks[i];
        }
    }

    return reassembledMessage.str();
}

// Check for missing chunks and invoke callback
void MessageFragmenter::checkForMissingChunks() {
    auto now = std::chrono::steady_clock::now();
    bool timeoutExceeded = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count() > timeoutDuration;

    for (int i = 0; i < (int)receivedChunks.size(); ++i) {
        if (receivedChunks.find(i) == receivedChunks.end()) {
            // Report missing chunk only once
            missingChunkCallback(i);
        }
    }

    // Report missing chunks as corrupted if the timeout has been exceeded
    if (timeoutExceeded) {
        for (int i = 0; i < (int)receivedChunks.size(); ++i) {
            if (receivedChunks.find(i) == receivedChunks.end()) {
                std::cout << "Reporting corrupted data: Missing chunk " << i << " after timeout." << std::endl;
            }
        }
    }
}

// Test function to validate the library
bool MessageFragmenter::test() {
    std::string message = "This is a test message to be fragmented and reassembled.";
    size_t chunkSize = 10;
    int timeout = 5000; // 5 seconds timeout

    bool missingChunkReported = false;

    MessageFragmenter fragmenter(chunkSize, timeout, [&](int missingChunk) {
        std::cout << "Missing chunk: " << missingChunk << std::endl;
        missingChunkReported = true;
    });

    // Fragment the message
    std::vector<std::string> fragments = fragmenter.fragmentMessage(message);

    // Simulate disordered chunk reception
    std::vector<std::pair<int, std::string>> receivedChunks = {
        {0, fragments[0]},
        {2, fragments[2]},  // Simulating that chunk 1 is missing
        {3, fragments[3]}
    };

    // Reassemble the message
    std::string reassembledMessage = fragmenter.reassembleMessage(receivedChunks);

    // Simulate delay to trigger timeout
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout + 100)); // Sleep just over the timeout

    // Check for missing chunks and apply timeout logic
    fragmenter.checkForMissingChunks();

    std::cout << "Reassembled Message: " << reassembledMessage << std::endl;

    return missingChunkReported;
}
