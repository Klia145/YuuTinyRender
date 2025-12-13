#include "Application.h"
#include "renderer.h"
#include "colorTable.h"
#include "constants.h"
#include "DefaultShader.h"
#include "shader/BlinnPhongShader.h"
#include "shader/WireframeShader.h"
#include <iostream>


Application::Application(int w, int h)
    :
    window(nullptr)
    , screen(nullptr)
    , width(w)
    , height(h)
    , running(false)
    , camera(vec3(0,0,3), vec3(0,0,0), Fov, aspect, Near, Far)
    , sun_light(vec3(1,1,1), vec3(1,1,1), 1.0f)
    , render_mode(RenderMode::DEFAULT)
    ,model_center(0, 0, 0)
    
    // 显示选项
    , show_grid(true)
    , simple_grid(false)
    , enable_fog(false)         
    , gamma_correction(false)   
    
    
    , mouse_left_down(false)    
    , mouse_right_down(false)   
    , last_mouse_x(0)           
    , last_mouse_y(0)           
    
    // 性能统计
    ,last_time(0)
    , frame_count(0)
{
    sun_light.setTimeOfDay(14.0f);
}
Application::~Application(){
    cleanup();
}

bool Application::init(){
    if(SDL_Init(SDL_INIT_VIDEO)<0){
        std::cerr<<"SDL_Init Error: "<<SDL_GetError()<<std::endl;
        return false;
    }
    window = SDL_CreateWindow(
        "Software Rasterizer - Yuu's Interactive Viewer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_SHOWN
    );
    if(!window){
        std::cerr<<"SDL_CreateWindow Error: "<<SDL_GetError()<<std::endl;
        return false;
    }
    screen=SDL_GetWindowSurface(window);
    std::cout<<"加载模型"<<std::endl;
    model_manager.switchTo(0);
    model=model_manager.getCurrentModel();
    texture=model_manager.getCurrentTexture();
    render_mode=RenderMode::DEFAULT;
    printControls();
    last_time = SDL_GetTicks();
    running = true;
    model_center=model_manager.getCurrentModelCenter();
    camera.focusPreset(ViewPreset::DEFAULT, model_center, 8.0f); 
    enable_fog=false;
    gamma_correction=false;
    return true;
}
void Application::printControls() {
    std::cout << "==================================" << std::endl;
    std::cout << "控制说明:" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "鼠标控制:" << std::endl;
    std::cout << "  左键拖拽: 旋转视角" << std::endl;
    std::cout << "  右键拖拽: 平移相机" << std::endl;
    std::cout << "  滚轮: 缩放" << std::endl;
    std::cout << "---" << std::endl;
    std::cout << "键盘控制:" << std::endl;
    std::cout << "  P: 切换投影模式 (透视/正交)" << std::endl;
    std::cout << "  Shift+1/2/3: 切换模型" << std::endl;
    std::cout << "  Ctrl+Z: 切换渲染模式" << std::endl;
    std::cout << "  F: 默认视角" << std::endl;
    std::cout << "  G: 切换网格显示" << std::endl;
    std::cout << "  Ctrl+G: 切换简单网格" << std::endl;
    std::cout << "  U: 切换雾效" << std::endl;
    std::cout << "  Q: 切换伽马校正" << std::endl;
    std::cout << "---" << std::endl;
    std::cout << "小键盘:" << std::endl;
    std::cout << "  小键盘.: 聚焦模型" << std::endl;
    std::cout << "  小键盘1: 前视图 (Ctrl+1: 后视图)" << std::endl;
    std::cout << "  小键盘3: 右视图 (Ctrl+3: 左视图)" << std::endl;
    std::cout << "  小键盘7: 顶视图 (Ctrl+7: 底视图)" << std::endl;
    std::cout << "---" << std::endl;
    std::cout << "光照控制:" << std::endl;
    std::cout << "  T: 推进时间 (+1小时)" << std::endl;
    std::cout << "  R: 切换太阳自动旋转" << std::endl;
    std::cout << "---" << std::endl;
    std::cout << "  ESC: 退出" << std::endl;
    std::cout << "==================================" << std::endl;
}
void Application::run() {
    while (running) {
        handleEvents();
        
        float dt = 0.016f;  // 假设 60 FPS
        update(dt);
        render();
        
        // FPS 统计
        frame_count++;
        Uint32 current_time = SDL_GetTicks();
        if (current_time - last_time >= 1000) {
            std::cout << "FPS: " << frame_count << std::endl;
            frame_count = 0;
            last_time = current_time;
        }
    }
}
void Application::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        else if (event.type == SDL_KEYDOWN) {
            handleKeyboard(event);
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN || 
                 event.type == SDL_MOUSEBUTTONUP ||
                 event.type == SDL_MOUSEMOTION ||
                 event.type == SDL_MOUSEWHEEL) {
            handleMouse(event);
        }
    }
}
void Application::update(float dt) {
    if (sun_light.auto_rotate) {
        sun_light.update(dt);
    }
}
void Application::render(){
    TGAImage framebuffer(width,height,3,ColorTable::getColor(ColorName::BLACK));
    std::vector<float> zbuffer(width * height, -1e9);
    
    framebuffer.clear();
    render_skyBox(framebuffer);
    
    // 获取矩阵
    mat4 view = camera.getViewMatrix();
    mat4 projection = camera.getProjectionMatrix();
    mat4 mvp = camera.getViewProjectionMatrix();
    if (show_grid) {
        if (simple_grid) {
            DrawGrid(framebuffer, mvp, camera.getPosition(), TGAColor(100,100,100,255));
        } else {
            DrawInfiniteGrid(framebuffer, view, projection, camera.getPosition());
        }
    }
    
    std::unique_ptr<IShader> shader;
    switch (render_mode) {
        case RenderMode::DEFAULT:
            shader = std::make_unique<DefaultShader>(
                model, texture, mvp, sun_light, camera.getPosition(), this->enable_fog
            );
            break;
        case RenderMode::BLINN_PHONG:
            shader = std::make_unique<BlinnPhongShader>(
                model, texture, mvp, sun_light, camera.getPosition()
            );
            break;
        case RenderMode::WIREFRAME:
            shader = std::make_unique<WireframeShader>(model, mvp);
            break;
        case RenderMode::COUNT: // 不应该到达这里
            break;    
    }
    if (shader && model) {
        RenderWithShader(framebuffer, *model, zbuffer, shader.get());
    }
    
    if (gamma_correction) {
        applyGammaCorrection(framebuffer);
    }
    
    copy_image_to_surface(framebuffer, screen, width, height);
    SDL_UpdateWindowSurface(window);
}
void Application::cleanup() {
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}
void Application::copy_image_to_surface(TGAImage&image,SDL_Surface* surface,int w,int h){
    SDL_LockSurface(surface);

    for(int y=0;y<h;y++){
        for(int x=0;x<w;x++){
            TGAColor color=image.get(x,y);
            Uint32* pixels=(Uint32*)surface->pixels;
            pixels[y*w+x]=SDL_MapRGB(
                surface->format,
                color.bgra[2],
                color.bgra[1],
                color.bgra[0]
            );

        }
    }
    SDL_UnlockSurface(surface);

};
void Application::handleKeyboard(const SDL_Event& event) {
    size_t old_index = model_manager.getCurrentIndex();
    InputHandler::handleKeyboard(
        event,
        camera,
        sun_light,
        model_manager,
        show_grid,
        simple_grid,
        enable_fog,
        gamma_correction,
        render_mode,
        running,model_center

    );
    if (model_manager.getCurrentIndex() != old_index) {
        model = model_manager.getCurrentModel();
        texture = model_manager.getCurrentTexture();
        std::cout << "✓ 模型指针已更新" << std::endl;
    }
}

void Application::handleMouse(const SDL_Event& event) {
    InputHandler::handleMouse(
        event,
        camera,
        mouse_left_down,
        mouse_right_down,
        last_mouse_x,
        last_mouse_y
    );
}