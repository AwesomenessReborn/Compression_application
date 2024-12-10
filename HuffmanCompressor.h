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

        std::string encodedString;
        for (char c : input)
        {
            encodedString += codes[c];
        }

        return encodedString;
    }

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
