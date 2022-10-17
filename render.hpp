#ifndef RENDER_HPP
#define RENDER_HPP

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <array>
#include <tuple>
#include <cassert>

#include "render_math.hpp"

using RenderBufferType = uint32_t;
using ZBufferType = uint32_t;

#define PERSP_CORR 1

const int screenWidth = 1280;
const int screenHeight = 720;
const int bytesPerPixel = 4;
const int textureSize = 2048;

void plot_line(RenderBufferType *buffer, ZBufferType *zBuffer, int x0, int y0, int z0, int x1, int y1, int z1)
{
    int dx = std::abs(x1 - x0);
    int sx = x0 < x1?1:-1;
    int dy = -std::abs(y1 - y0);
    int sy = y0 < y1? 1: -1;
    int error = dx + dy;

    bool calc_z_by_x = std::abs(x0 - x1) > std::abs(y0 - y1);

    const int u0 = calc_z_by_x? x0: y0;
    const int u1 = calc_z_by_x? x1: y1;
    const int u_len = u1 - u0;
    int & u_var = calc_z_by_x? x0: y0;

    while(true)
    {
        double x_half = screenWidth / 2.0;
        double y_half = screenHeight / 2.0;
        int cx0 = x0 + x_half;
        int cy0 = -y0 + y_half;

        if(cy0 < screenHeight && cx0 <screenWidth && cy0>=0 && cx0>=0)
        {
            double normalized_progress = (u_var-u0)/(double)u_len;

            const int pixIndex = cx0 + cy0 * screenWidth;
            const int z = (z1 * normalized_progress) + (z0 * (1.0-normalized_progress));

            if(z<zBuffer[pixIndex] && z>=0)
            {
                buffer[pixIndex] = 0xFFFFFFFF;
                zBuffer[pixIndex] = z;
            }

        }
        if(x0 == x1 && y0 == y1)
        {
            break;
        }
        int e2 = 2 * error;
        if(e2 >= dy)
        {
            if(x0 == x1)
            {
                break;
            }
            error = error + dy;
            x0 = x0 + sx;
        }
        if(e2 <= dx)
        {
            if(y0 == y1)
            {
                break;
            }
            error = error + dx;
            y0 = y0 + sy;
        }
    }
}

void draw_wireframe_triangle(RenderBufferType *buffer, ZBufferType *zBuffer, int x0, int y0, int z0, int x1, int y1, int z1, int x2, int y2, int z2)
{
    plot_line(buffer, zBuffer, x0, y0, z0, x1, y1, z1);
    plot_line(buffer, zBuffer, x1, y1, z1, x2, y2, z2);
    plot_line(buffer, zBuffer, x2, y2, z2, x0, y0, z0);
}

void draw_filled_triangle(RenderBufferType *buffer, ZBufferType *zBuffer, int x0, int y0, int z0, double z0_norm, Color3i c0, Uv2f uv0, int x1, int y1, int z1, double z1_norm, Color3i c1, Uv2f uv1, int x2, int y2, int z2, double z2_norm, Color3i c2, Uv2f uv2, uint32_t *texture)
{
    const int min_y = std::min(std::min(y0, y1), y2);
    const int max_y = std::max(std::max(y0, y1), y2);
    const int min_x = std::min(std::min(x0, x1), x2);
    const int max_x = std::max(std::max(x0, x1), x2);

    for(int x = min_x; x<=max_x; ++x)
        for(int y = min_y; y<=max_y; ++y)
        {
            double d1=(x-x0)*(y1-y0)-(y-y0)*(x1-x0);
            double d2=(x-x1)*(y2-y1)-(y-y1)*(x2-x1);
            double d3=(x-x2)*(y0-y2)-(y-y2)*(x0-x2);

            if((d1<=0.0 && d2<=0.0 && d3<=0.0))
            {
                double x_half = screenWidth / 2.0;
                double y_half = screenHeight / 2.0;
                int cx0 = x + x_half;
                int cy0 = -y + y_half;

                if(cy0 < screenHeight && cx0 <screenWidth && cy0>=0 && cx0>=0)
                {
                    const int pixIndex = cx0 + cy0 * screenWidth;

                    const double d1_norm = d1/((x2-x0)*(y1-y0)-(y2-y0)*(x1-x0));
                    const double d2_norm= d2/((x0-x1)*(y2-y1)-(y0-y1)*(x2-x1));
                    const double d3_norm= d3/((x1-x2)*(y0-y2)-(y1-y2)*(x0-x2));

                    const int z = z0 * d2_norm + z1 * d3_norm + z2 * d1_norm;

                    if(z<zBuffer[pixIndex] && z>=0)
                    {

#ifdef PERSP_CORR
                        const double z0_recip = 1.0/z0_norm;
                        const double z1_recip = 1.0/z1_norm;
                        const double z2_recip = 1.0/z2_norm;

                        const double u0_recip = uv0.U() / z0_norm;
                        const double u1_recip = uv1.U() / z1_norm;
                        const double u2_recip = uv2.U() / z2_norm;

                        const double v0_recip = uv0.V() / z0_norm;
                        const double v1_recip = uv1.V() / z1_norm;
                        const double v2_recip = uv2.V() / z2_norm;

                        const double u_interp = d1_norm * u2_recip + d2_norm * u0_recip + d3_norm * u1_recip;
                        const double v_interp = d1_norm * v2_recip + d2_norm * v0_recip + d3_norm * v1_recip;
                        const double z_interp = d1_norm * z2_recip + d2_norm * z0_recip + d3_norm * z1_recip;

                        const double z_correct = 1.0 / z_interp;
                        const double u_correct = u_interp * z_correct;
                        const double v_correct = v_interp * z_correct;


                        const int u_int = u_correct * (textureSize -1);
                        const int v_int = v_correct * (textureSize -1);
#else
                        const double u_interp = d1_norm * uv2.U() + d2_norm * uv0.U() + d3_norm * uv1.U();
                        const double v_interp = d1_norm * uv2.V() + d2_norm * uv0.V() + d3_norm * uv1.V();

                        const int u_int = u_interp * (textureSize -1);
                        const int v_int = v_interp * (textureSize -1);
#endif

                        int texel_base_index = (((textureSize-v_int-1) * textureSize) + u_int);

//                        qDebug() << "u " << u_interp << "v " << v_interp << "tex " << texel_base_index << "\n";


//                        uint8_t color_r = u_int;
//                        uint8_t color_g = v_int;
//                        uint8_t color_b = 0;

                        uint8_t texel_r = (texture[texel_base_index] >> 24) & 0xFF;
                        uint8_t texel_g = (texture[texel_base_index] >> 16) & 0xFF;
                        uint8_t texel_b = (texture[texel_base_index] >> 8) & 0xFF;

                        uint8_t r_intensity = (d1_norm * c2.R() + d2_norm * c0.R() + d3_norm * c1.R());
                        uint8_t g_intensity = (d1_norm * c2.G() + d2_norm * c0.G() + d3_norm * c1.G());
                        uint8_t b_intensity = (d1_norm * c2.B() + d2_norm * c0.B() + d3_norm * c1.B());

                        uint8_t r_mixed = (r_intensity * texel_r) / 255;
                        uint8_t g_mixed = (g_intensity * texel_g) / 255;
                        uint8_t b_mixed = (b_intensity * texel_b) / 255;
                        //buffer[pixIndex] = 0xFF000000 + (r_intensity << 16) + (g_intensity << 8) + (b_intensity << 0);
                        buffer[pixIndex] = 0xFF000000 + (r_mixed << 16) + (g_mixed << 8) + (b_mixed << 0);
                        zBuffer[pixIndex] = z;
                    }
                }
            }
        }
}


void render_triangles(RenderBufferType *buffer, ZBufferType *zBuffer, Matrix4f &p_matrix, Matrix4f &m_matrix, std::tuple<Point4f, Point4f, Point4f> &triangle, std::tuple<Point4f, Point4f, Point4f> &normals, std::tuple<Color3i, Color3i, Color3i> &colors, std::tuple<Uv2f, Uv2f, Uv2f> uvs, uint32_t *texture)
{
    double smallestHalfDim = std::min(screenWidth, screenHeight) / 2.0;

    Point4f ip1 = m_matrix.transform(std::get<0>(triangle));
    Point4f ip2 = m_matrix.transform(std::get<1>(triangle));
    Point4f ip3 = m_matrix.transform(std::get<2>(triangle));

    // To avoid NaNs and Infs we'll ignore any triangle with any point closer to the 'camera' than Z-near.
    // The proper solution would be to clip the triangles with the Z-near plane.
    if(ip1.Z() > -p_matrix.getZNear() || ip2.Z() > -p_matrix.getZNear() || ip3.Z() > -p_matrix.getZNear())
    {
        return;
    }

    Point4f light_normal = {0.0, 0.0, 1.0};

    Matrix4f normal_rotation(m_matrix);
    normal_rotation.removeTranslationAndScale();

    Point4f &n1 = std::get<0>(normals);
    Point4f &n2 = std::get<1>(normals);
    Point4f &n3 = std::get<2>(normals);

    Point4f n1_t = normal_rotation.transform(n1);
    Point4f n2_t = normal_rotation.transform(n2);
    Point4f n3_t = normal_rotation.transform(n3);

    double i1 = std::abs(n1_t.dotProduct3(light_normal));
    double i2 = std::abs(n2_t.dotProduct3(light_normal));
    double i3 = std::abs(n3_t.dotProduct3(light_normal));

    Color3i &c1 = std::get<0>(colors);
    Color3i &c2 = std::get<0>(colors);
    Color3i &c3 = std::get<0>(colors);

    Color3i c1_int = {int(c1.R() * i1), int(c1.G() * i1), int(c1.B() * i1)};
    Color3i c2_int = {int(c2.R() * i1), int(c2.G() * i1), int(c2.B() * i1)};
    Color3i c3_int = {int(c3.R() * i1), int(c3.G() * i1), int(c3.B() * i1)};

    Uv2f &uv1 = std::get<0>(uvs);
    Uv2f &uv2 = std::get<1>(uvs);
    Uv2f &uv3 = std::get<2>(uvs);

    Point4f p1 = p_matrix.transform(ip1);
    Point4f p2 = p_matrix.transform(ip2);
    Point4f p3 = p_matrix.transform(ip3);


    Matrix4f t_matrix = Matrix4f::mult(m_matrix, p_matrix);

    p1.normalize();
    p2.normalize();
    p3.normalize();

//    plot_line(buffer,
//             int(p1.X() * smallestHalfDim),
//             int(p1.Y() * smallestHalfDim),
//             int(p1.Z() * 0xFFFF),
//             int((p1.X() + n1_t.X() * normal_factor) * smallestHalfDim),
//             int((p1.Y() + n1_t.Y() * normal_factor) * smallestHalfDim),
//             int((p1.Z() + n1_t.Z() * normal_factor) * 0xFFFF));

//    plot_line(buffer,
//             int(p2.X() * smallestHalfDim),
//             int(p2.Y() * smallestHalfDim),
//             int(p2.Z() * 0xFFFF),
//             int((p2.X() + n2_t.X() * normal_factor) * smallestHalfDim),
//             int((p2.Y() + n2_t.Y() * normal_factor) * smallestHalfDim),
//             int((p2.Z() + n2_t.Z() * normal_factor) * 0xFFFF));

//    plot_line(buffer,
//             int(p3.X() * smallestHalfDim),
//             int(p3.Y() * smallestHalfDim),
//             int(p3.Z() * 0xFFFF),
//             int((p3.X() + n3_t.X() * normal_factor) * smallestHalfDim),
//             int((p3.Y() + n3_t.Y() * normal_factor) * smallestHalfDim),
//             int((p3.Z() + n3_t.Z() * normal_factor) * 0xFFFF));

    draw_filled_triangle(buffer,
                         zBuffer,
                         int(p1.X() * smallestHalfDim),
                         int(p1.Y() * smallestHalfDim),
                         int(p1.Z() * 0xFFFF),
                         ip1.Z(),
                         c1_int,
                         uv1,
                         int(p2.X() * smallestHalfDim),
                         int(p2.Y() * smallestHalfDim),
                         int(p2.Z() * 0xFFFF),
                         ip2.Z(),
                         c2_int,
                         uv2,
                         int(p3.X() * smallestHalfDim),
                         int(p3.Y() * smallestHalfDim),
                         int(p3.Z() * 0xFFFF),
                         ip3.Z(),
                         c3_int,
                         uv3,
                         texture);
}

#endif // RENDER_HPP
