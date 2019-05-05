#ifndef GROUP_HPP
#define GROUP_HPP

#include "../dependencies/rapidxml.hpp"
#include "model.hpp"
#include "transformations.hpp"
#include <memory>
#include <optional>
#include <vector>

class Group {
private:
    std::vector<std::unique_ptr<Transformation>> transformations;
    std::vector<std::unique_ptr<Model>> models;
    std::vector<Group> subgroups;
    int _levels;
    int _model_count;
    float r, g, b, a;
    bool random_color;

    Group(rapidxml::xml_node<char>*, float, float, float, float);
    std::optional<Matrix> get_model_position(size_t, Matrix, double) const;

public:
    Group(rapidxml::xml_node<char>* group)
        : Group(group, 0, 0, 0, 1)
    {
    }

    int levels() const { return _levels; };
    int model_count() const { return _model_count; }
    std::optional<Point> get_model_position(size_t, double) const;
    void prepare();
    void draw(int, double) const;
};

#endif // GROUP_HPP
