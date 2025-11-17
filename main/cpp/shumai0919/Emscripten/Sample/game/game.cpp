#include <SDL.h>
#include <emscripten.h>

// ウィンドウサイズ
const int WIDTH = 640;
const int HEIGHT = 480;

// ボールの情報
struct Ball {
    float x, y;
    float vx, vy;
    int radius;
};

SDL_Window* window;
SDL_Renderer* renderer;
Ball ball;

// 毎フレーム呼ばれる関数
void loop() {
    // 背景を黒で塗る
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // ボールの座標を更新
    ball.x += ball.vx;
    ball.y += ball.vy;

    // 壁で反射
    if (ball.x < ball.radius || ball.x > WIDTH - ball.radius)
        ball.vx = -ball.vx;
    if (ball.y < ball.radius || ball.y > HEIGHT - ball.radius)
        ball.vy = -ball.vy;

    // ボールを描画（白）
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int w = -ball.radius; w < ball.radius; w++) {
        for (int h = -ball.radius; h < ball.radius; h++) {
            if (w * w + h * h <= ball.radius * ball.radius) {
                SDL_RenderDrawPoint(renderer, (int)ball.x + w, (int)ball.y + h);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Emscripten Mini Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    // ボール初期化
    ball.x = WIDTH / 2;
    ball.y = HEIGHT / 2;
    ball.vx = 2.5f;
    ball.vy = 1.8f;
    ball.radius = 20;

    // メインループ登録
    emscripten_set_main_loop(loop, 0, 1);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
