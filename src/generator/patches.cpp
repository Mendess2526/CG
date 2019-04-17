#include "patches.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

template <typename T>
static vector<T> parse_line(string, string);

Patches::Patches(string filename, int t)
    : tessellation(t)
{
    ifstream control_points(filename);
    if (!control_points.good()) {
        string error("File not found: ");
        error.append(filename);
        throw error;
    }
    size_t patch_count;
    control_points >> patch_count;
    string line;                   // TODO: Ugly
    getline(control_points, line); // TODO: Ugly
    for (size_t i = 0; i < patch_count; i++) {
        string line;
        getline(control_points, line);
        vector<size_t> indices = parse_line<size_t>(line, "Error parsing indices");
        patches.push_back(Patch(indices));
    }
    size_t point_count;
    control_points >> point_count;
    getline(control_points, line); // TODO: Ugly
    for (size_t i = 0; i < point_count; i++) {
        string line;
        getline(control_points, line);
        vector<float> coords = parse_line<float>(line, "Error parsing control points");
        points.push_back(Point(coords[0], coords[1], coords[2]));
    }
}

vector<Point> Patches::draw() const
{
    vector<Point> points;
    const float delta = 1.0 / tessellation;
    for (size_t i = 0; i < patches.size(); i++) {
        for (size_t iu = 0; iu < tessellation; iu++) {
            float u = delta * iu;
            float uNext = delta * (iu + 1);
            for (size_t iv = 0; iv < tessellation; iv++) {
                float v = delta * iv;
                float vNext = delta * (iv + 1);
                Point p0 = bezier_point(i, u, v);
                Point p1 = bezier_point(i, u, vNext);
                Point p2 = bezier_point(i, uNext, v);
                Point p3 = bezier_point(i, uNext, vNext);

                points.push_back(p1);
                points.push_back(p2);
                points.push_back(p3);

                points.push_back(p1);
                points.push_back(p0);
                points.push_back(p2);
            }
        }
    }
    return points;
}

static void mult_vec_matrix4(const float[4], const float[4][4], float[4]);

static const float M[4][4] = {
    { -1, +3, -3, +1 },
    { +3, -6, +3, +0 },
    { -3, +3, +0, +0 },
    { +1, +0, +0, +0 }
};

#define T(t)                   \
    {                          \
        t *t *t, t *t, t, 1.0f \
    }

Point Patches::bezier_point(size_t patch, float du, float dv) const
{
    const float u[4] = T(du);
    float u_m[4];
    mult_vec_matrix4(u, M, u_m);
    const float v[4] = T(dv);
    float v_m[4];
    mult_vec_matrix4(v, M, v_m);
    Point points[4][4];
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            points[i][j] = point_at(patch, i * 4 + j);
        }
    }
    Point ump[4];
    for (size_t i = 0; i < 4; i++) {
        ump[i] = Point();
        for (size_t j = 0; j < 4; j++) {
            ump[i] = ump[i] + points[j][i] * u_m[j];
        }
    }
    Point r = Point();
    for (size_t i = 0; i < 4; i++) {
        r = r + ump[i] * v_m[i];
    }
    return r;
}

void mult_vec_matrix4(const float v[4], const float m[4][4], float res[4])
{
    for (size_t i = 0; i < 4; i++) {
        res[i] = 0;
        for (size_t j = 0; j < 4; j++) {
            res[i] += v[j] * m[i][j];
        }
    }
}

Point Patches::point_at(size_t patch, size_t index) const
{
    return points[patches[patch].control_points[index]];
}

string Patches::to_string() const
{
    stringstream r;
    r << patches.size() << endl;
    for (const auto& patch : patches) {
        r << patch.to_string() << endl;
    }
    r << points.size() << endl;
    for (const auto& point : points) {
        r << " " << point.x() << ", " << point.y() << ", " << point.z() << endl;
    }
    return r.str();
}

template <typename T>
vector<T> parse_line(string line, string error)
{
    const string delimiter = ", ";
    stringstream linestream(line);
    vector<T> elems;
    string token;
    while (getline(linestream, token, ',')) {
        if (token == " ")
            continue;
        T n;
        stringstream ss(token);
        ss >> n;
        if (ss.fail()) {
            throw error;
        }
        elems.push_back(n);
    }
    return elems;
}

Patch::Patch(vector<size_t> v)
{
    for (size_t i = 0; i < control_points.size(); i++) {
        control_points[i] = v[i];
    }
}

string Patch::to_string() const
{
    stringstream r;
    const char* s = "";
    for (size_t i : control_points) {
        r << s << i;
        s = ", ";
    }
    return r.str();
}