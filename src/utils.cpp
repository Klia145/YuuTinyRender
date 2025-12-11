#include "utils.h"
#include "constants.h"
#include<cmath>
std::tuple<int, int, float> project(vec4 clip_pos) {
    float w = clip_pos.w;

    if(std::abs(w)<0.001f){
        w=0.001f;
    }

    float ndcX = clip_pos.x / w;
    float ndcY = clip_pos.y / w;
    float ndcZ = clip_pos.z / w;
    ndcX=std::max(-1.0f,std::min(1.0f,ndcX));
    ndcY=std::max(-1.0f,std::min(1.0f,ndcY));

    int margin = 100;
    int x = (ndcX + 1.0) * (width - 2 * margin) / 2.0 + margin;
    int y = (1.0 - ndcY) * (height - 2 * margin) / 2.0 + margin;

    x = std::max(0, std::min(x, width - 1));
    y = std::max(0, std::min(y, height - 1));
    return {x, y, ndcZ};
}
