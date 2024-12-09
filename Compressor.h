#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <string>

class Compressor {
public:
    virtual ~Compressor() = default;

    // Pure virtual methods for encoding and decoding
    virtual std::string encode(const std::string& input) = 0;
    virtual std::string decode(const std::string& input) = 0;
};

#endif // COMPRESSOR_H
