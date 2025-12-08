[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
chcp 65001 > $null


Write-Host "Quick Build - SDL Viewer with ImGui" -ForegroundColor Cyan

$SDL_PATH = "x86_64-w64-mingw32"

$sources = @(
    "main.cpp",
    "src/renderer.cpp",
    "src/geometry.cpp",
    "src/utils.cpp",
    "src/constants.cpp",
    "src/tgaimage.cpp",
    "src/vec3.cpp",
    "src/vec4.cpp",
    "src/model.cpp",
    "src/antialiasing.cpp",
    "src/colorTable.cpp",
    "src/vec2.cpp",
    "src/mat4.cpp",
    "src/camera.cpp",
    # ← 新增：ImGui源文件
    "imgui/imgui.cpp",
    "imgui/imgui_draw.cpp",
    "imgui/imgui_tables.cpp",
    "imgui/imgui_widgets.cpp",
    "imgui/backends/imgui_impl_sdl2.cpp",
    "imgui/backends/imgui_impl_sdlrenderer2.cpp"

)

g++ -Wall -g3 -std=c++17 `
    -Iinclude "-I$SDL_PATH/include/SDL2" -Iimgui -Iimgui/backends `
    $sources `
    "-L$SDL_PATH/lib" `
    -lmingw32 -lSDL2main -lSDL2 `
    -o build/viewer.exe

if ($LASTEXITCODE -eq 0) {
    Write-Host "[OK] Build success!" -ForegroundColor Green
    Write-Host "Running..." -ForegroundColor Yellow
    .\build\viewer.exe
} else {
    Write-Host "[ERROR] Build failed!" -ForegroundColor Red
}