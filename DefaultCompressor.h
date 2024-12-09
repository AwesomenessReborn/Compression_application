#ifndef DEFAULT_COMPRESSOR_H
#define DEFAULT_COMPRESSOR_H

#include "Compressor.h"
#include <string>

class DefaultCompressor : public Compressor {
public:
    std::string encode(const std::string& input) override {
        // Default: No compression, return input as is
        return input;
    }

    std::string decode(const std::string& input) override {
        // Default: No decompression, return input as is
        return input;
    }
};

#endif // DEFAULT_COMPRESSOR_H
