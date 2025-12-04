#include "constants.h"

const int width = 800;
const int height = 800;
const float MouseSensitivity = 0.1f;
const float ROTATE_SENSITIVITY=0.01f;
const float PAN_SENSITIVITY=0.01f;
const float ZOOM_SENSITIVITY=0.5f;
const std::string outputCubeRender = "cube_render.png";
const std::string output_head = "head.png";
const std::string output_path = "output_exe/";
const std::string African_Head = "models/AfricanHead.obj";
const std::string African_Head_Diffuse = "obj/african_head/african_head_diffuse.tga";
const float MSAA_4X_OFFSETS[4][2] = {
    {0.25f, 0.25f},
    {0.75f, 0.25f},
    {0.75f, 0.75f},
    {0.25f, 0.75f}
};

//camera
const float Fov=45.0f/180.0f*3.1415926535f;
const float Near=0.1f;
const float aspect=width/height;
const float Far=100.0f;