#pragma once
#include <SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "camera.h"
#include "geometry.h"

struct RenderConfig
{
    bool show_grid=true;
    bool simple_grid=false;

    bool gamma_corrention=true;
    float gamma_value=2.2f;

    bool enable_fog=false;
    float fog_start=5.0f;
    float fog_end=20.0f;
    
    vec3 fog_color=vec3(0.53f,0.81f,0.92f);

    vec3 light_dir=vec3(1,1,1);

    float light_intensity=1.0f;
    float ambient=0.2f;

    bool show_fps=true;

};

/**
 * @brief UI 管理器
 * 
 * @details 
 * 负责 ImGui 的生命周期管理和参数调整面板的绘制。
 * 相当于在 CPU 和 GPU 之间搭建一个"控制台"，
 * 用户通过 UI 修改参数 -> 更新 RenderConfig -> Shader 读取配置。
 * 
 * @note 
 * - 设计为静态类，全局唯一实例
 * - 线程不安全（但软光栅是单线程的，所以没问题）
 */
 class UIManger
 {
    public:
    /**
    * @brief 初始化 ImGui
    *
    * @param window SDL窗口
    * @param renderer SDL渲染器(不能和SDL_GetWindowSurface混用
   
    * @return true 成功
    * @return false 失败

    *
    *@details
    * 必须再渲染循环前调用一次
    *会设置ImGui的样式和后端
    
    */
    static bool init(SDL_Window* window,SDL_Renderer* renderer);

    /**

    * @brief 处理 SDL 事件
     * 
     * @param event SDL事件指针
     * 
     * @details 
     * 必须在你的事件循环中，优先调用此函数！

     * 调用顺序：
     * ```cpp
     * while (SDL_PollEvent(&event)) {
     *     UIManager::ProcessEvent(&event);  // 1. 先让UI处理
     *     
     *     if (!UIManager::WantCaptureMouse()) {  // 2. 检查UI是否占用
     *         // 3. 你的鼠标事件处理
     *     }
     * }
     * ```
    
    
    */
    static void ProcessEvent(SDL_Event* event);

    /**
     *@brief 检查ImGui是否正在使用鼠标 
     *
     *@return true ImGui正在使用鼠标,忽略鼠标事件
     *@return false ImGui没有使用鼠标，可以处理旋转和缩放逻辑
    */
    static bool WantCaptureMouse();
    /**
     * @brief 检查 ImGui 是否正在使用键盘
     * 
     * @return true UI正在使用键盘（比如输入文本），应该忽略你的按键事件
     * @return false UI没在用键盘，可以处理你的快捷键
     */

    static bool WantCaptureKeyboard();
    
    /**
     * @brief 绘制参数调整面板（构建UI数据）
     * 
     * @param camera 相机引用（用于显示/修改相机参数）
     * @param config 渲染配置引用（所有Shader参数都在这里）
     * @param model_center 模型中心坐标（用于相机Focus功能）
     * @return true 继续渲染
     * @return false 用户请求退出（可选，看你要不要在UI里加退出按钮）
     * 
     * @details 
     * 调用时机：每一帧在渲染3D内容之后，SDL_RenderPresent之前。
     * 
     * 典型流程：
     * ```cpp
     * // 1. 渲染3D场景到framebuffer
     * Render(framebuffer, ...);
     * 
     * // 2. 把framebuffer复制到SDL纹理
     * copy_to_texture(framebuffer, texture);
     * SDL_RenderCopy(renderer, texture, NULL, NULL);
     * 
     * // 3. 绘制UI（会叠在3D画面上）
     * UIManager::Render(camera, config, model_center);
     * UIManager::Draw(renderer);
     * 
     * // 4. 呈现到屏幕
     * SDL_RenderPresent(renderer);
     * ```
     */
    static bool Render(Camera& camera, RenderConfig& config, const vec3& model_center);
    
    /**
     * @brief 执行ImGui的实际绘制（提交DrawData到GPU）
     * 
     * @param renderer SDL渲染器
     * 
     * @details 
     *  必须在 Render() 之后，SDL_RenderPresent() 之前调用。
     * 这一步才会真正把UI画到屏幕上。
     */
    static void Draw(SDL_Renderer* renderer);
    
    /**
     * @brief 清理资源，销毁ImGui上下文
     * 
     * @details 
     * 在程序退出前调用一次，释放ImGui占用的内存。
     */
    static void Cleanup();



 };