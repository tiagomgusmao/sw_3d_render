#ifndef RENDER_MATH_HPP
#define RENDER_MATH_HPP

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <array>
#include <tuple>
#include <cassert>

class Uv2f
{
public:
    Uv2f(double u, double v)
    {
        uv[0] = u;
        uv[1] = v;
    }

    double U()
    {
        return uv[0];
    }

    double V()
    {
        return uv[1];
    }
private:
    double uv[2];
};

class Color3i
{
public:
    Color3i(int r, int g, int b)
    {
        colors[0] = r;
        colors[1] = g;
        colors[2] = b;
    }

    double R()
    {
        return colors[0];
    }

    double G()
    {
        return colors[1];
    }

    double B()
    {
        return colors[2];
    }
private:
    int colors[3];
};

// forward declaration so Point4f can be friend of Matrix4f
class Matrix4f;

class Point4f
{
public:
    friend Matrix4f;
    static constexpr int N=4;

    Point4f()
    {}

    Point4f(double x, double y, double z, double w=1.0)
    {
        m[0] = x;
        m[1] = y;
        m[2] = z;
        m[3] = w;
    }

    void set(double x, double y, double z, double w=1.0)
    {
        m[0] = x;
        m[1] = y;
        m[2] = z;
        m[3] = w;
    }

    double X()
    {
        return m[0];
    }

    double Y()
    {
        return m[1];
    }

    double Z()
    {
        return m[2];
    }

    double dotProduct3(Point4f &p)
    {
        return m[0] * p.m[0] + m[1] * p.m[1] + m[2] * p.m[2];
    }

    void normalize()
    {
        if(m[3] > 0.001)
        {
            m[0] /= m[3];
            m[1] /= m[3];
            m[2] /= m[3];
            m[3] = 1.0;
        }
        else
        {
            double length = sqrt((m[0] * m[0]) + (m[1] * m[1]));
            if(length>0.001)
            {
            m[0] /= length/2;
            m[1] /= length/2;
            }
            else
            {
                assert(false);
            }
            m[2] = -1.0;
            m[3] = 1.0;
        }
    }

private:
    double m[4];
};

class Matrix4f
{
public:
    static constexpr int N=4;

    Matrix4f()
    {}

    Matrix4f(Matrix4f & o)
    {
        std::memcpy(m, o.m, sizeof(m));
    }

    Matrix4f(std::initializer_list<double> values)
    {
        assert(values.size() == N*N);
        int i=0;
        for(auto &v :values)
        {
            m[i++] = v;
        }
    }

    void set(std::initializer_list<double> values)
    {
        assert(values.size() == N*N);
        int i=0;
        for(auto &v :values)
        {
            m[i++] = v;
        }
    }

    void removeTranslationAndScale()
    {
        m[4*0+3] = 0.0;
        m[4*1+3] = 0.0;
        m[4*2+3] = 0.0;

        m[4*3] = 0.0;
        m[4*3+1] = 0.0;
        m[4*3+2] = 0.0;
    }

    static Matrix4f mult(Matrix4f &m1, Matrix4f &m2)
    {
        Matrix4f res;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                double num = 0;
                for (int k = 0; k < N; k++) {
                    num += m1.m[i + k * N] * m2.m[k + j * N];
                }
                res.m[i +j * N] = num;
            }
        }
        return res;
    }

    void xRot(double angle)
    {
        set({1, 0, 0, 0,
            0, std::cos(angle), -std::sin(angle), 0,
            0, std::sin(angle), std::cos(angle), 0,
            0, 0, 0, 1});
    }

    void yRot(double angle)
    {
         set({cos(angle), 0,  sin(angle), 0,
              0, 1, 0, 0,
              -sin(angle), 0, cos(angle), 0,
              0, 0, 0, 1});
    }

    void translate(double x, double y, double z)
    {
        set({1.0, 0.0, 0.0, x,
             0.0, 1.0, 0.0, y,
             0.0, 0.0, 1.0, z,
             0.0, 0.0, 0.0, 1.0,});
    }

    void setToIdentity()
    {
        set({1.0, 0.0, 0.0, 0.0,
             0.0, 1.0, 0.0, 0.0,
             0.0, 0.0, 1.0, 0.0,
             0.0, 0.0, 0.0, 1.0,});
    }

    Point4f transform(Point4f p)
    {
        Point4f res;

        // i - iterates target point index and matrix row
        for (int i = 0; i < N; i++)
        {
            double num = 0.0;
            // iterates input point index and matrix column
            for (int j = 0; j < N; j++)
            {
                num += m[j + i * N] * p.m[j];
            }
            res.m[i] = num;
        }

        return res;
    }

    void setToPerspective(double fov, double near, double far)
    {
        zNear = near;
        double scale = (1 / std::tan(fov * 0.5 * (M_PI / 180)));

        double m_2_2 = -far / (far - near); // used to remap z to [0,1]
        double m_3_2 = -(far * near) / (far - near);  // used to remap z [0,1]


        set({scale, 0.0, 0.0 ,0.0,
             0.0, scale, 0.0, 0.0,
             0.0, 0.0, m_2_2, -1.0,
             0.0, 0.0, m_3_2, 0.0});
    }

    double getZNear()
    {
        return zNear;
    }

private:
    double m[N*N];
    double zNear = 0.0;
};

#endif // RENDER_MATH_HPP
