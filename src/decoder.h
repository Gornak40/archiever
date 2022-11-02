#pragma once

#include <string>
#include <filesystem>
#include "io.h"
#include "trie.h"

class Decoder {
public:
    explicit Decoder(std::filesystem::path name);

private:
    bool DecodeFile();
    void ReadOrder(std::vector<Symbol>& order, std::array<size_t, MAX_SYMBOLS_COUNT>& code_len);
    Node* BuildTrie(const std::vector<Symbol>& order, const std::array<Code, MAX_SYMBOLS_COUNT>& codes);
    Symbol ReadSymbol(Node* root);
    void ReadName(Node* root, std::string& name);
    bool ReadWriteContent(Node* root, const std::string& name);

    Reader reader_;
};