#pragma once
#include "Ishader.h"
#include "model.h"
#include "mat4.h"

class WireframeShader:public IShader{
    private:
    Model* model;
    mat4 mvp;
    TGAColor line_color;
    float line_width;

    public:
    WireframeShader(Model* m,const mat4&_mvp);
    vec4 vertex(int iface,int nthvert)override;
    bool fragment(vec3 bar,TGAColor&color)override;
};