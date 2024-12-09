#ifndef HUFFMAN_COMPRESSOR_H
#define HUFFMAN_COMPRESSOR_H

#include "Compressor.h"
#include <string>

class HuffmanCompressor : public Compressor {
public:
    std::string encode(const std::string& input) override {
        // Implement Huffman encoding
        return "Huffman Encoded: " + input;
    }

    std::string decode(const std::string& input) override {
        // Implement Huffman decoding
        return "Huffman Decoded: " + input;
    }
};

#endif // HUFFMAN_COMPRESSOR_H
