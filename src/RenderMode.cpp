// src/RenderMode.cpp
#include "RenderMode.h"

const char* getRenderModeName(RenderMode mode) {
    switch(mode) {
        case RenderMode::DEFAULT:     return "默认模式";
        case RenderMode::BLINN_PHONG: return "Blinn-Phong";
        case RenderMode::WIREFRAME:   return "线框模式";
        default:                      return "未知模式";
    }
}