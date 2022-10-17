#include <cstdint>
#include <iostream>

#include "render.hpp"
#include "geometry.hpp"

#include <vector>
#include <algorithm>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

constexpr int Width = 1280;
constexpr int Height = 720;

constexpr int TextureSize = 2048;


constexpr double Fov = 90.0;
constexpr double ZNear = 1.0;
constexpr double ZFar = 100.0;

double mouseRotFactor = 0.01;
double mouseTranslateFactor = 2.0;

int xRot = 0;
int yRot = 0;

int zTrans = 0;

RenderBufferType framebuffer[Width*Height];
ZBufferType zBuffer[screenWidth*screenHeight];

uint32_t texture0[TextureSize*TextureSize];

// forward declarations to avoid reordering the functions
void renderGeometry();
void render();

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture* sdl_texture;

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        return -1;
    }
    else 
    {
        window = SDL_CreateWindow("SDL Output", SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED, Width, Height,
                                    SDL_WINDOW_SHOWN);
        if (window == nullptr) 
        {
            std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
            return -1;
        }
        else 
        {
            renderer = SDL_CreateRenderer(window, -1, 0);
            sdl_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, Width, Height);
        }
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "SDL2_image failed with error: " << IMG_GetError() << "\n";
        return -1;
    }

    SDL_Surface* texture0_surface = IMG_Load("cottage_diffuse.png");

    if(texture0_surface == nullptr)
    {
        std::cout << "SDL2_image couldn't load texture: " << IMG_GetError() << "\n";
        return -1;
    }

    SDL_Surface* texture0_surface_formatted = SDL_ConvertSurfaceFormat(texture0_surface, 
                                                                       SDL_PIXELFORMAT_RGBA8888, 
                                                                       0);

    if(texture0_surface_formatted == nullptr)
    {
        std::cout << "SDL2_image couldn't convert texture: " << IMG_GetError() << "\n";
        return -1;
    }

    SDL_FreeSurface(texture0_surface);

    SDL_LockSurface(texture0_surface_formatted);
    for(int y=0; y<TextureSize; ++y)
    {
        for(int x=0; x<TextureSize; ++x)
        {
            const int pitch_in_4bytes = texture0_surface_formatted->pitch/texture0_surface_formatted->format->BytesPerPixel;
            texture0[(y*TextureSize) + x] = ((uint32_t *)texture0_surface_formatted->pixels)[(y * pitch_in_4bytes) + x];
        }
    }
    SDL_UnlockSurface(texture0_surface_formatted);

    SDL_FreeSurface(texture0_surface_formatted);

    // render once because it might take a while to get an input
    render();

    SDL_Event e;   
    do
    {
        int pending = SDL_PollEvent(&e);

        // if the mouse moved
        if(e.type == SDL_MOUSEMOTION)
        {
            // if the left button was being held
            if (e.motion.state & SDL_BUTTON_LMASK)
            {
                // update the rotations with the 'deltas' of the mouse events
                xRot += e.motion.yrel;
                yRot += e.motion.xrel;
            }
        }

        if(e.type == SDL_MOUSEWHEEL)
        {
            zTrans += e.wheel.y;
        }

        // if we're done processing the event(s) then we can render
        if(!pending)
        {
                render();
        }

    } while (e.type != SDL_QUIT);

    SDL_DestroyTexture(sdl_texture);

    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
    SDL_Quit();
}

void render()
{
    // clear the framebuffer to black
    std::memset(framebuffer, 0x00, Width*Height*4);

    // reset the z-buffer
    for(int i=0; i<screenHeight* screenWidth; ++i)
    {
        zBuffer[i] = std::numeric_limits<int>::max();
    }

    // render into our (non-SDL) framebuffer
    renderGeometry();

    // update/copy the SDL texture
    SDL_UpdateTexture(sdl_texture , NULL, framebuffer, Width * sizeof (uint32_t));
   
    //SDL_RenderClear(renderer);
    // copy the texture into the renderer
    SDL_RenderCopy(renderer, sdl_texture , NULL, NULL);
    // have the renderer actually update the screen
    SDL_RenderPresent(renderer); 

}

void renderGeometry()
{
    // create the perspective matrix
    Matrix4f p;
    p.setToPerspective(Fov, ZNear, ZFar);

    Matrix4f r1;
    r1.xRot(xRot * mouseRotFactor);

    Matrix4f r2;
    r2.yRot(yRot * mouseRotFactor);

    Matrix4f r = Matrix4f::mult(r1, r2);

    Matrix4f m1;
    m1.translate(0.0, 0.0, std::clamp(-30.0 + zTrans * mouseTranslateFactor, -ZFar, -ZNear));

    for(int i=0; i< triangleCount ; ++i)
    {
        auto & indices = geom_vertex_indices[i];
        Point4f p1 {std::get<0>(geom_vertex_list[std::get<0>(indices)]),
                   std::get<1>(geom_vertex_list[std::get<0>(indices)]),
                   std::get<2>(geom_vertex_list[std::get<0>(indices)])};

        Point4f p2 {std::get<0>(geom_vertex_list[std::get<1>(indices)]),
                   std::get<1>(geom_vertex_list[std::get<1>(indices)]),
                   std::get<2>(geom_vertex_list[std::get<1>(indices)])};

        Point4f p3 {std::get<0>(geom_vertex_list[std::get<2>(indices)]),
                   std::get<1>(geom_vertex_list[std::get<2>(indices)]),
                   std::get<2>(geom_vertex_list[std::get<2>(indices)])};

        const int base_normal_index = i * 3;

        Point4f n1 {std::get<0>(geom_normal_list[base_normal_index]),
                   std::get<1>(geom_normal_list[base_normal_index]),
                   std::get<2>(geom_normal_list[base_normal_index])};

        Point4f n2 {std::get<0>(geom_normal_list[base_normal_index+1]),
                   std::get<1>(geom_normal_list[base_normal_index+1]),
                   std::get<2>(geom_normal_list[base_normal_index+1])};

        Point4f n3 {std::get<0>(geom_normal_list[base_normal_index+2]),
                   std::get<1>(geom_normal_list[base_normal_index+2]),
                   std::get<2>(geom_normal_list[base_normal_index+2])};

        Color3i c1 {std::get<0>(geom_vertex_colors[base_normal_index]),
                   std::get<1>(geom_vertex_colors[base_normal_index]),
                   std::get<2>(geom_vertex_colors[base_normal_index])};

        Color3i c2 {std::get<0>(geom_vertex_colors[base_normal_index+1]),
                   std::get<1>(geom_vertex_colors[base_normal_index+1]),
                   std::get<2>(geom_vertex_colors[base_normal_index+1])};

        Color3i c3 {std::get<0>(geom_vertex_colors[base_normal_index+2]),
                   std::get<1>(geom_vertex_colors[base_normal_index+2]),
                   std::get<2>(geom_vertex_colors[base_normal_index+2])};

        Uv2f uv1 {std::get<0>(geom_uv_coords[base_normal_index]),
                 std::get<1>(geom_uv_coords[base_normal_index])};

        Uv2f uv2 {std::get<0>(geom_uv_coords[base_normal_index+1]),
                 std::get<1>(geom_uv_coords[base_normal_index+1])};

        Uv2f uv3 {std::get<0>(geom_uv_coords[base_normal_index+2]),
                 std::get<1>(geom_uv_coords[base_normal_index+2])};

        std::tuple<Point4f, Point4f, Point4f> points { p1, p2, p3 };
        std::tuple<Point4f, Point4f, Point4f> normals { n1, n2, n3 };
        Color3i white = {255, 255, 255};
        //std::tuple<Color3i, Color3i, Color3i> colors { white, white, white };
        std::tuple<Color3i, Color3i, Color3i> colors { c1, c2, c3 };
        std::tuple<Uv2f, Uv2f, Uv2f> uvs { uv1, uv2, uv3 };
        Matrix4f t = Matrix4f::mult(r, m1);
        render_triangles(framebuffer, zBuffer, p, t, points, normals, colors, uvs, texture0);
    }
}