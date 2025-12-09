# 确保在项目根目录
Set-Location $PSScriptRoot

# 添加MSYS2到PATH
$env:Path = "C:\msys64\ucrt64\bin;$env:Path"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "CMake Build - Software Rasterizer" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

# 创建build目录（如果不存在）
if (!(Test-Path "build_cmake")) {
    Write-Host "📁 Creating build directory..." -ForegroundColor Yellow
    New-Item -ItemType Directory -Path "build_cmake" | Out-Null
}


Set-Location build_cmake

if (!(Test-Path "build.ninja")) {
    Write-Host "🔧 Configuring CMake..." -ForegroundColor Yellow
    cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "[ERROR] CMake configuration failed!" -ForegroundColor Red
        Set-Location ..
        exit 1
    }
}

# 编译
Write-Host "🔨 Building..." -ForegroundColor Yellow
cmake --build . -j 8

# 检查结果
Set-Location ..

if ($LASTEXITCODE -eq 0) {
    Write-Host "[OK] Build success!" -ForegroundColor Green
    
    # 运行
    if (Test-Path "build_cmake\bin\viewer.exe") {
        Write-Host "Running..." -ForegroundColor Cyan
        .\build_cmake\bin\viewer.exe
    } else {
        Write-Host "[WARNING] viewer.exe not found!" -ForegroundColor Yellow
    }
} else {
    Write-Host "[ERROR] Build failed!" -ForegroundColor Red
}