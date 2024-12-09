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


