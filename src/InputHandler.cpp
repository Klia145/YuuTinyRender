// src/InputHandler.cpp
#include "InputHandler.h"
#include "constants.h"
#include <iostream>
#include"RenderMode.h"
void InputHandler::handleKeyboard(
    const SDL_Event& event,
    Camera& camera,
    SunLight& sun_light,
    ModelManager& model_manager,
    bool& show_grid,
    bool& simple_grid,
    bool& enable_fog,
    bool& gamma_correction,
    RenderMode& render_mode,
    bool& running,vec3&model_center
) {    
    bool ctrl_pressed = (SDL_GetModState() & KMOD_CTRL);
    bool shift_pressed = (SDL_GetModState() & KMOD_SHIFT);
\
    switch (event.key.keysym.sym) {
        // === 退出 ===
        case SDLK_ESCAPE:
            running = false;
            break;
        
        // === 相机控制 ===
        case SDLK_p:
            camera.toggleProjectionMode();
            std::cout << "切换投影模式" << std::endl;
            break;
        
        case SDLK_f:
            camera.focusPreset(ViewPreset::DEFAULT, model_center, 3.0f);
            std::cout << "恢复默认视角" << std::endl;
            break;
        
        case SDLK_KP_PERIOD:
            camera.focusOn(model_center, CameraDistance);
            std::cout << "聚焦模型中心" << std::endl;
            break;
        
        // 小键盘视图
        case SDLK_KP_1:
            camera.focusPreset(
                ctrl_pressed ? ViewPreset::BACK : ViewPreset::FRONT,
                model_center, 3.0f
            );
            break;
        
        case SDLK_KP_3:
            camera.focusPreset(
                ctrl_pressed ? ViewPreset::LEFT : ViewPreset::RIGHT,
                model_center, 3.0f
            );
            break;
        
        case SDLK_KP_7:
            camera.focusPreset(
                ctrl_pressed ? ViewPreset::BOTTOM : ViewPreset::TOP,
                model_center, 3.0f
            );
            break;
        
        // === 渲染模式 ===
        case SDLK_z:

            if (ctrl_pressed) {
                int current = static_cast<int>(render_mode);
                int next = (current + 1) % static_cast<int>(RenderMode::COUNT);
                render_mode = static_cast<RenderMode>(next);
                std::cout << "渲染模式: " << getRenderModeName(render_mode) << std::endl;
            }
            break;
        
        // === 显示选项 ===
        case SDLK_g:
            if (ctrl_pressed) {
                simple_grid = !simple_grid;
                std::cout << "简单网格: " << (simple_grid ? "开" : "关") << std::endl;
            } else {
                show_grid = !show_grid;
                std::cout << "网格: " << (show_grid ? "开" : "关") << std::endl;
            }
            break;
        
        case SDLK_u:
            enable_fog = !enable_fog;
            std::cout << "雾效: " << (enable_fog ? "开" : "关") << std::endl;
            break;
        
        case SDLK_q:
            gamma_correction = !gamma_correction;
            std::cout << "伽马校正: " << (gamma_correction ? "开" : "关") << std::endl;
            break;
        
        // === 光照控制 ===
        case SDLK_t:
            {
                float current_time = sun_light.getTimeOfDay();
                sun_light.setTimeOfDay(current_time + 1.0f);
                std::cout << "时间: " << sun_light.getTimeOfDay() << ":00" << std::endl;
            }
            break;
        
        case SDLK_r:
            sun_light.auto_rotate = !sun_light.auto_rotate;
            std::cout << "太阳自动旋转: " << (sun_light.auto_rotate ? "开" : "关") << std::endl;
            break;
        
        // === 模型切换 ===
        default:
            if (shift_pressed && event.key.keysym.sym >= SDLK_1 && event.key.keysym.sym <= SDLK_9) {
                int index = event.key.keysym.sym - SDLK_1;
                if (index < (int)model_manager.getModelCount()) {
                    model_manager.switchTo(index);
                    std::cout << "切换到模型 " << (index + 1) << std::endl;
                } else {
                    std::cout << "没有模型 " << (index + 1) << std::endl;
                }
            }
            break;
    }
}

void InputHandler::handleMouse(
    const SDL_Event& event,
    Camera& camera,
    bool& mouse_left_down,
    bool& mouse_right_down,
    int& last_mouse_x,
    int& last_mouse_y
) {
    bool alt_pressed = (SDL_GetModState() & KMOD_ALT);
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                mouse_left_down = true;
                last_mouse_x = event.button.x;
                last_mouse_y = event.button.y;
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                mouse_right_down = true;
                last_mouse_x = event.button.x;
                last_mouse_y = event.button.y;
            }
            break;
        
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                mouse_left_down = false;
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                mouse_right_down = false;
            }
            break;
        
        case SDL_MOUSEMOTION:
            {
                int current_x = event.motion.x;
                int current_y = event.motion.y;
                int delta_x = current_x - last_mouse_x;
                int delta_y = current_y - last_mouse_y;
                bool should_rotate=false;

                
                if(alt_pressed&&mouse_left_down){
                    camera.setCameraMode(CameraMode::ORBIT);
                    should_rotate=true;
                }else if (mouse_left_down) {
                    camera.setCameraMode(CameraMode::FREE);
                    should_rotate = true;
                }
                if(should_rotate)
                {
                    camera.rotate(delta_x*Config::MouseSensitivity, 
                                  delta_y*Config::MouseSensitivity
                    );
                     static int count = 0;
    if (++count % 20 == 0) {  // 每 20 次打印一次
        vec3 cam_pos = camera.getPosition();
        vec3 cam_target = camera.getTarget();
        
        std::cout << "=== 轨道旋转 ===" << std::endl;
        std::cout << "相机位置: (" << cam_pos.x << ", " << cam_pos.y << ", " << cam_pos.z << ")" << std::endl;
        std::cout << "相机目标: (" << cam_target.x << ", " << cam_target.y << ", " << cam_target.z << ")" << std::endl;
    }
                }
                if (mouse_right_down) {
                    camera.pan(delta_x * Config::MouseSensitivity, 
                               delta_y * Config::MouseSensitivity);
                }
                
                last_mouse_x = current_x;
                last_mouse_y = current_y;
            }
            break;
        
        case SDL_MOUSEWHEEL:
            {
                float zoom_delta = event.wheel.y * Config::ZOOM_SENSITIVITY;
                camera.zoom(zoom_delta);
            }
            break;
    }
}