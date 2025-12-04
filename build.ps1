Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Building Software Rasterizer..." -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

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
    "src/mat4.cpp"
)

Write-Host "Compiling..." -ForegroundColor Yellow

g++ -Wall -g3 -std=c++17 `
    -Iinclude "-I$SDL_PATH/include/SDL2" `
    $sources `
    "-L$SDL_PATH/lib" `
    -lmingw32 -lSDL2main -lSDL2 `
    -o build/main.exe

if ($LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "[SUCCESS] Build completed!" -ForegroundColor Green
    Write-Host ""
    Write-Host "Copying SDL2.dll..." -ForegroundColor Yellow
    
    if (!(Test-Path "build")) {
        New-Item -ItemType Directory -Path "build" | Out-Null
    }
    
    Copy-Item "$SDL_PATH\bin\SDL2.dll" "build\" -Force
    
    Write-Host "[OK] SDL2.dll copied" -ForegroundColor Green
    Write-Host ""
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "Run with:" -ForegroundColor Green
    Write-Host "  cd build" -ForegroundColor White
    Write-Host "  .\main.exe" -ForegroundColor White
    Write-Host "========================================" -ForegroundColor Cyan
} else {
    Write-Host ""
    Write-Host "[ERROR] Build failed!" -ForegroundColor Red
    Write-Host ""
}