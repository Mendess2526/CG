#ifndef PARSER_H
#define PARSER_H
#include "group.hpp"
#include <string>
#include <vector>

namespace Parser {
Group* load(std::vector<std::string> configs);
}

#endif // PARSER_H
