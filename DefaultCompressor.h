#ifndef DEFAULTCOMPRESSOR_H
#define DEFAULTCOMPRESSOR_H

#include "Compressor.h"
#include <vector>
#include <string>

class DefaultCompressor : public Compressor {
public:
    // Encodes a string into a vector of bytes without any compression
    std::vector<uint8_t> encode(const std::string& input) override {
        // Simply convert the string into a vector of bytes
        return std::vector<uint8_t>(input.begin(), input.end());
    }

    // Decodes a vector of bytes back into the original string without any decompression
    std::string decode(const std::vector<uint8_t>& input) override {
        // Simply convert the vector of bytes back into a string
        return std::string(input.begin(), input.end());
    }
};

#endif // DEFAULTCOMPRESSOR_H