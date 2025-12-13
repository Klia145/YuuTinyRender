// main.cpp
#include "Application.h"
#include <iostream>
#include <windows.h>

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(65001);
    system("chcp 65001 > nul");
    
    Application app(800, 800);
    
    if (!app.init()) {
        std::cerr << "❌ 应用初始化失败" << std::endl;
        return 1;
    }
    
    std::cout << "✅ 软光栅器启动成功！" << std::endl;
    
    app.run();
    app.cleanup();
    
    return 0;
}