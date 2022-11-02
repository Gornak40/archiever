#include "encoder.h"
#include "io.h"
#include "heap.h"

Encoder::Encoder(std::filesystem::path path, const std::list<std::filesystem::path>& names) : writer_(std::move(path)) {
    for (auto it = names.begin(); it != names.end(); ++it) {
        EncodeFile(*it, std::next(it) == names.end());
    }
}

void Encoder::EncodeFile(const std::filesystem::path& name, bool is_end) {
    std::array<size_t, MAX_SYMBOLS_COUNT> freq;
    BuildFreq(name, freq);
    std::array<Node*, MAX_SYMBOLS_COUNT> link;
    Node* root = BuildTrie(freq, link);
    std::vector<Symbol> order;
    std::array<size_t, MAX_SYMBOLS_COUNT> code_len;
    BuildOrder(link, order, code_len);
    std::array<Code, MAX_SYMBOLS_COUNT> codes;
    BuildCanonical(order, code_len, codes);
    WriteHeader(order, code_len, codes);
    WriteContent(name, is_end, codes);
    delete root;
}

void Encoder::BuildFreq(const std::filesystem::path& name, std::array<size_t, MAX_SYMBOLS_COUNT>& freq) {
    std::fill(freq.begin(), freq.end(), 0);
    for (Symbol c : name.filename().string()) {
        ++freq[c];
    }
    ++freq[FILENAME_END];
    Reader reader(name);
    for (Symbol c = 0; reader.ReadSymbol<BYTE_SIZE>(c);) {
        ++freq[c];
    }
    ++freq[ONE_MORE_FILE];
    ++freq[ARCHIVE_END];
}

Node* Encoder::BuildTrie(const std::array<size_t, MAX_SYMBOLS_COUNT>& freq,
                         std::array<Node*, MAX_SYMBOLS_COUNT>& link) {
    std::fill(link.begin(), link.end(), nullptr);
    Heap<Node*, Node::Comp> heap;
    for (Symbol c = 0; c < static_cast<Symbol>(MAX_SYMBOLS_COUNT); ++c) {
        if (freq[c] != 0) {
            link[c] = new Node(c, freq[c]);
            heap.Push(link[c]);
        }
    }
    while (heap.Size() > 1) {
        Node* a = heap.Pop();
        Node* b = heap.Pop();
        heap.Push(new Node(std::min(a->GetC(), b->GetC()), a->GetCnt() + b->GetCnt(), a, b));
    }
    return heap.Top();
}

void Encoder::BuildOrder(const std::array<Node*, MAX_SYMBOLS_COUNT>& link, std::vector<Symbol>& order,
                         std::array<size_t, MAX_SYMBOLS_COUNT>& code_len) {
    for (Symbol c = 0; c < static_cast<Symbol>(MAX_SYMBOLS_COUNT); ++c) {
        if (link[c] != nullptr) {
            order.push_back(c);
        }
    }
    std::fill(code_len.begin(), code_len.end(), 0);
    for (Symbol c : order) {
        code_len[c] = link[c]->GoUp().size();
    }
    std::stable_sort(order.begin(), order.end(), [&code_len](Symbol a, Symbol b) { return code_len[a] < code_len[b]; });
}

void Encoder::WriteHeader(std::vector<Symbol> order, std::array<size_t, MAX_SYMBOLS_COUNT> code_len,
                          std::array<Code, MAX_SYMBOLS_COUNT>& codes) {

    writer_.WriteSymbol<SYMBOL_SIZE>(static_cast<Symbol>(order.size()));
    for (Symbol c : order) {
        writer_.WriteSymbol<SYMBOL_SIZE>(c);
    }
    std::sort(code_len.begin(), code_len.end());
    size_t max = code_len.back();
    for (size_t i = 1; i <= max; ++i) {
        auto itl = std::lower_bound(code_len.begin(), code_len.end(), i);
        auto itr = std::upper_bound(code_len.begin(), code_len.end(), i);
        writer_.WriteSymbol<9>(static_cast<Symbol>(itr - itl));
    }
}

void Encoder::WriteContent(const std::filesystem::path& name, bool is_end,
                           const std::array<Code, MAX_SYMBOLS_COUNT>& codes) {
    for (Symbol c : name.filename().string()) {
        writer_.Eat(codes[c]);
    }
    writer_.Eat(codes[FILENAME_END]);
    Reader reader(name);
    for (Symbol c = 0; reader.ReadSymbol<BYTE_SIZE>(c);) {
        writer_.Eat(codes[c]);
    }
    writer_.Eat(codes[is_end ? ARCHIVE_END : ONE_MORE_FILE]);
}
