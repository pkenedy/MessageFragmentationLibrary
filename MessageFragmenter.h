#ifndef MESSAGE_FRAGMENTER_H
#define MESSAGE_FRAGMENTER_H

#include <vector>
#include <string>
#include <map>
#include <functional>
#include <chrono>

class MessageFragmenter {
public:
    using MissingChunkCallback = std::function<void(int)>;

    MessageFragmenter(size_t chunkSize, int timeoutDuration, MissingChunkCallback callback);

    std::vector<std::string> fragmentMessage(const std::string& message);
    std::string reassembleMessage(const std::vector<std::pair<int, std::string>>& chunks);
    void checkForMissingChunks();
    static bool test();

private:
    size_t chunkSize;
    int timeoutDuration;
    MissingChunkCallback missingChunkCallback;

    std::map<int, std::string> receivedChunks;
    std::vector<bool> chunkReported; // To track which chunks have been reported
    std::chrono::time_point<std::chrono::steady_clock> startTime;
};

#endif
