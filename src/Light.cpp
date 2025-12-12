// sun_light.cpp
#include "Light.h"
#include <cmath>
#include"geometry.h"
SunLight::SunLight() 
    : direction(normalize(vec3(1, 1, 1)))
    , color(1, 1, 1)
    , intensity(1.0f)
    , time_of_day(12.0f)
    , auto_rotate(false)
{}

SunLight::SunLight(const vec3& dir, const vec3& col, float inten)
    : direction(normalize(dir))
    , color(col)
    , intensity(inten)
    , time_of_day(12.0f)
    , auto_rotate(false)
{}

SunLight::~SunLight() {}

void SunLight::setDirection(const vec3& dir) {
    direction = normalize(dir);
}

void SunLight::setColor(const vec3& col) {
    color = col;
}

void SunLight::setIntensity(float inten) {
    intensity = std::max(0.0f, inten);
}
float SunLight::getTimeOfDay(){
    return time_of_day;
}
void SunLight::setTimeOfDay(float hour) {
    time_of_day = hour;

    float angle = (hour - 6.0f) / 12.0f * 3.14159f; 
    
    direction.x = 0.5f;
    direction.y = std::sin(angle);  // -1 到 1
    direction.z = std::cos(angle);
    direction = normalize(direction);
    

    if (hour < 6.0f || hour > 18.0f) {
        color = vec3(0.2f, 0.2f, 0.4f);
        intensity = 0.1f;
    } else if (hour < 8.0f || hour > 16.0f) {
        color = vec3(1.0f, 0.6f, 0.3f);
        intensity = 0.6f;
    } else {
        color = vec3(1.0f, 1.0f, 0.95f);
        intensity = 1.0f;
    }
}

void SunLight::update(float delta_time) {
    if (auto_rotate) {
        time_of_day += delta_time * 0.1f;  // 控制旋转速度
        if (time_of_day >= 24.0f) {
            time_of_day -= 24.0f;
        }
        setTimeOfDay(time_of_day);
    }
}