#include"antialiasing.h"
#include<tgaimage.h>
#include<iostream>
#include<random>

SSAA::SSAA(int scale){
    if(scale<1){
        scale=1;
    }
    this->_Scale=scale;
}
int SSAA::getRenderHeight(int height){
    return height*this->_Scale;
}
int SSAA::getRenderWidth(int width){
    return width*this->_Scale;
}

TGAImage SSAA::downSample(TGAImage& image,const int targetWidth,const int targetHeight){
    TGAImage result(targetHeight,targetWidth,3,TGAColor(255,255,255,255));
    for(int x=0;x<targetWidth;x++){
        for(int y=0;y<targetHeight;y++){
            int r_sum=0,g_sum=0,b_sum=0;
            for(int i=0;i<_Scale;i++){
                for(int j=0;j<_Scale;j++){
                    int x1=x*_Scale+i;
                    int y1=y*_Scale+j;
                    TGAColor color=image.get(x1,y1);
                    r_sum+=color.bgra[2];
                    g_sum+=color.bgra[1];
                    b_sum+=color.bgra[0];

                }
            }
            int sample=_Scale*_Scale;
            int r=r_sum/sample;
            int g=g_sum/sample;
            int b=b_sum/sample;
            result.set(x,y,TGAColor(r,g,b,255));
        }
    }
    return result;
}
//MSAA:MSAA的原理是对像素内进行多点采样。计算每个采样点的颜色，然后取平均值作为最终的颜色。

MSAA::MSAA(int samples,SamplePattern pattern):samples_(samples),pattern_(pattern){
    std::cout << "MSAA 初始化: " << samples << "x, 模式: " << pattern << std::endl;
    generateSamplePositions();
}
int MSAA::getSampleCount()const{
    return samples_;
}
std::pair<float,float>MSAA::getSamplePosition(int index)const{
    if(index<0||index>=samples_){
        return {0.5f,0.5f};
        
    }
    return positions_[index];
}
const std::vector<std::pair<float,float>>&MSAA::getAllPositions()const{
    return positions_;
}
void MSAA::regenerate(){
    generateSamplePositions();
}
void MSAA::setPattern(SamplePattern pattern){
    pattern_=pattern;
    generateSamplePositions();
}
float MSAA::halton(int index,int base){
    float result=0.0f;
    float f=1.0f;
    int i=index;
    while(i>0){
        f=f/base;
        result=result+f*(i%base);
        i=i/base;
    }
    return result;
}

void MSAA::generateSamplePositions(){
    positions_.clear();
    switch(pattern_){
        case REGULAR:{
            int grid_size=(int)std::sqrt(samples_);
            if(grid_size*grid_size!=samples_){
                grid_size=2;
            }
            for(int i=0;i<samples_;i++){
                float x=(i%grid_size+0.5f)/static_cast<float>(grid_size);
                float y=(i/static_cast<float>(grid_size)+0.5f)/grid_size;
                positions_.push_back({x,y});
            }
            std::cout<<"MSAA: 使用规则采样模式";
            break;
        }
        //旋转网格，减少采样点的规律性
        case ROTATED:{
            int grid_size=(int)std::sqrt(samples_);
            if(grid_size*grid_size!=samples_){
                grid_size=2;
            }
            float angle=26.565f*3.14159f/180.0f;

            for(int i=0;i<samples_;i++){
                float x=(i%grid_size+0.5f)/static_cast<float>(grid_size)*-0.5f;
                float y=(i/static_cast<float>(grid_size)+0.5f)/grid_size*0.5f-0.5f;

                float rx=x*std::cos(angle)-y*std::sin(angle)+0.5f;
                float ry=x*std::sin(angle)+y*std::cos(angle)+0.5f;

                positions_.push_back({rx,ry});
            }
            std::cout<<"生成旋转网格";
            break;
        }
        case HALTON:{
            for(int i=0;i<samples_;i++){
                float x=halton(i,2);
                float y=halton(i,3);
                positions_.push_back({x,y});

            }
            break;
            
        }
        case RANDOM:{
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float>dis(0.0f,1.0f);
            for(int i=0;i<samples_;i++){
                positions_.push_back({dis(gen),dis(gen)});}
            break;


        }

    }
    std::cout<<"采样点:";
    for(int i=0;i<std::min(4,(int)positions_.size());i++){
        std::cout<<positions_[i].first<<","<<positions_[i].second<<" ";
        
    }
    std::cout<<std::endl;
}
