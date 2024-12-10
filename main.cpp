#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <memory>
#include <chrono>
#include "Compressor.h"
#include "HuffmanCompressor.h"
#include "LZWCompressor.h"
#include "DefaultCompressor.h"

int main(int argc, char* argv[]) {
    // ---- BEGIN ARGUMENT PROCESSING ----
    std::cout << std::endl; 
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <encoding_order>" << std::endl;
        return 1;
    }

    std::string inputFile = std::string("inputs/") + argv[1];
    std::string encodingOrder = argv[2];

    std::ifstream input(inputFile, std::ios::binary);
    if (!input) {
        std::cerr << "Error: Cannot open file " << inputFile << std::endl;
        return 1;
    }
    std::string data((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    input.close();
    // ---- END ARGUMENT PROCESSING ----

    // Double-ended queue to hold compressors based on encoding order
    std::deque<std::unique_ptr<Compressor>> compressorDeque;

    for (char c : encodingOrder) {
        switch (c) {
            case '1':
                compressorDeque.push_back(std::make_unique<HuffmanCompressor>());
                break;
            case '2':
                compressorDeque.push_back(std::make_unique<LZWCompressor>());
                break;

            case '3': 
                compressorDeque.push_back(std::make_unique<DefaultCompressor>()); 
                break; 
            // Add more cases for additional algorithms
            default:
                std::cerr << "Error: Invalid encoding order character '" << c << "'." << std::endl;
                return 1;
        }
    }

    // Perform compression
    std::vector<uint8_t> compressedData(data.begin(), data.end());
    double totalCompressionTime = 0.0;

    for (auto& compressor : compressorDeque) {
        auto start = std::chrono::high_resolution_clock::now();
        compressedData = compressor->encode(std::string(compressedData.begin(), compressedData.end()));
        auto end = std::chrono::high_resolution_clock::now();

        double compressionTime = std::chrono::duration<double, std::milli>(end - start).count();

        std::cout << "\tCompression with " << typeid(*compressor).name() 
                  << " took " << compressionTime << " ms" << std::endl;

        totalCompressionTime += compressionTime;
    }

    std::cout << "Total Compression Time: " << totalCompressionTime << " ms\n" << std::endl;

    // Perform decompression in reverse order using the same deque
    std::vector<uint8_t> decompressedData = compressedData;
    double totalDecompressionTime = 0.0;

    while (!compressorDeque.empty()) {
        auto& compressor = compressorDeque.back(); // Access the last compressor
        auto start = std::chrono::high_resolution_clock::now();

        // Convert intermediate decompressed data (vector<uint8_t>) back to string for decode()
        std::string intermediateData(decompressedData.begin(), decompressedData.end());
        std::string decodedString = compressor->decode(decompressedData);

        // Convert the decoded string back to vector<uint8_t> for further processing
        decompressedData = std::vector<uint8_t>(decodedString.begin(), decodedString.end());

        auto end = std::chrono::high_resolution_clock::now();
        double decompressionTime = std::chrono::duration<double, std::milli>(end - start).count();
        totalDecompressionTime += decompressionTime;

        std::cout << "\tDecompression with " << typeid(*compressor).name() 
                << " took " << decompressionTime << " ms" << std::endl;

        compressorDeque.pop_back(); // Remove the last compressor
    }

    std::cout << "Total Decompression Time: " << totalDecompressionTime << " ms\n" << std::endl;

    // Calculate and output the compression ratio
    double compressionRatio = static_cast<double>(compressedData.size()) / data.size();
    std::cout << "\n-----\nCompression Ratio: " << compressionRatio << "\n-----" << std::endl;

    // Write output to the "outputs" folder
    std::ofstream output_compressed("outputs/output_compressed.bin", std::ios::binary);
    std::ofstream output_back_to_original("outputs/output_back_to_original.txt", std::ios_base::binary); 
    if (!output_compressed) {
        std::cerr << "Error: Cannot open compressed output file." << std::endl;
        return 1;
    }
    if (!output_back_to_original) {
        std::cerr << "Error: Cannot open back_to_original output file." << std::endl;
        return 1;
    }
    
    output_compressed.write(reinterpret_cast<const char*>(compressedData.data()), compressedData.size());
    output_compressed.close();

    output_back_to_original.write(reinterpret_cast<const char*>(decompressedData.data()), decompressedData.size()); 
    output_back_to_original.close(); 

    return 0;
}