#include "TextureSampler.h"
#include<algorithm>
#include<cmath>
#include"utils.h"

TextureSampler::TextureSampler(const TGAImage* tex,SamplingMode sampling_mode):texture(tex),mode(sampling_mode){

}
void TextureSampler::setMode(SamplingMode sampling_mode){
    mode=sampling_mode;


}
SamplingMode TextureSampler::getMode()const{
    return mode;
}
void TextureSampler::setTexture(const TGAImage* tex) {
    texture = tex;
}
TGAColor TextureSampler::sample(vec2 uv)const{
    switch (mode) {
        case SamplingMode::NEAREST:
            return sample_nearest(uv);
        
        case SamplingMode::BILINEAR:
            return sample_bilinear(uv);
        
        case SamplingMode::TRILINEAR:
            return sample_bilinear(uv); 
        
        default:
            return sample_nearest(uv);
    }
}
TGAColor TextureSampler::sample(float u,float v)const{
    return sample(vec2(u,v));
}
TGAColor TextureSampler::sample_nearest(vec2 uv)const{
    uv.u = std::max(0.0f, std::min(uv.u, 0.9999f));
    uv.v = std::max(0.0f, std::min(uv.v, 0.9999f));
    
    int tex_x = (int)(uv.u * texture->width());
    int tex_y = (int)(uv.v * texture->height());
    
    tex_x = std::max(0, std::min(tex_x, texture->width() - 1));
    tex_y = std::max(0, std::min(tex_y, texture->height() - 1));
    
    return texture->get(tex_x, tex_y);
}
TGAColor TextureSampler::sample_bilinear(vec2 uv)const{
    
    float fx = uv.u * texture->width() - 0.5f;  
    float fy = uv.v * texture->height() - 0.5f;
    

    int x0 = (int)std::floor(fx);
    int y0 = (int)std::floor(fy);
    
    float dx = fx - x0;  
    float dy = fy - y0;  
    
    // === 第3步：计算4个采样点的坐标 ===
    int x1 = x0 + 1;
    int y1 = y0 + 1;
    
    // 边界限制（Clamp模式）
    x0 = std::max(0, std::min(x0, texture->width() - 1));
    x1 = std::max(0, std::min(x1, texture->width() - 1));
    y0 = std::max(0, std::min(y0, texture->height() - 1));
    y1 = std::max(0, std::min(y1, texture->height() - 1));
    
    TGAColor c00 = texture->get(x0, y0);  // 左上
    TGAColor c10 = texture->get(x1, y0);  // 右上
    TGAColor c01 = texture->get(x0, y1);  // 左下
    TGAColor c11 = texture->get(x1, y1);  // 右下
    TGAColor result;
    for (int i = 0; i < 4; i++) {  // BGRA 4个通道

        unsigned char top = lerp_byte(c00.bgra[i], c10.bgra[i], dx);
        unsigned char bottom = lerp_byte(c01.bgra[i], c11.bgra[i], dx);
        
        result.bgra[i] = lerp_byte(top, bottom, dy);
    }
    
    return result;
}
