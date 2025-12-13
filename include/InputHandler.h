// include/InputHandler.h
#pragma once
#include <SDL.h>
#include "camera.h"
#include "Light.h"
#include "manager/ModelManager.h"
#include"RenderMode.h"

class InputHandler {
public:
    // 处理键盘事件
    static void handleKeyboard(
        const SDL_Event& event,
        Camera& camera,
        SunLight& sun_light,
        ModelManager& model_manager,
        bool& show_grid,
        bool& simple_grid,
        bool& enable_fog,
        bool& gamma_correction,
        RenderMode& render_mode,
        bool& running,vec3&model_venter
    );
    
    // 处理鼠标事件
    static void handleMouse(
        const SDL_Event& event,
        Camera& camera,
        bool& mouse_left_down,
        bool& mouse_right_down,
        int& last_mouse_x,
        int& last_mouse_y
    );
};