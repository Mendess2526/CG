#ifndef GENERATOR_H
#define GENERATOR_H
#include "../common/point.hpp"
#include <vector>

class Primitive {
public:
    virtual std::vector<Point> draw() const = 0;
    virtual ~Primitive() {};
};

#endif // GENERATOR_H
