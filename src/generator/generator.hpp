#include "../common/point.hpp"
#include <vector>

std::vector<Point> draw_plane(double side_length);

std::vector<Point> draw_box(double x, double y, double z);

std::vector<Point> draw_sphere(double radius, double slices, double stacks);

std::vector<Point> draw_cone(double radius, double slices, double stacks);