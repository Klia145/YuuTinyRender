#include<SDL.h>
#include<iostream>
#include"renderer.h"
#include"constants.h"
#include"colorTable.h"
#include"geometry.h"
#include"camera.h"
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
    
    Camera camera(vec3(0,0,3),vec3(0,0,0),Fov,aspect,Near,Far);
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
            if(event.type==SDL_KEYDOWN){
                vec3 model_center=model.getCenter();
                bool ctrl_pressed=(SDL_GetModState()&KMOD_CTRL);
                if(event.key.keysym.sym==SDLK_KP_PERIOD){
                    
                    camera.focusOn(model_center,CameraDistance);
                    std::cout<<"聚焦模型中"<<std::endl;
                }
                if(event.key.keysym.sym == SDLK_f){
                   camera.focusPreset(ViewPreset::DEFAULT, model_center, 3.0f);
                }
                if(event.key.keysym.sym==SDLK_KP_1){
                    if(ctrl_pressed){
                    camera.focusPreset(ViewPreset::BACK, model_center, 3.0f);
                    }
                    else{
                    camera.focusPreset(ViewPreset::FRONT, model_center, 3.0f);
                    }
                }
                if(event.key.keysym.sym == SDLK_KP_3){
                    if(ctrl_pressed){
                    camera.focusPreset(ViewPreset::LEFT, model_center, 3.0f);
                    } else {
                    camera.focusPreset(ViewPreset::RIGHT, model_center, 3.0f);
                    }
                }
                if(event.key.keysym.sym==SDLK_KP_7){
                    if(ctrl_pressed){
                        camera.focusPreset(ViewPreset::BOTTOM,model_center,3.0f);
                    }
                    else{
                        camera.focusPreset(ViewPreset::TOP,model_center,3.0f);
                    }
                }
                if(event.key.keysym.sym==SDLK_ESCAPE){
                    running=false;
                }
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
                    std::cout << "左键松开！" << std::endl;
                }
                if(event.button.button==SDL_BUTTON_RIGHT){
                    mouse_right_down=false;
                    std::cout << "右键松开！" << std::endl;
                }
            }
            if(event.type==SDL_MOUSEMOTION){
                int current_x = event.motion.x;
                int current_y = event.motion.y;
                int delta_x = current_x - last_mouse_x;
                int delta_y = current_y - last_mouse_y;
                
                if (mouse_left_down) {
                    camera.rotate(delta_x*Config::MouseSensitivity,delta_y*MouseSensitivity);
                }
                if(mouse_right_down){
                    camera.pan(delta_x*Config::MouseSensitivity,delta_y*Config::MouseSensitivity);
                }
                last_mouse_x=current_x;//这部分就是生命周期
                last_mouse_y=current_y;
            }
            if(event.type==SDL_MOUSEWHEEL)
            {
                float zoom_delta=event.wheel.y*Config::ZOOM_SENSITIVITY;
                camera.zoom(zoom_delta);
                std::cout<<"Zoom:"<<camera.getDistance()<<std::endl;
            }
        }
        TGAImage framebuffer(width,height,3,ColorTable::getColor(ColorName::BLACK));
        render_skyBox(framebuffer);
        std::vector<float>zbuffer(width*height,-1e9);

        mat4 mvp=camera.getViewProjectionMatrix();

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