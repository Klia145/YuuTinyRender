#pragma once
#include<tgaimage.h>

class SSAA{
    private:
    int _Scale;
    public:
    SSAA(int scale);
    TGAImage downSample(TGAImage&Highimage,const int targetWidth,const int targetHeight);
    int getRenderWidth(int width);
    int getRenderHeight(int height);


};
class MSAA{
    public:
    enum SamplePattern{
        REGULAR,
        ROTATED,
        RANDOM,
        HALTON
    };
    private:
    int samples_;
    SamplePattern pattern_;
    std::vector<std::pair<float,float>>positions_;

    void generateSamplePositions();
    float halton(int index,int base);
    public:
    MSAA(int samples=4,SamplePattern pattern=REGULAR);

    int getSampleCount()const;

    //得到第I个采样点
    std::pair<float,float>getSamplePosition(int index)const;

    const std::vector<std::pair<float,float>>&getAllPositions()const;
    //用于准随机
    void regenerate();
    void setPattern(SamplePattern pattern);
};