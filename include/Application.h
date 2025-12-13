#pragma once
#include<SDL.h>
#include<memory>
#include"camera.h"
#include"Light.h"
#include"InputHandler.h"
#include"manager/ModelManager.h"

class Application{
    private:
    SDL_Window* window;
    SDL_Surface* screen;
    int width, height;
    bool running;

    ModelManager model_manager;
    Camera camera;
    SunLight sun_light;
    Model* model;
    TGAImage* texture;
    RenderMode render_mode;
    vec3 model_center;
    
    


    bool show_grid;
    bool simple_grid;
    bool enable_fog;
    bool gamma_correction;
    
    bool mouse_left_down;
    bool mouse_right_down;
    int last_mouse_x, last_mouse_y;


    
    Uint32 last_time;
    int frame_count;
    public:
    Application(int width,int height);
    ~Application();
    bool init();
    void run();
    void cleanup();

    private:
    void handleEvents();
    void update(float dt);
    void render();
    void copy_image_to_surface(TGAImage&image,SDL_Surface* surface,int w,int h);

    void handleKeyboard(const SDL_Event&event);
    void handleMouse(const SDL_Event&event);
    void printControls();
    
};