// sun_light.h
#pragma once
#include "vec3.h"

class SunLight {
private:
    vec3 direction;      
    vec3 color;         
    float intensity;    
    float time_of_day;    

public:
    bool auto_rotate;   
    SunLight();
    SunLight(const vec3& dir, const vec3& col = vec3(1, 1, 1), float inten = 1.0f);
    ~SunLight();
    

    vec3 getDirection() const { return direction; }
    vec3 getColor() const { return color; }
    float getIntensity() const { return intensity; }
    
    void setDirection(const vec3& dir);
    void setColor(const vec3& col);
    void setIntensity(float inten);
    

    void setTimeOfDay(float hour); 
    float getTimeOfDay();
    void update(float delta_time);
    
    vec3 getFinalColor() const { return color * intensity; }
};