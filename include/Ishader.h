#pragma once
#include "geometry.h"
#include "tgaimage.h"

/*
@brief IShader接口
这是一个抽象基类。模拟GPU的可编程管线
任何一个具体的渲染效果都应该继承这个类
HLSL关系映射

1.vertex() ->HLSL的vertex(顶点着色器)
2.fragment() ->HLSL的fragment(片元着色器)
3.varying ->HLSL的struct varying(顶点到片元的插值数据)
4.Model类中的std::vector数组离，代表着数据读取函数.

在CPU模拟中只能作为Model文件中的函数进行调用


*/
struct IShader{
    virtual ~IShader(){}
    //变量插值：
    /*

    
    */


    virtual vec4 vertex(int iface,int nthvert)=0;

    /*
    对象是处理每个像素
    计算出的像素颜色->对应HLSL中的SV_Target

    @return bool 返回true意味着丢弃该像素，false表示保留
    片元着色器基类。在Hlsl文件中基于此进行扩展


    作用：
    用重心坐标对varying进行插值(c++需要手动写一遍，HLSL自动做一遍)
    2：采样纹理数据
    3:计算光照
    
    */
    virtual bool fragment(vec3 bar,TGAColor &color)=0;

};