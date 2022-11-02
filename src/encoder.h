#pragma once

#include <list>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <functional>
#include "io.h"
#include "trie.h"

class Encoder {
public:
    Encoder(std::filesystem::path path, const std::list<std::filesystem::path>& names);

private:
    void EncodeFile(const std::filesystem::path& name, bool is_end);
    void BuildFreq(const std::filesystem::path& name, std::array<size_t, MAX_SYMBOLS_COUNT>& freq);
    Node* BuildTrie(const std::array<size_t, MAX_SYMBOLS_COUNT>& freq, std::array<Node*, MAX_SYMBOLS_COUNT>& link);
    void BuildOrder(const std::array<Node*, MAX_SYMBOLS_COUNT>& link, std::vector<Symbol>& order,
                    std::array<size_t, MAX_SYMBOLS_COUNT>& code_len);
    void WriteHeader(std::vector<Symbol> order, std::array<size_t, MAX_SYMBOLS_COUNT> code_len,
                     std::array<Code, MAX_SYMBOLS_COUNT>& codes);
    void WriteContent(const std::filesystem::path& name, bool is_end, const std::array<Code, MAX_SYMBOLS_COUNT>& codes);

    Writer writer_;
};