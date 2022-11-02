#include "io.h"
#include "exceptions.h"

Reader::Reader(std::filesystem::path name) : file_(std::move(name)) {
    if (!file_.good()) {
        throw exceptions::BadReader();
    }
}

void Writer::Eat(const Code& line) {
    for (bool x : line) {
        WriteSymbol<1>(x);
    }
}
Writer::Writer(std::filesystem::path name) : file_(std::move(name)) {
    if (!file_.good()) {
        throw exceptions::BadWriter();
    }
}

Writer::~Writer() {
    if (cnt_ != 0) {
        file_ << static_cast<unsigned char>((buf_ & ((1 << cnt_) - 1)) << (8 - cnt_));
    }
}

void BuildCanonical(const std::vector<Symbol>& order, const std::array<size_t, MAX_SYMBOLS_COUNT>& code_len,
                    std::array<Code, MAX_SYMBOLS_COUNT>& codes) {
    codes[order.front()].assign(code_len[order.front()], false);
    for (auto it = std::next(order.begin()); it != order.end(); ++it) {
        codes[*it] = codes[*std::prev(it)];
        auto last_zero = std::find_if_not(codes[*it].rbegin(), codes[*it].rend(), [](bool x) { return x; });
        *last_zero = true;
        std::fill(codes[*it].rbegin(), last_zero, false);
        codes[*it].resize(code_len[*it], false);
    }
}
