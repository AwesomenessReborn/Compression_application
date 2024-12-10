#ifndef HUFFMAN_COMPRESSOR_H
#define HUFFMAN_COMPRESSOR_H

#include "Compressor.h"
#include <unordered_map>
#include <string>
#include <queue>
#include <vector>
#include <stdexcept>
#include <ostream>
#include <iostream>

class HuffmanTreeNode
{
private:
    HuffmanTreeNode *leftChild;
    HuffmanTreeNode *rightChild;
    char character;
    int frequency;

public:
    HuffmanTreeNode(HuffmanTreeNode *leftChildNode, HuffmanTreeNode *rightChildNode)
        : leftChild(leftChildNode), rightChild(rightChildNode), character('\0'),
          frequency(leftChild->GetFrequency() + rightChild->GetFrequency()) {}

    HuffmanTreeNode(char leafCharacter, int leafFrequency)
        : leftChild(nullptr), rightChild(nullptr), character(leafCharacter),
          frequency(leafFrequency) {}

    ~HuffmanTreeNode()
    {
        delete leftChild;
        delete rightChild;
    }

    char GetCharacter() const { return character; }

    HuffmanTreeNode *GetLeftChild() const { return leftChild; }

    HuffmanTreeNode *GetRightChild() const { return rightChild; }

    int GetFrequency() const { return frequency; }
};

class HuffmanNodeFrequencyComparer
{
public:
    bool operator()(HuffmanTreeNode *lhs, HuffmanTreeNode *rhs)
    {
        return lhs->GetFrequency() > rhs->GetFrequency();
    }
};

class HuffmanCompressor : public Compressor
{
private:
    std::unordered_map<char, std::string> codes;
    HuffmanTreeNode *root;

    std::unordered_map<char, int> BuildFrequencyTable(const std::string &input)
    {
        std::unordered_map<char, int> table;
        for (char c : input)
        {
            table[c]++;
        }
        return table;
    }

    HuffmanTreeNode *BuildTree(const std::unordered_map<char, int> &frequencyTable)
    {
        std::priority_queue<HuffmanTreeNode *, std::vector<HuffmanTreeNode *>, HuffmanNodeFrequencyComparer> nodes;

        for (const auto &entry : frequencyTable)
        {
            nodes.push(new HuffmanTreeNode(entry.first, entry.second));
        }

        while (nodes.size() > 1)
        {
            auto *left = nodes.top();
            nodes.pop();
            auto *right = nodes.top();
            nodes.pop();
            nodes.push(new HuffmanTreeNode(left, right));
        }

        return nodes.top();
    }

    void GenerateCodes(HuffmanTreeNode *node, const std::string &prefix)
    {
        if (!node->GetLeftChild() && !node->GetRightChild())
        {
            codes[node->GetCharacter()] = prefix;
            return;
        }

        if (node->GetLeftChild())
            GenerateCodes(node->GetLeftChild(), prefix + "0");
        if (node->GetRightChild())
            GenerateCodes(node->GetRightChild(), prefix + "1");
    }

public:
    HuffmanCompressor() : root(nullptr) {}

    ~HuffmanCompressor()
    {
        delete root;
    }

    std::string encode(const std::string &input) override
    {
        auto frequencyTable = BuildFrequencyTable(input);
        root = BuildTree(frequencyTable);

        codes.clear();
        GenerateCodes(root, "");

        std::string binaryString;
        for (char c : input)
        {
            binaryString += codes[c];
        }

        std::string encodedString;
        uint8_t currentByte = 0;
        int bitCount = 0;

        for (char bit : binaryString)
        {
            currentByte = (currentByte << 1) | (bit - '0');
            bitCount++;

            if (bitCount == 8)
            {
                encodedString += static_cast<char>(currentByte);
                currentByte = 0;
                bitCount = 0;
            }
        }

        if (bitCount > 0)
        {
            currentByte <<= (8 - bitCount);
            encodedString += static_cast<char>(currentByte);
        }

        return encodedString;
    }

    std::string decode(const std::string &encodedString) override {
        if (!root)
            throw std::runtime_error("Decoding requires a valid Huffman tree.");

        std::string binaryString;

        for (char c : encodedString) {
            uint8_t byte = static_cast<uint8_t>(c);
            for (int i = 7; i >= 0; --i) 
            {
                binaryString += ((byte >> i) & 1) ? '1' : '0';
            }
        }

        std::cout << "Binary Decoded String: " << binaryString << std::endl;

        std::string decodedString;
        HuffmanTreeNode *current = root;

        for (char bit : binaryString) {
            current = (bit == '0') ? current->GetLeftChild() : current->GetRightChild();

            if (!current->GetLeftChild() && !current->GetRightChild())
            {
                decodedString += current->GetCharacter();
                current = root;
            }
        }

        return decodedString;
    }
};

#endif // HUFFMAN_COMPRESSOR_H
