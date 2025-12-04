# Mysoft Rasterizer Framework

![Language](https://img.shields.io/badge/language-C%2B%2B-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

一个从零开始编写的微型软光栅渲染器（Software Rasterizer）。
不依赖 OpenGL、DirectX 或 Vulkan 等图形 API，纯 CPU 实现完整的图形渲染管线。

本项目旨在通过手写核心算法，深入理解 GPU 的工作原理以及 3D 图形学的底层数学逻辑。

> **当前状态：** 几何阶段验证通过（视图变换 + 视锥体裁剪）。

## 🖼️ 渲染效果 (Gallery)

[待补充：视锥体裁剪测试截图]

## ✨ 特性 (Features)

目前已实现的管线阶段：

- **数学库 (Math Core)**
  - 纯手写的 `Vector` 与 `Matrix` 运算库。
  - 实现了平移、旋转、缩放及透视投影矩阵的构建。
- **几何管线 (Geometry Pipeline)**
  - 完整的坐标变换链路：`Local` -> `World` -> `View` -> `Clip` -> `Screen`。
  - **视锥体裁剪 (Frustum Clipping)**：实现了针对视锥体平面的图元裁剪算法，解决了近平面剔除和视野边缘的几何错误问题。
- **平台层 (Platform)**
  - 使用 SDL2 进行窗口管理和像素缓冲区（Framebuffer）的提交。
  - (正在进行) 集成 ImGui 以实现实时参数调试。

## 🚀 构建与运行 (Build & Run)

### 环境依赖
* C++ 编译器 (支持 C++17)
* SDL2 开发库

### 构建方式
项目根目录提供了构建脚本：

```bash
# 如果使用 PowerShell
./build.ps1

# 或者直接运行批处理
build.bat