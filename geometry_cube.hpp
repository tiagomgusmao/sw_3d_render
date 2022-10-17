#include <tuple>
std::tuple<double, double, double, double> geom_vertex_list [] {
{ 1.0, 1.0, 1.0, 0.0 },
{ 1.0, 1.0, -1.0, 0.0 },
{ 1.0, -1.0, 1.0, 0.0 },
{ 1.0, -1.0, -1.0, 0.0 },
{ -1.0, 1.0, 1.0, 0.0 },
{ -1.0, 1.0, -1.0, 0.0 },
{ -1.0, -1.0, 1.0, 0.0 },
{ -1.0, -1.0, -1.0, 0.0 },
};

std::tuple<int, int, int> geom_vertex_indices [] {
{ 4, 2, 0 },
{ 2, 7, 3 },
{ 6, 5, 7 },
{ 1, 7, 5 },
{ 0, 3, 1 },
{ 4, 1, 5 },
{ 4, 6, 2 },
{ 2, 6, 7 },
{ 6, 4, 5 },
{ 1, 3, 7 },
{ 0, 2, 3 },
{ 4, 0, 1 },
};

std::tuple<double, double, double, double> geom_normal_list [] {
{ 0.0, 0.0, 1.0, 1.0 },
{ 0.0, 0.0, 1.0, 1.0 },
{ 0.0, 0.0, 1.0, 1.0 },
{ 0.0, -1.0, 0.0, 1.0 },
{ 0.0, -1.0, 0.0, 1.0 },
{ 0.0, -1.0, 0.0, 1.0 },
{ -1.0, 0.0, 0.0, 1.0 },
{ -1.0, 0.0, 0.0, 1.0 },
{ -1.0, 0.0, 0.0, 1.0 },
{ 0.0, 0.0, -1.0, 1.0 },
{ 0.0, 0.0, -1.0, 1.0 },
{ 0.0, 0.0, -1.0, 1.0 },
{ 1.0, 0.0, 0.0, 1.0 },
{ 1.0, 0.0, 0.0, 1.0 },
{ 1.0, 0.0, 0.0, 1.0 },
{ 0.0, 1.0, 0.0, 1.0 },
{ 0.0, 1.0, 0.0, 1.0 },
{ 0.0, 1.0, 0.0, 1.0 },
{ 0.0, 0.0, 1.0, 1.0 },
{ 0.0, 0.0, 1.0, 1.0 },
{ 0.0, 0.0, 1.0, 1.0 },
{ 0.0, -1.0, 0.0, 1.0 },
{ 0.0, -1.0, 0.0, 1.0 },
{ 0.0, -1.0, 0.0, 1.0 },
{ -1.0, 0.0, 0.0, 1.0 },
{ -1.0, 0.0, 0.0, 1.0 },
{ -1.0, 0.0, 0.0, 1.0 },
{ 0.0, 0.0, -1.0, 1.0 },
{ 0.0, 0.0, -1.0, 1.0 },
{ 0.0, 0.0, -1.0, 1.0 },
{ 1.0, 0.0, 0.0, 1.0 },
{ 1.0, 0.0, 0.0, 1.0 },
{ 1.0, 0.0, 0.0, 1.0 },
{ 0.0, 1.0, 0.0, 1.0 },
{ 0.0, 1.0, 0.0, 1.0 },
{ 0.0, 1.0, 0.0, 1.0 },
};

const int triangleCount = 12;

std::tuple<int, int, int> geom_vertex_colors [] {
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
{ 0.800000011920929, 0.800000011920929, 0.800000011920929 },
};

std::tuple<double, double> geom_uv_coords [] {
{ 0.6666666269302368, 0.0 },
{ 0.9999998807907104, 0.3333333134651184 },
{ 0.9999999403953552, 0.0 },
{ 0.0, 0.33333325386047363 },
{ 0.3333333134651184, 0.0 },
{ 0.0, 0.0 },
{ 0.0, 0.6666666269302368 },
{ 0.3333333134651184, 0.3333333730697632 },
{ 0.0, 0.3333333730697632 },
{ 0.3333333730697632, 0.0 },
{ 0.6666666269302368, 0.3333333134651184 },
{ 0.6666666865348816, 0.0 },
{ 0.3333333730697632, 0.6666666269302368 },
{ 0.6666666865348816, 0.3333333730697632 },
{ 0.3333333730697632, 0.3333333730697632 },
{ 0.0, 0.9999998807907104 },
{ 0.3333333134651184, 0.6666666269302368 },
{ 0.0, 0.6666666269302368 },
{ 0.6666666269302368, 0.0 },
{ 0.6666666269302368, 0.33333325386047363 },
{ 0.9999998807907104, 0.3333333134651184 },
{ 0.0, 0.33333325386047363 },
{ 0.33333325386047363, 0.3333333134651184 },
{ 0.3333333134651184, 0.0 },
{ 0.0, 0.6666666269302368 },
{ 0.33333325386047363, 0.6666666865348816 },
{ 0.3333333134651184, 0.3333333730697632 },
{ 0.3333333730697632, 0.0 },
{ 0.3333333730697632, 0.33333325386047363 },
{ 0.6666666269302368, 0.3333333134651184 },
{ 0.3333333730697632, 0.6666666269302368 },
{ 0.6666666269302368, 0.6666666865348816 },
{ 0.6666666865348816, 0.3333333730697632 },
{ 0.0, 0.9999998807907104 },
{ 0.33333325386047363, 0.9999999403953552 },
{ 0.3333333134651184, 0.6666666269302368 },
};

//tri_count= 12
//vertices 8
//split_normals= 36
//indices= 36
//colors= 36
//uvs= 36