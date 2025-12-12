 #include "shader/WireframeShader.h"
 WireframeShader::WireframeShader(Model* m,const mat4&_mvp):model(m),mvp(_mvp),line_color(255,255,255,255)
 ,line_width(0.02f){
 }
 vec4 WireframeShader::vertex(int iface,int nthvert){
    vec3 pos=model->vert(iface,nthvert);
    return mvp*vec4(pos,1.0f);
 }
 bool WireframeShader::fragment(vec3 bar,TGAColor&color){
    if(bar.x<line_width||bar.y<line_width||bar.z<line_width){
        color=line_color;
        return false;
    }
    else{
        return true;
    }
 }