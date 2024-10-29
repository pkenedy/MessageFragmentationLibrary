# Message Fragmenter

This application is designed to fragment and reassemble messages into manageable chunks. It incorporates a callback mechanism to handle missing chunks and utilizes a timeout feature to ensure data integrity. Built in C++, this library is ideal for applications requiring reliable message processing and reconstruction.

## Features

- **Message Fragmentation**: Automatically divides a given message into smaller chunks based on a specified size.
- **Reassembly**: Reconstructs the original message from the received chunks while checking for missing entries.
- **Missing Chunk Detection**: Invokes a callback for any missing chunks, allowing for effective error handling.
- **Timeout Management**: Implements a timeout mechanism to trigger missing chunk detection after a specified duration.

## Implementation Approach

### 1. **Constructor Initialization**

The `MessageFragmenter` class is initialized with the following parameters:
- **chunkSize**: The size of each chunk for message fragmentation.
- **timeoutDuration**: The duration after which missing chunks are detected.
- **MissingChunkCallback**: A callback function that is triggered for each missing chunk.

### 2. **Message Fragmentation**

The application features a `fragmentMessage` method that accepts a string message and divides it into smaller substrings based on the `chunkSize`. The method returns a vector of strings representing these chunks, making it easy to manage large messages.

### 3. **Reassembling Messages**

The `reassembleMessage` method takes a vector of pairs, where each pair consists of an index and the corresponding chunk of the message. It concatenates the received chunks in order while invoking the `checkForMissingChunks` method to identify any missing entries.

### 4. **Missing Chunk Detection**

The `checkForMissingChunks` method scans through the received chunks to determine if any are missing. If any chunks are unaccounted for, the callback function is called. This mechanism ensures that applications using the `MessageFragmenter` can effectively handle data integrity issues.

### 5. **Timeout Management**

A timer is initiated when the `MessageFragmenter` object is created. If the time elapsed exceeds `timeoutDuration`, the application checks for missing chunks and invokes the callback for each detected missing chunk, preventing indefinite waiting for data.

## Requirements
- **C++17 or higher**
- **MSYS2 / MinGW64** or a similar environment for building and running the project.
- **GDB (GNU Debugger)** for debugging and inspecting the program flow.

## Compilation

To compile the program, use the following command in your terminal:

```bash
g++ -g -o main main.cpp MessageFragmenter.cpp

gdb ./main

## Set breakpoints
break MessageFragmenter::MessageFragmenter
break MessageFragmenter::fragmentMessage
break MessageFragmenter::reassembleMessage
break MessageFragmenter::checkForMissingChunks
break MessageFragmenter::test

## Run the program
 run



  TO Callback for missing chunks & Report corrupted data after a timeout threshold   
 
 
  ##Compile the Code:

        bash

g++ -g -o main main.cpp MessageFragmenter.cpp

  ##Run the Executable:

bash

    ./main
