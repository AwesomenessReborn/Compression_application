#ifndef HUFFMAN_COMPRESSOR_H
#define HUFFMAN_COMPRESSOR_H

#include "Compressor.h"
#include <unordered_map>
#include <string>
#include <queue>
#include <vector>
#include <stdexcept>

class HuffmanTreeNode
{
private:
    HuffmanTreeNode *leftChild;
    HuffmanTreeNode *rightChild;
    char character;
    int frequency;

public:
    // Constructor for internal nodes
    HuffmanTreeNode(HuffmanTreeNode *leftChildNode, HuffmanTreeNode *rightChildNode)
        : leftChild(leftChildNode), rightChild(rightChildNode), character('\0'),
          frequency(leftChild->GetFrequency() + rightChild->GetFrequency()) {}

    // Constructor for leaf nodes
    HuffmanTreeNode(char leafCharacter, int leafFrequency)
        : leftChild(nullptr), rightChild(nullptr), character(leafCharacter),
          frequency(leafFrequency) {}

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

    // Helper function to build a frequency table
    std::unordered_map<char, int> BuildFrequencyTable(const std::string &input)
    {
        std::unordered_map<char, int> table;
        for (char c : input)
        {
            table[c]++;
        }
        return table;
    }

    // Helper function to build the Huffman tree
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

    // Helper function to generate codes from the tree
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
        // Clean up tree memory (if necessary)
        // Note: Implement a recursive delete function if needed.
    }

    // Override encode method
    std::string encode(const std::string &input) override
    {
        // Build frequency table and tree
        auto frequencyTable = BuildFrequencyTable(input);
        root = BuildTree(frequencyTable);

        // Generate codes from the tree
        codes.clear();
        GenerateCodes(root, "");

        // Encode input string
        std::string encodedString;
        for (char c : input)
        {
            encodedString += codes[c];
        }

        return encodedString;
    }

    // Override decode method
    std::string decode(const std::string &input) override
    {
        if (!root)
            throw std::runtime_error("Decoding requires a valid Huffman tree.");

        std::string decodedString;
        HuffmanTreeNode *current = root;

        for (char bit : input)
        {
            if (bit == '0')
                current = current->GetLeftChild();
            else if (bit == '1')
                current = current->GetRightChild();

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