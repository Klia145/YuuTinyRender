#pragma once
#include"tgaimage.h"
#include<unordered_map>

enum class ColorName{
    BLACK,
    WHITE,
    RED,
    GREEN,
    BLUE,
    YELLOW,CYAN,MAGENTA,
};
class ColorTable{
    private:
    static std::unordered_map<ColorName,TGAColor>colors;
    public:
    static const TGAColor&getColor(ColorName name){
        return colors[name];
    }
    

};