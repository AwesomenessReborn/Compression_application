#ifndef LZW_COMPRESSOR_H
#define LZW_COMPRESSOR_H

#include "Compressor.h"
#include <string>

class LZWCompressor : public Compressor {
public:
    std::string encode(const std::string& input) override {
        // Implement LZW encoding
        return "LZW Encoded: " + input;
    }

    std::string decode(const std::string& input) override {
        // Implement LZW decoding
        return "LZW Decoded: " + input;
    }
};

#endif // LZW_COMPRESSOR_H
