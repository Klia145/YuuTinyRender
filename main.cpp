#include<SDL.h>
#include<iostream>
#include"renderer.h"
#include"constants.h"
#include"colorTable.h"
#include"geometry.h"

void copy_image_to_surface(TGAImage&image,SDL_Surface* surface,int w,int h){
    SDL_LockSurface(surface);

    for(int y=0;y<h;y++){
        for(int x=0;x<w;x++){
            TGAColor color=image.get(x,y);
            Uint32* pixels=(Uint32*)surface->pixels;
            pixels[y*w+x]=SDL_MapRGB(
                surface->format,
                color.bgra[2],
                color.bgra[1],
                color.bgra[0]
            );

        }
    }
    SDL_UnlockSurface(surface);

}

int main(int argc,char* argv[]){

    if(SDL_Init(SDL_INIT_VIDEO)<0){
        std::cerr<<"Error initializing SDL:"<<SDL_GetError()<<std::endl;
        return 1;
    }

    SDL_Window* window=SDL_CreateWindow(
        "Software Rasterizer-Yuu's Interactive Viewer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,height,SDL_WINDOW_SHOWN
    );
    if(!window){
        std::cerr<<"Window creation failed:"<<SDL_GetError()<<std::endl;
        SDL_Quit();
        return 1;
    }
    SDL_Surface* screen=SDL_GetWindowSurface(window);
    std::cout<<" 加载模型和贴图"<<std::endl;

    Model model(African_Head);
    TGAImage texture;

     
    if (!texture.read_tga_file(African_Head_Diffuse)) {
        std::cerr << "Failed to load texture" << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    std::cout << "==================================" << std::endl;
    std::cout << "SDL Viewer Started!" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  Left/Right Arrow: Rotate model" << std::endl;
    std::cout << "  Up/Down Arrow: Zoom in/out" << std::endl;
    std::cout << "  ESC: Exit" << std::endl;
    std::cout << "==================================" << std::endl;
    
    // 渲染参数
    vec3 light_dir=normalize(vec3(1,1,1));
    float angle_y=0.0f;
    float angle_x=0.0f;
    float distance=3.0f;
    float pan_x=0.0f;
    float pan_y=0.0f;
    bool mouse_left_down=false;
    bool mouse_right_down=false;
    int last_mouse_x=0;
    int last_mouse_y=0;

    bool running=true;
    Uint32 Last_time=SDL_GetTicks();
    int frame_count=0;

    while(running){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT){
                running=false;
            }
            if(event.type==SDL_BUTTON_LEFT){
                mouse_left_down=true;
                last_mouse_x=event.button.x;
                last_mouse_y=event.button.y;
                std::cout<<"Left mouse button pressed at ("<<event.button.x<<","<<event.button.y<<")"<<std::endl;
            }
            if(event.type==SDL_MOUSEBUTTONDOWN){
                if(event.button.button==SDL_BUTTON_LEFT){
                    mouse_left_down=true;
                    last_mouse_x=event.button.x;
                    last_mouse_y=event.button.y;

                }
                if(event.button.button==SDL_BUTTON_RIGHT){
                    mouse_right_down=true;
                    last_mouse_x=event.button.x;
                    last_mouse_y=event.button.y;

                }
            }
            if(event.type==SDL_MOUSEBUTTONUP){
                if(event.button.button==SDL_BUTTON_LEFT){
                    mouse_left_down=false;
                }
                if(event.button.button==SDL_BUTTON_RIGHT){
                    mouse_right_down=false;
                }
            }
            if(event.type==SDL_MOUSEMOTION){
                int current_x = event.motion.x;
                int current_y = event.motion.y;
                int delta_x = current_x - last_mouse_x;
                int delta_y = current_y - last_mouse_y;
                
                if (mouse_left_down) {
                    angle_y += delta_x * MouseSensitivity;  // 灵敏度：0.01
                    
                    angle_x += delta_y * MouseSensitivity;
                    

                    if (angle_x > 1.5f) angle_x = 1.5f;
                    if (angle_x < -1.5f) angle_x = -1.5f;
                    
                    std::cout << "Rotation - Y: " << (angle_y * 180.0f / 3.14159f) 
                              << "°, X: " << (angle_x * 180.0f / 3.14159f) << "°" << std::endl;
                }
                if(mouse_right_down){
                pan_x+=delta_x*PAN_SENSITIVITY;
                pan_y-=delta_y*PAN_SENSITIVITY;
                std::cout<<"Pan - X: "<<pan_x<<", Y: "<<pan_y<<std::endl;
                }
                last_mouse_x=current_x;//这部分就是生命周期
                last_mouse_y=current_y;
            }
            if(event.type==SDL_MOUSEWHEEL)
            {
                if(event.wheel.y>0){
                    distance-=0.5f;
                    if(distance<0.5f)distance=0.5f;
                }
                else if(event.wheel.y<0){
                    distance+=0.5f;
                    if(distance>30.0f)distance=30.0f;
                }
                std::cout<<"Zoom: "<<distance<<std::endl;
            }
        }
        TGAImage framebuffer(width,height,3,ColorTable::getColor(ColorName::BLACK));
        std::vector<float>zbuffer(width*height,-1e9);

        mat4 model_mat=mat4::rotate_y(angle_y)*mat4::rotate_x(angle_x);
        mat4 view=mat4::translate(pan_x,pan_y,-distance);
        mat4 proj=mat4::perspective(Fov,aspect,Near,Far);
        mat4 mvp=proj*view*model_mat;

        Render(framebuffer,model,zbuffer,texture,light_dir,mvp);
        copy_image_to_surface(framebuffer,screen,width,height);

        SDL_UpdateWindowSurface(window);
        frame_count++;
        Uint32 current_time = SDL_GetTicks();
        if (current_time - Last_time >= 1000) {
            std::cout << "FPS: " << frame_count << std::endl;
            frame_count = 0;
            Last_time = current_time;
        }
        
        SDL_Delay(16);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}