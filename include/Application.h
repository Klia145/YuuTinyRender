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

    SDL_Renderer* renderer;
    SDL_Texture* framebuffer_texture;
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
    int current_fps;
    public:
    Application(int width,int height);
    ~Application();
    bool init();
    bool UI_Init();
    void Init_Property();
    void run();
    void cleanup();

    private:
    void handleEvents();
    void update(float dt);
    void render();

    void uploadFramebufferToTexture(const TGAImage&image);
    void renderUI();

    void handleKeyboard(const SDL_Event&event);
    void handleMouse(const SDL_Event&event);
    void printControls();

    
};