#include "MessageFragmenter.h"
#include <iostream>

int main() {
    if (MessageFragmenter::test()) {
        std::cout << "Test Passed: All features validated successfully." << std::endl;
    } else {
        std::cout << "Test Failed: Missing chunks were not reported correctly." << std::endl;
    }

    return 0;
}
