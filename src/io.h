#pragma once

#include <fstream>
#include <filesystem>
#include <vector>

using Symbol = int32_t;
using Code = std::vector<bool>;

const Symbol FILENAME_END = 256;
const Symbol ONE_MORE_FILE = 257;
const Symbol ARCHIVE_END = 258;
const size_t MAX_SYMBOLS_COUNT = 259;
const size_t BYTE_SIZE = 8;
const size_t SYMBOL_SIZE = 9;
const size_t BIT_SIZE = 1;

class Reader {
public:
    explicit Reader(std::filesystem::path name);

    template <size_t cnt>
    bool ReadSymbol(Symbol& x) {
        while (cnt_ < cnt) {
            if (file_.peek() == EOF) {
                return false;
            }
            Symbol c = file_.get();
            buf_ = (buf_ << 8) | c;
            cnt_ += 8;
        }
        x = (buf_ & ((1 << cnt_) - 1)) >> (cnt_ - cnt);
        cnt_ -= cnt;
        return true;
    }

    template <size_t cnt>
    Symbol ReadSymbol() {
        Symbol res = 0;
        ReadSymbol<cnt>(res);
        return res;
    }

private:
    std::ifstream file_;
    Symbol buf_ = 0;
    size_t cnt_ = 0;
};

class Writer {
public:
    explicit Writer(std::filesystem::path name);
    ~Writer();

    void Eat(const Code& line);

    template <size_t cnt>
    void WriteSymbol(Symbol x) {
        buf_ = (buf_ << cnt) | x;
        cnt_ += cnt;
        while (cnt_ >= 8) {
            file_ << static_cast<unsigned char>((buf_ & ((1 << cnt_) - 1)) >> (cnt_ - 8));
            cnt_ -= 8;
        }
    }

private:
    std::ofstream file_;
    Symbol buf_ = 0;
    size_t cnt_ = 0;
};

void BuildCanonical(const std::vector<Symbol>& order, const std::array<size_t, MAX_SYMBOLS_COUNT>& code_len,
                    std::array<Code, MAX_SYMBOLS_COUNT>& codes);