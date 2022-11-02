#pragma once

#include <exception>

namespace exceptions {
class BadReader : public std::exception {};
class BadWriter : public std::exception {};
const int ERROR_CODE = 111;
}  // namespace exceptions