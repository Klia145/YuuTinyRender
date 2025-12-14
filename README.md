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
  <img src="docs/screenshots/interactive1.gif" width="40%" />
  <br>
  <i>图示：默认渲染</i>
</div>
<div align="center">
  <img src="docs/screenshots/interactive2.gif" width="40%" />
  <br>
  <i>图示：线框模式</i>
</div>
<div align="center">
  <img src="docs/screenshots/interactive3.gif" width="40%" />
  <br>
  <i>图示：基础Blinn-Phong模式和多角度光照</i>
</div>

## 📊 性能基准 (Performance Benchmark)

<div align="center">
  <img src="docs/screenshots/interactivefps.gif" width="80%" />
  <br>
  <i>图示：性能测试/i>
</div>

> 测试模型：African Head (约 15,000 三角面) | 分辨率：800x600 | 平台：Windows 10

### 🖥️ 测试环境 (Environment)
* **CPU**: [Intel Core i7-12700H] @ 2.30GHz (Single Core / Multi-thread)
* **RAM**: 32 GB DDR4
* **Optimization**: OpenMP Enabled (Parallel Rasterization)

### 📈 帧率统计 (Frame Rates)

| Render Mode | FPS (Avg) | Frame Time | Computational Cost |
| :--- | :--- | :--- | :--- |
| **Wireframe** | **60+ FPS** | < 16.6 ms |  Low (Vertex Processing Only) |
| **Blinn-Phong** | **~25 FPS** | ~40.0 ms |  High (Per-pixel Lighting + Texture) |
| **4x MSAA** | **~15 FPS** | ~66.6 ms |  Very High (4x Depth Samples) |

### ⚡ 优化策略 (Optimizations)
为了在 CPU 上实现可交互的帧率，项目实施了以下优化：

1.  **Back-face Culling (背面剔除)**:
    * 在光栅化阶段前计算三角形法线，剔除背向摄像机的面。
    * **收益**: 减少了约 **50%** 的像素着色计算量。

2.  **Bounding Box Traversal (包围盒遍历)**:
    * 仅遍历三角形 AABB (Axis-Aligned Bounding Box) 内的像素，而非全屏扫描。
    * **收益**: 将像素遍历复杂度从屏幕分辨率 $O(W \times H)$ 降低至三角形大小相关。

3.  **Parallel Rasterization (OpenMP)**:
    * 利用 `#pragma omp parallel for` 对三角形光栅化循环进行多线程加速。
    * **收益**: 在多核 CPU 上提升了约 **40%-60%** 的渲染性能。

4.  **Early-Z Test (早期深度测试)**:
    * 在执行复杂的 Fragment Shader (纹理采样/光照计算) 之前，先进行深度测试。
    * **收益**: 避免了被遮挡像素的无效计算。

### 实时交互环境
集成了 **Dear ImGui**，允许在运行时实时调整 Shader 参数、观察模型细节并监控性能。

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
```


致谢与参考 (References)
GAMES101 - 闫令琪教授 (现代计算机图形学入门)
TinyRenderer - Dmitry V. Sokolov
LearnOpenGL - Joey de Vries





