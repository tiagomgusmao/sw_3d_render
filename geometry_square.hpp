#include <tuple>

std::tuple<double, double, double, double> geom_vertex_list [] {
{ -1.0, 1.0, 0.0, 1.0 },
{ 1.0, 1.0, 0.0, 1.0 },
{ 1.0, -1.0, 0.0, 1.0 },
{ -1.0, -1.0, 0.0, 1.0 },
};

std::tuple<int, int, int> geom_vertex_indices [] {
{ 0, 2, 1 },
{ 0, 3, 2 },
};

std::tuple<double, double, double, double> geom_normal_list [] {
{ 0.0, 0.0, 1.0, 1.0 },
{ 0.0, 0.0, 1.0, 1.0 },
{ 0.0, 0.0, 1.0, 1.0 },
{ 0.0, 0.0, 1.0, 1.0 },
{ 0.0, 0.0, 1.0, 1.0 },
{ 0.0, 0.0, 1.0, 1.0 },
};

const int triangleCount = 2;

std::tuple<int, int, int> geom_vertex_colors [] {
{ 255, 255, 255 },
{ 255, 255, 255 },
{ 255, 255, 255 },
{ 255, 255, 255 },
{ 255, 255, 255 },
{ 255, 255, 255 },
};

std::tuple<double, double> geom_uv_coords [] {
{ 0.0, 0.0 },
{ 1.0, 0.0 },
{ 1.0, 1.0 },
{ 0.0, 1.0 },
};
