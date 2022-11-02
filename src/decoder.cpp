#include "decoder.h"

Decoder::Decoder(std::filesystem::path name) : reader_(std::move(name)) {
    do {
    } while (DecodeFile());
}

bool Decoder::DecodeFile() {
    std::vector<Symbol> order;
    std::array<size_t, MAX_SYMBOLS_COUNT> code_len;
    ReadOrder(order, code_len);
    std::array<Code, MAX_SYMBOLS_COUNT> codes;
    BuildCanonical(order, code_len, codes);
    Node* root = BuildTrie(order, codes);
    std::string name;
    ReadName(root, name);
    bool result = ReadWriteContent(root, name);
    delete root;
    return result;
}

void Decoder::ReadOrder(std::vector<Symbol>& order, std::array<size_t, MAX_SYMBOLS_COUNT>& code_len) {
    Symbol symbols_count = reader_.ReadSymbol<SYMBOL_SIZE>();
    for (size_t i = 0; i < static_cast<size_t>(symbols_count); ++i) {
        order.push_back(reader_.ReadSymbol<SYMBOL_SIZE>());
    }
    std::fill(code_len.begin(), code_len.end(), 0);
    for (size_t i = 0, len = 1; i < static_cast<size_t>(symbols_count); ++len) {
        for (size_t cnt = reader_.ReadSymbol<9>(); cnt; --cnt, ++i) {
            code_len[order[i]] = len;
        }
    }
}

Node* Decoder::BuildTrie(const std::vector<Symbol>& order, const std::array<Code, MAX_SYMBOLS_COUNT>& codes) {
    Node* root = new Node();
    for (Symbol c : order) {
        root->GoDown(c, codes[c]);
    }
    return root;
}

Symbol Decoder::ReadSymbol(Node* v) {
    while (!v->IsTerminate()) {
        v = v->GetSon(reader_.ReadSymbol<BIT_SIZE>());
    }
    return v->GetC();
}

void Decoder::ReadName(Node* root, std::string& name) {
    for (Symbol c = ReadSymbol(root); c != FILENAME_END; c = ReadSymbol(root)) {
        name.push_back(static_cast<char>(c));
    }
}

bool Decoder::ReadWriteContent(Node* root, const std::string& name) {
    Writer writer(name);
    for (Symbol c = ReadSymbol(root);; c = ReadSymbol(root)) {
        switch (c) {
            case ONE_MORE_FILE:
                return true;
            case ARCHIVE_END:
                return false;
            default:
                writer.WriteSymbol<BYTE_SIZE>(c);
        }
    }
}
