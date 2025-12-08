#include"mat4.h"
#include<cmath>
#include"utils.h"
mat4::mat4():mat4(1.0f){};
mat4::mat4(float diagonal){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            m[i][j]=0.0f;
        }
    }
    m[0][0]=diagonal;
    m[1][1]=diagonal;
    m[2][2]=diagonal;
    m[3][3]=diagonal;
}
mat4 mat4::operator*(const mat4&other)const{
    mat4 result(0);
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            for(int k=0;k<4;k++){
                result.m[i][j]+=m[i][k]*other.m[k][j];
            }
        }
    }
    return result;
};
vec4 mat4::operator*(const vec4&v)const{
    vec4 result;
     return vec4(
        m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
        m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
        m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
        m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
    );
}
vec4 operator*(const vec4&v,const mat4&m){
    return vec4(v.x*m.m[0][0]+v.y*m.m[1][0]+v.z*m.m[2][0]+v.w*m.m[3][0],
    v.x*m.m[0][1]+v.y*m.m[1][1]+v.z*m.m[2][1]+v.w*m.m[3][1],
    v.x*m.m[0][2]+v.y*m.m[1][2]+v.z*m.m[2][2]+v.w*m.m[3][2],
    v.x*m.m[0][3]+v.y*m.m[1][3]+v.z*m.m[2][3]+v.w*m.m[3][3]
    );
}
mat4 mat4::identity(){
    return mat4(1);
}
mat4 mat4::translate(float x,float y,float z){
    mat4 result(1);
    result.m[0][3]=x;
    result.m[1][3]=y;
    result.m[2][3]=z;
    return result;
}
mat4& mat4::Applytranslate(float x,float y,float z){
    mat4 result(1);
    result.m[0][3]=x;
    result.m[1][3]=y;
    result.m[2][3]=z;
    *this=*this*result;
    return *this;
}

mat4 mat4::scale(float x,float y ,float z){
    mat4 result(1);
    result.m[0][0]=x;
    result.m[1][1]=y;
    result.m[2][2]=z;
    return result;
}
mat4 mat4::rotate_x(float angle){
    mat4 result(0);
    result.m[0][0]=1;
    result.m[1][1]=std::cos(angle);
    result.m[1][2]=-std::sin(angle);
    result.m[2][1]=std::sin(angle);
    result.m[2][2]=std::cos(angle);
    result.m[3][3]=1;
    return result;
}
mat4 mat4::rotate_y(float angle){
    mat4 result(0);
    result.m[0][0]=std::cos(angle);
    result.m[0][2]=std::sin(angle);
    result.m[2][0]=-std::sin(angle);
    result.m[2][2]=std::cos(angle);
    result.m[1][1]=1;
    result.m[3][3]=1;
    return result;
}
mat4 mat4::rotate_z(float angle){
    mat4 result(0);
    result.m[0][0]=std::cos(angle);
    result.m[0][1]=-std::sin(angle);
    result.m[1][0]=std::sin(angle);
    result.m[1][1]=std::cos(angle);
    result.m[2][2]=1;
    result.m[3][3]=1;
    return result;
}
mat4& mat4::Applyscale(float x,float y,float z){
    mat4 result(1);
    result.m[0][0]=x;
    result.m[1][1]=y;
    result.m[2][2]=z;
    *this=*this*result;
    return *this;
}
mat4 mat4::perspective(float fov,float aspect,float near,float far){
    mat4 result(0);

    float tan_half_fov=tan(fov/2.0f);
    float f=1.0f/tan_half_fov;

    result.m[0][0]=f/aspect;
    result.m[1][1]=f;
    result.m[2][2]=-(far+near)/(far-near);
    result.m[2][3]=-(2.0f*far*near)/(far-near);

    result.m[3][2]=-1.0f;
    return result;
}
mat4 mat4::lookAt(const vec3&eye,const vec3&target,const vec3&up){
    vec3 forward=normalize(target-eye);
    vec3 right=normalize(forward.cross(up));
    vec3 camera_up=right.cross(forward);

    mat4 rotation(1);
    rotation.m[0][0]=right.x;
    rotation.m[0][1]=right.y;
    rotation.m[0][2]=right.z;

    rotation.m[1][0] = camera_up.x;
    rotation.m[1][1] = camera_up.y;
    rotation.m[1][2] = camera_up.z;

    rotation.m[2][0] = -forward.x;  // 注意：右手坐标系Z朝外，所以取反
    rotation.m[2][1] = -forward.y;
    rotation.m[2][2] = -forward.z;

    mat4 translation = mat4::translate(-eye.x, -eye.y, -eye.z);
    
    return rotation * translation;
}
