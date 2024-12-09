# Compression Project 

This project implements a modular compression system in C++ that supports multiple compression algorithms. The system allows users to specify the encoding order via command-line arguments and processes input files accordingly. The outputs include compression statistics like time taken, decompression time, and compression ratio.

## Project Structure

```
compression_project/
├── main.cpp                # Main entry point for the program
├── Compressor.h            # Base class for compression algorithms
├── DefaultCompressor.h     # Default no-op compression implementation
├── HuffmanCompressor.h     # Huffman compression implementation
├── LZWCompressor.h         # LZW compression implementation
├── CMakeLists.txt          # CMake build configuration (or Makefile)
├── inputs/                 # Directory for input files
├── outputs/                # Directory for output files
└── README.md               # Project documentation
```

## Usage 

### Building the project

Ensure you have a C++ compiler and CMake (or Make) installed on your system. Then, follow these steps to build the project:

1. Clone the repo 
    ```
    git clone https://github.com/AwesomenessReborn/Compression_application
    cd compression_project
    ```
2. Build the project 
    ```
    g++ -std=c++17 -O2 -fsanitize=address,undefined -Wall -Wextra -o compression_project main.cpp *.h
    ```

3. Run the project. Ex: 
    ```
    ./compression_project input1.txt 12
    ```
    
### Running the program:

The program requires 3 arguements supplied through `(int argc, char* argv[])`:
1. `argv[0]`: the program name
2. `argv[1]`: the name of input file located in the `/inputs` folder.
3. `argv[2]`: a string of numbers `1` to `9` representing the encoding order. 
    * `0` :  default compression (nothing)
    * `1` :  Huffman compression
    * `2` :  LZW compression
    * `3` :  Run Length Encoding

## Implemented algorithms:

1. DefaultCompressor
        A placeholder algorithm that performs no compression, used as a fallback.

2. HuffmanCompressor
        Implements Huffman coding, a lossless data compression algorithm.

3. LZWCompressor
        Implements Lempel-Ziv-Welch compression, suitable for text and binary data.
