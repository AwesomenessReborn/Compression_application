#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <chrono>
#include "Compressor.h"
#include "HuffmanCompressor.h"
#include "LZWCompressor.h"
#include "DefaultCompressor.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <encoding_order>" << std::endl;
        return 1;
    }

    // Parse arguments
    std::string inputFile = std::string("inputs/") + argv[1];
    std::string encodingOrder = argv[2];

    // Read the input file
    std::ifstream input(inputFile, std::ios::binary);
    if (!input) {
        std::cerr << "Error: Cannot open file " << inputFile << std::endl;
        return 1;
    }
    std::string data((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    input.close();

    // Process encoding order
    std::string compressedData = data;
    std::string decompressedData;
    for (char c : encodingOrder) {
        auto start = std::chrono::high_resolution_clock::now();

        std::unique_ptr<Compressor> compressor;
        switch (c) {
            case '1':
                compressor = std::make_unique<HuffmanCompressor>();
                break;
            case '2':
                compressor = std::make_unique<LZWCompressor>();
                break;
            // Add more cases for additional algorithms
            default:
                std::cerr << "<encoding_order> must only contain 1, 2, or 3. " << inputFile << std::endl;
                break;
        }

        compressedData = compressor->encode(compressedData);

        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Compression with algorithm " << c 
                  << " took " << std::chrono::duration<double, std::milli>(end - start).count() << " ms" << std::endl;
    }

    // Decompress
    decompressedData = compressedData;
    for (auto it = encodingOrder.rbegin(); it != encodingOrder.rend(); ++it) {
        auto start = std::chrono::high_resolution_clock::now();

        std::unique_ptr<Compressor> compressor;
        switch (*it) {
            case '1':
                compressor = std::make_unique<HuffmanCompressor>();
                break;
            case '2':
                compressor = std::make_unique<LZWCompressor>();
                break;
            default:
                compressor = std::make_unique<DefaultCompressor>();
                break;
        }

        decompressedData = compressor->decode(decompressedData);

        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Decompression with algorithm " << *it 
                  << " took " << std::chrono::duration<double, std::milli>(end - start).count() << " ms" << std::endl;
    }

    // Calculate and output the compression ratio
    double compressionRatio = static_cast<double>(compressedData.size()) / data.size();
    std::cout << "Compression Ratio: " << compressionRatio << std::endl;

    // Write output to the "outputs" folder
    std::ofstream output("outputs/output.txt", std::ios::binary);
    if (!output) {
        std::cerr << "Error: Cannot open output file." << std::endl;
        return 1;
    }
    output << compressedData;
    output.close();

    return 0;
}
