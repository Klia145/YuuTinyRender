#pragma once
#include"Camera.h"
#include"Light.h"
#include"Application.h"
#include"manager/ModelManager.h"
class UIManager{
    private:
    int fps;
    public:
    UIManager();
    ~UIManager();
    void init();
    void render(int fps,RenderMode rendermode,const Camera&camera,const SunLight&sun_light,const ModelManager&model_manager);
    void cleanup();
};