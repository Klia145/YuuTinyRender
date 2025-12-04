#include<SDL.h>
#include<iostream>
int main(int argc,char* argv[]){
    if(SDL_Init(SDL_INIT_VIDEO)<0){
        std::cout<<"SDL could not initialize! SDL_Error: "<<SDL_GetError()<<std::endl;
        return 1;
    }
    std::cout<<"初始化成功"<<std::endl;

    SDL_Window* window=SDL_CreateWindow(
        "我的SDL窗口",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,600,SDL_WINDOW_SHOWN
    );
    if(!window){
        std::cout<<"窗口创建失败！SDL_Error: "<<SDL_GetError()<<std::endl;
        SDL_Quit();
        return 1;
    }

    std::cout<<"窗口成功创建"<<std::endl;
    SDL_Surface* screen = SDL_GetWindowSurface(window);
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 0, 0));
    SDL_UpdateWindowSurface(window);
    SDL_Delay(5000);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    std::cout << "程序结束" << std::endl;
    return 0;

}