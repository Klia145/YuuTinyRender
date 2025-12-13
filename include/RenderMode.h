// include/RenderMode.h
#pragma once

enum class RenderMode {
    DEFAULT, 
    BLINN_PHONG, 
    WIREFRAME, 
    COUNT
};

const char* getRenderModeName(RenderMode mode);