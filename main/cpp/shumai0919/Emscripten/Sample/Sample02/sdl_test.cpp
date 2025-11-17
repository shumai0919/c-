#include <SDL.h>
#include <emscripten.h>

SDL_Window* window;
SDL_Renderer* renderer;

void loop() {
    SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Emscripten Demo", 100, 100, 400, 300, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    emscripten_set_main_loop(loop, 0, 1);
}
