#pragma once

#include <array>
#include <cstddef>
#include <vector>
#include "io.h"

class Node {
public:
    struct Comp {
        bool operator()(Node* a, Node* b) const;
    };

    explicit Node(Symbol c = 0, size_t cnt = 0, Node* left = nullptr, Node* right = nullptr);
    ~Node();

    Node*& GetSon(bool x);
    bool IsTerminate() const;
    Symbol GetC() const;
    size_t GetCnt() const;
    Code GoUp() const;
    void GoDown(Symbol c, const Code& code);

private:
    Symbol c_;
    size_t cnt_;
    Node* parent_ = nullptr;
    Node* left_;
    Node* right_;
};
