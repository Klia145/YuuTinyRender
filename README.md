# C++ Software Rasterizer (Interactive Viewer)

![Language](https://img.shields.io/badge/language-C%2B%2B17-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux-lightgrey.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)

> **"What I cannot create, I do not understand."**

这是一个从零开始编写的软件光栅化渲染器，**不依赖任何图形 API (OpenGL/DirectX/Vulkan)**。项目旨在通过纯 CPU 计算模拟完整的 3D 渲染管线，深入探究图形学的底层数学原理与工程实现。

不仅复现了经典的渲染算法，还集成了一个**完全交互式的 GUI 环境**，支持实时调整光照、切换渲染模式及各种调试功能。

---

##  渲染效果 (Gallery)
<div align="center">
  <img src=![Image](https://github.com/user-attachments/assets/18b914d5-eff1-4795-ae64-aa325559ea41) width="80%" />
  <br>
  <i>图示：实时光照与阴影变化演示与线框模式演示</i>
</div>

### 实时交互环境
集成了 **Dear ImGui**，允许在运行时实时调整 Shader 参数、观察模型细节并监控性能。

![Overview](docs/screenshots/Screenshot 2025-12-14 133551.png)

### 渲染模式对比
支持多种渲染模式的实时热切换，用于调试几何结构与光照计算。

<table>
  <tr>
    <td align="center"><b>FogEffect</b></td>
    <td align="center"><b>Wireframe Mode</b></td>
    <td align="center"><b>Blinn-Phong Shading</b></td>
  </tr>
  <tr>
    <td align="center"><img src="docs/screenshots/fog.png" width="400"/></td>
    <td align="center"><img src="docs/screenshots/wireframe.png" width="400"/></td>
    <td align="center"><img src="docs/screenshots/shading.png" width="400"/></td>
  </tr>
  <tr>
    <td align="center">基于距离的雾颜色和物体颜色的插值运算<br>(带背面剔除)</td>
    <td align="center">基于重心坐标的三角形光栅化<br>(带背面剔除)</td>
    <td align="center">基于法线插值的高光渲染<br>(带纹理映射)</td>
  </tr>
</table>

---

##  核心特性 (Features)

###  渲染管线 (Pipeline)
* **Math Library**: 手写数学库，实现了 `Vector`, `Matrix`, `Quaternion` 等核心运算。
* **Transform**: 完整的 MVP (Model-View-Projection) 变换矩阵推导。
* **Clipping**: 视锥体裁剪 (Frustum Culling) 与 齐次空间裁剪。
* **Rasterization**: 
    * 基于包围盒 (Bounding Box) 的三角形遍历算法。
    * **透视校正插值 (Perspective Correct Interpolation)**：修复纹理在透视投影下的扭曲。
    * **深度测试 (Z-Buffering)**：处理复杂的物体遮挡关系。

###  着色与光照 (Shading & Lighting)
* **可编程管线模拟**: 抽象了 `VertexShader` 和 `FragmentShader`，模拟 GPU 工作流。
* **光照模型**: 
    * Flat Shading
    * Gouraud Shading
    * **Phong & Blinn-Phong Shading**
* **纹理映射**: 支持 Diffuse Map（漫反射贴图）与 Specular Map（高光贴图），使用双线性插值采样。
* **高级特性**:
    * **Gamma Correction**: 线性空间光照计算。
    * **Linear Fog**: 基于深度的线性雾效。

###  工程与交互 (Engineering & Tools)
* **Interactive UI**: 集成 ImGui，支持 FPS 统计、参数滑块调节。
* **Camera System**: 实现了类似 FPS 游戏的摄像机控制（平移/旋转/缩放）。
* **Debug Tools**: 世界空间网格 (Grid)、坐标轴可视化、线框模式切换。
* **Performance**: 使用 OpenMP 进行多线程像素着色加速。

---

##  控制指南 (Controls)

| 输入 | 动作 |
| :--- | :--- |
| **W / A / S / D** | 移动摄像机 (Move Camera) |
| **Mouse Drag** | 旋转视角 (Orbit/Rotate) |
| **Mouse Wheel** | 缩放视图 (Zoom) |
| **1 / 2 / 3** | 切换渲染模式 (Wireframe / Flat / Phong) |
| **P** | 切换投影方式 (Perspective / Orthographic) |
| **G** | 开关辅助网格 (Toggle Grid) |

---

##  快速开始 (Build & Run)

### 环境依赖
* **Compiler**: C++17 (MSVC / GCC / Clang)
* **Dependencies**: [SDL2](https://www.libsdl.org/) (Window & Input)

### 构建 (CMake)

```bash

git clone [https://github.com/YourUsername/SoftwareRasterizer.git](https://github.com/YourUsername/SoftwareRasterizer.git)
cd SoftwareRasterizer

mkdir build && cd build
cmake ..
cmake --build . --config Release

./SoftwareRasterizer
 致谢与参考 (References)
GAMES101 - 闫令琪教授 (现代计算机图形学入门)
TinyRenderer - Dmitry V. Sokolov
LearnOpenGL - Joey de Vries

<div align="center"> <b>Made with  by 屿 (Yuu)</b>


<i>Technical Artist Candidate</i> </div>