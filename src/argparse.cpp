#include "argparse.h"
#include <iostream>

ArgParse::ArgParse(int argc, char** argv) {
    auto end = argv + argc;
    for (auto it = std::next(argv); it != end; ++it) {
        if (**it == '-') {
            arguments_[*it] = *std::next(it);
            ++it;
        } else {
            free_arguments_.push_back(*it);
        }
    }
}

ArgParse::Mode ArgParse::GetMode() const {
    if (arguments_.contains("-d") && arguments_.size() == 1) {
        return Mode::DECODE;
    }
    if (arguments_.contains("-c") && arguments_.size() == 1) {
        return Mode::ENCODE;
    }
    return Mode::HELP;
}

std::list<std::filesystem::path> ArgParse::GetFreeArguments() const {
    return free_arguments_;
}

std::filesystem::path ArgParse::GetArgument(const std::string& s) const {
    return arguments_.at(s);
}
