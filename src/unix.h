#include <stdio.h>
#include <SDL2/SDL.h>

#define SDL_MAIN_HANDLED

SDL_Window * window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Event event;

Uint32 old_time, current_time;
float ftime;

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Juego!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280-32, 960-32, 0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    current_time = SDL_GetTicks();
}

SDL_Texture *loadBMP(char *file_name) {
    SDL_Surface *image = SDL_LoadBMP(file_name);
    SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0xFF, 0x00, 0xFF));

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    
    SDL_FreeSurface(image);

    return texture;
}

void quit() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void clear() {
    SDL_RenderClear(renderer);
}

void render() {
    SDL_RenderPresent(renderer);
    old_time = current_time;
    current_time = SDL_GetTicks();
    ftime = (current_time - old_time) / 1000.0f;
}


void resizeWindow(int newWidth, int newHeight){
    SDL_SetWindowSize(window, newWidth, newHeight);
}
