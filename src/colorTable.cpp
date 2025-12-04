#include "colorTable.h"

std::unordered_map<ColorName, TGAColor> ColorTable::colors = {
    {ColorName::BLACK,  TGAColor(0, 0, 0, 255)},
    {ColorName::WHITE,  TGAColor(255, 255, 255, 255)},
    {ColorName::RED,    TGAColor(255, 0, 0, 255)},
    {ColorName::GREEN,  TGAColor(0, 255, 0, 255)},
    {ColorName::BLUE,   TGAColor(0, 0, 255, 255)},
    {ColorName::YELLOW, TGAColor(255, 255, 0, 255)},
    {ColorName::CYAN,   TGAColor(0, 255, 255, 255)},
    {ColorName::MAGENTA,TGAColor(255, 0, 255, 255)}
};