#pragma once
#include "vec3.h"
#include<vector>
#include<string>
#include"vec2.h"

class Model{
    private:
    std::vector<vec3>vertices_;
    std::vector<std::vector<int>>faces_;
    std::vector<vec2>uvs_;
    std::vector<std::vector<int>>uv_indices_;
    std::vector<vec3>normals_;
    std::vector<std::vector<int>>normal_indices_;


    public:
    Model(const std::string&filename);
    int nverts() const;
    int nfaces() const;

    vec3 vert(int i) const;
    vec2 uv(int iface,int nthvert) const;

    std::vector<int>face(int i) const;
    vec3 vert(int iface,int nthvert) const;
    vec3 normal(int iface,int nthvert)const;

    vec3 getCenter()const;
};