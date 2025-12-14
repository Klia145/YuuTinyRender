#include "Application.h"
#include "renderer.h"
#include "colorTable.h"
#include "constants.h"
#include "DefaultShader.h"
#include "shader/BlinnPhongShader.h"
#include "shader/WireframeShader.h"
#include <iostream>
#include<SDL.h>
#include "../Imgui/imgui.h"
#include "../imgui/backends/imgui_impl_sdl2.h"
#include "../imgui/backends/imgui_impl_sdlrenderer2.h"

Application::Application(int w, int h)
    :
    window(nullptr)
    ,renderer(nullptr),framebuffer_texture(nullptr)
    , width(w)
    , height(h)
    , running(false)
    , camera(vec3(0,0,3), vec3(0,0,0), Fov, aspect, Near, Far)
    , sun_light(vec3(1,1,1), vec3(1,1,1), 1.0f)
    , render_mode(RenderMode::DEFAULT)
    ,model_center(0, 0, 0)
    
    , show_grid(true)
    , simple_grid(false)
    , enable_fog(false)         
    , gamma_correction(false)   
    
    
    , mouse_left_down(false)    
    , mouse_right_down(false)   
    , last_mouse_x(0)           
    , last_mouse_y(0)           
    
    ,last_time(0)
    , frame_count(0),current_fps(0)
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
    bool UI_Init_True_Or_False=UI_Init();
    if(!UI_Init_True_Or_False){
        return false;
    }
    Init_Property();
    return true;
}
bool Application::UI_Init(){

    renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

    if(!renderer){
        std::cerr<<"创建Renderer失败"<<SDL_GetError()<<std::endl;
        SDL_Quit();
        return false;
    }
    framebuffer_texture=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_STREAMING,width,height);
    if (!framebuffer_texture) {
        std::cerr << " 创建纹理失败: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/msyh.ttc", 18.0f, nullptr, 
                                  io.Fonts->GetGlyphRangesChineseFull());
    if (io.Fonts->Fonts.Size == 0) {
        io.Fonts->AddFontDefault();
        std::cout << " 中文字体加载失败，使用默认字体" << std::endl;
    }
    
    ImGui::StyleColorsDark();     
    if (!ImGui_ImplSDL2_InitForSDLRenderer(window, renderer)) {
        std::cerr << " ImGui SDL2 初始化失败" << std::endl;
        return false;
    }
    
    if (!ImGui_ImplSDLRenderer2_Init(renderer)) {
        std::cerr << " ImGui Renderer 初始化失败" << std::endl;
        ImGui_ImplSDL2_Shutdown();
        return false;
    }
        
    std::cout << " ImGui 初始化成功" << std::endl;
    return true;
}
void Application::Init_Property(){
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
        
        float dt = 0.016f; 
        update(dt);
        render();
        
        frame_count++;
        Uint32 current_time = SDL_GetTicks();
        if (current_time - last_time >= 1000) {
            current_fps = frame_count;
            std::cout << "FPS: " << frame_count << std::endl;
            frame_count = 0;
            last_time = current_time;
        }
    }
}
void Application::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {

        ImGui_ImplSDL2_ProcessEvent(&event);
        ImGuiIO& io = ImGui::GetIO();
        if (event.type == SDL_QUIT) {
            running = false;
        }
        else if (event.type == SDL_KEYDOWN) {
            if (!io.WantCaptureKeyboard) {
                handleKeyboard(event);
            }
        }
        
        else if (event.type == SDL_MOUSEBUTTONDOWN || 
                 event.type == SDL_MOUSEBUTTONUP ||
                 event.type == SDL_MOUSEMOTION ||
                 event.type == SDL_MOUSEWHEEL) {
            if (!io.WantCaptureMouse) {
                handleMouse(event);
            }
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
        case RenderMode::COUNT:
            break;    
    }
    if (shader && model) {
        RenderWithShader(framebuffer, *model, zbuffer, shader.get());
    }
    
    if (gamma_correction) {
        applyGammaCorrection(framebuffer);
    }
    uploadFramebufferToTexture(framebuffer);
    
    SDL_RenderClear(renderer);
    
    // 显示 3D 场景
    SDL_RenderCopy(renderer, framebuffer_texture, nullptr, nullptr);
    
    // 渲染 UI
    renderUI();
    
    // 显示
    SDL_RenderPresent(renderer);
    /*
    copy_image_to_surface(framebuffer,screen,width,height);
    SDL_UpdateWindowSurface(window);
    */
    
}
void Application::cleanup() {
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    if (framebuffer_texture) {
        SDL_DestroyTexture(framebuffer_texture);
        framebuffer_texture = nullptr;
    }
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}
/*
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
*/
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
        std::cout << "模型指针已更新" << std::endl;
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

void Application::uploadFramebufferToTexture(const TGAImage&framebuffer){

    void* pixels;
    int pitch;
    SDL_LockTexture(framebuffer_texture,nullptr,&pixels,&pitch);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            TGAColor color = framebuffer.get(x, y);
            Uint32* target = (Uint32*)((Uint8*)pixels + y * pitch);
            target[x] = (color.bgra[2] << 16) | (color.bgra[1] << 8) | color.bgra[0];
        }
    }
    SDL_UnlockTexture(framebuffer_texture);
}
void Application::renderUI() {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    
    // === 主信息窗口 ===
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 450), ImGuiCond_FirstUseEver);

    
    ImGui::Begin(" Yuu's Rasterizer", nullptr);
    
    ImGui::SeparatorText(" 性能");
    if (current_fps >= 50) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "FPS: %d", current_fps);
    } else if (current_fps >= 30) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "FPS: %d", current_fps);
    } else {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "FPS: %d", current_fps);
    }
    float fps_ratio = std::min(current_fps / 60.0f, 1.0f);
    ImGui::ProgressBar(fps_ratio, ImVec2(-1, 0));
    
    ImGui::Spacing();
    
    // 相机
    ImGui::SeparatorText("相机");
    vec3 pos = camera.getPosition();
    ImGui::Text("位置: (%.1f, %.1f, %.1f)", pos.x, pos.y, pos.z);
    ImGui::Text("距离: %.2f", camera.getDistance());
    
    ImGui::Spacing();
    
    // 模型
    ImGui::SeparatorText(" 模型");
    if (model) {
        ImGui::Text("顶点: %d", model->nverts());
        ImGui::Text("面数: %d", model->nfaces());
    }
    
    ImGui::Spacing();
    
    // 渲染状态
    ImGui::SeparatorText(" 渲染");
    ImGui::Text("模式: %s", getRenderModeName(render_mode));
    ImGui::BulletText("网格: %s", show_grid ? "坐标系网格" : "简单网格");
    ImGui::BulletText("雾效: %s", enable_fog ? "open" : "close");
    ImGui::BulletText("伽马: %s", gamma_correction ? "open" : "close");
    
    ImGui::End();
    ImGui::SetNextWindowPos(
        ImVec2(width-300,20),ImGuiCond_FirstUseEver
    );
    ImGui::SetNextWindowSize(
    ImVec2(280, 200),                // 窗口大小
    ImGuiCond_FirstUseEver
);
    ImGui::Begin("RenderMode Settings",nullptr,ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize);
    ImGui::Text("Render Mode:");
    ImGui::Separator();  // 分隔线
    if (ImGui::RadioButton("Wireframe", render_mode == RenderMode::WIREFRAME)) {
        render_mode = RenderMode::WIREFRAME;
    }
    
    if (ImGui::RadioButton("Blinn-Phong", render_mode == RenderMode::BLINN_PHONG)) {
        render_mode = RenderMode::BLINN_PHONG;
    }
    
    if (ImGui::RadioButton("Default (Phong)", render_mode == RenderMode::DEFAULT)) {
        render_mode = RenderMode::DEFAULT;
    }

    ImGui::End();
    
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
}