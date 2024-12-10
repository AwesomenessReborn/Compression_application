#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <string>
#include <vector>

class Compressor {
public:
    virtual ~Compressor() = default;

    // Encodes a string into a vector of bytes (binary format).
    virtual std::vector<uint8_t> encode(const std::string& input) = 0;

    // Decodes a vector of bytes into the original string.
    virtual std::string decode(const std::vector<uint8_t>& input) = 0;
};

#endif // COMPRESSOR_H
