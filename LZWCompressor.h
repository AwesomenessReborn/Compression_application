#ifndef LZW_COMPRESSOR_H
#define LZW_COMPRESSOR_H

#include "Compressor.h"
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <string>

class LZWCompressor : public Compressor {
public:
    std::vector<uint8_t> encode(const std::string& input) override {
        if (input.empty())
            return {};

        std::unordered_map<std::string, int> dictionary;
        for (int i = 0; i < 256; ++i)
            dictionary[std::string(1, char(i))] = i;

        std::string current;
        std::vector<uint16_t> codes;
        int dictSize = 256;

        for (char ch : input) {
            current += ch;
            if (dictionary.find(current) == dictionary.end()) {
                codes.push_back(dictionary[current.substr(0, current.size() - 1)]);
                dictionary[current] = dictSize++;
                current = ch;
            }
        }

        if (!current.empty())
            codes.push_back(dictionary[current]);

        // Convert 16-bit codes into a byte stream for consistency
        std::vector<uint8_t> encodedBytes;
        for (uint16_t code : codes) {
            encodedBytes.push_back((code >> 8) & 0xFF); // High byte
            encodedBytes.push_back(code & 0xFF);        // Low byte
        }

        return encodedBytes;
    }

    std::string decode(const std::vector<uint8_t>& compressed) override {
        if (compressed.empty())
            return "";

        // Convert byte stream back to 16-bit codes
        std::vector<uint16_t> codes;
        for (size_t i = 0; i < compressed.size(); i += 2) {
            uint16_t code = (compressed[i] << 8) | compressed[i + 1];
            codes.push_back(code);
        }

        std::unordered_map<int, std::string> dictionary;
        for (int i = 0; i < 256; ++i)
            dictionary[i] = std::string(1, char(i));

        std::string current = dictionary[codes[0]];
        std::string result = current;
        int dictSize = 256;

        for (size_t i = 1; i < codes.size(); ++i) {
            std::string entry;
            if (dictionary.find(codes[i]) != dictionary.end()) {
                entry = dictionary[codes[i]];
            } else if (codes[i] == dictSize) {
                entry = current + current[0];
            } else {
                throw std::runtime_error("Invalid LZW code encountered during decoding.");
            }

            result += entry;
            dictionary[dictSize++] = current + entry[0];
            current = entry;
        }

        return result;
    }
};

#endif // LZW_COMPRESSOR_H
