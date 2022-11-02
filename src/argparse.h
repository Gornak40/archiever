#pragma once

#include <list>
#include <string>
#include <map>
#include <filesystem>

class ArgParse {
public:
    enum class Mode {
        ENCODE,
        DECODE,
        HELP,
    };

    ArgParse(int argc, char** argv);
    Mode GetMode() const;
    std::filesystem::path GetArgument(const std::string& s) const;
    std::list<std::filesystem::path> GetFreeArguments() const;

private:
    std::map<std::string, std::filesystem::path> arguments_;
    std::list<std::filesystem::path> free_arguments_;
};