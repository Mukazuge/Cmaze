#include <stdio.h>
#include <Windows.h>
#include <SDL2/SDL.h>

typedef enum { false, true } bool;

SDL_Window * window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Event event;

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Pulsa X para comenzar de nuevo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1248, 928, 0);
    
    HWND windowHandle = GetConsoleWindow();
	ShowWindow(windowHandle,SW_HIDE);
    
    renderer = SDL_CreateRenderer(window, -1, 0);	

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

SDL_Texture *loadBMP(char *file_name) {
    SDL_Surface *image = SDL_LoadBMP(file_name);
    SDL_SetColorKey(image, 1, SDL_MapRGB(image->format, 255, 0, 255));
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
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
}

void resizeWindow(int newWidth, int newHeight){
    SDL_SetWindowSize(window, newWidth, newHeight);
}
