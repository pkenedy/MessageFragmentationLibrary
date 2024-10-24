#ifndef MESSAGE_FRAGMENTER_H
#define MESSAGE_FRAGMENTER_H

#include <vector>
#include <string>
#include <map>
#include <functional>
#include <chrono>

class MessageFragmenter {
public:
    // Callback for missing chunks
    using MissingChunkCallback = std::function<void(int)>;

    // Constructor
    MessageFragmenter(size_t chunkSize, int timeoutDuration, MissingChunkCallback callback);

    // Fragment the message into smaller chunks
    std::vector<std::string> fragmentMessage(const std::string& message);

    // Reassemble fragmented chunks into the whole message
    std::string reassembleMessage(const std::vector<std::pair<int, std::string>>& chunks);

    // Check for missing chunks
    void checkForMissingChunks();

    // Validate message reassembly and callback reporting test
    static bool test();

private:
    size_t chunkSize; // Size of each chunk
    int timeoutDuration; // Timeout for corrupted data (in milliseconds)
    MissingChunkCallback missingChunkCallback;

    // Internal data structures
    std::map<int, std::string> receivedChunks;
    std::vector<int> missingChunks;
    std::chrono::time_point<std::chrono::steady_clock> startTime;
};

#endif
