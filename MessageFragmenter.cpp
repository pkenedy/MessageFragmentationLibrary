#include "MessageFragmenter.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <algorithm>

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
    for (const auto& chunk : receivedChunks) {
        reassembledMessage << chunk.second;
    }

    return reassembledMessage.str();
}

// Check for missing chunks and invoke callback
void MessageFragmenter::checkForMissingChunks() {
    missingChunks.clear();
    for (int i = 0; i < receivedChunks.size(); ++i) {
        if (receivedChunks.find(i) == receivedChunks.end()) {
            missingChunks.push_back(i);
        }
    }

    // Timeout logic to handle missing/corrupted data
    auto now = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count() > timeoutDuration) {
        for (int missingChunk : missingChunks) {
            missingChunkCallback(missingChunk);
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

    // Expected to report missing chunk and return reassembled message
    fragmenter.checkForMissingChunks();

    std::cout << "Reassembled Message: " << reassembledMessage << std::endl;

    return missingChunkReported;
}
