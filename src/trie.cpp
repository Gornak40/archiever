#include "trie.h"

Node::Node(Symbol c, size_t cnt, Node* left, Node* right) : c_(c), cnt_(cnt), left_(left), right_(right) {
    if (left_ != nullptr) {
        left_->parent_ = this;
    }
    if (right_ != nullptr) {
        right_->parent_ = this;
    }
}

Node::~Node() {
    delete left_;
    delete right_;
}

Node*& Node::GetSon(bool x) {
    return (x ? right_ : left_);
}

bool Node::IsTerminate() const {
    return left_ == nullptr && right_ == nullptr;
}

Symbol Node::GetC() const {
    return c_;
}

size_t Node::GetCnt() const {
    return cnt_;
}

Code Node::GoUp() const {
    Code result;
    for (const Node* v = this; v->parent_ != nullptr; v = v->parent_) {
        result.push_back(v == v->parent_->right_);
    }
    std::reverse(result.begin(), result.end());
    return result;
}

void Node::GoDown(Symbol c, const Code& code) {
    Node* v = this;
    for (bool x : code) {
        Node*& son = v->GetSon(x);
        if (son == nullptr) {
            son = new Node();
            son->parent_ = v;
        }
        v = son;
    }
    v->c_ = c;
}

bool Node::Comp::operator()(Node* a, Node* b) const {
    return std::make_pair(a->GetCnt(), a->GetC()) < std::make_pair(b->GetCnt(), b->GetC());
}
