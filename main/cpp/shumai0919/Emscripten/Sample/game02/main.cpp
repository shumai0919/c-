// main.cpp
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include <cmath>

const int WIN_W = 900;
const int WIN_H = 600;

struct Texture {
    SDL_Texture* tex = nullptr;
    int w = 0, h = 0;
    void free(SDL_Renderer* r){ if(tex){ SDL_DestroyTexture(tex); tex=nullptr; } }
};

SDL_Texture* loadTexture(SDL_Renderer* ren, const std::string& path, int &w, int &h){
    SDL_Surface* s = IMG_Load(path.c_str());
    if(!s){ std::cerr << "IMG_Load failed: " << IMG_GetError() << " for " << path << "\n"; return nullptr; }
    SDL_Texture* t = SDL_CreateTextureFromSurface(ren, s);
    w = s->w; h = s->h;
    SDL_FreeSurface(s);
    return t;
}

void renderText(SDL_Renderer* ren, TTF_Font* font, const std::string& txt, int x, int y){
    SDL_Color col={255,255,255,255};
    SDL_Surface* surf = TTF_RenderUTF8_Blended(font, txt.c_str(), col);
    if(!surf){ std::cerr<<"TTF_Render error: "<<TTF_GetError()<<"\n"; return; }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
    SDL_Rect r={x,y,surf->w,surf->h};
    SDL_FreeSurface(surf);
    SDL_RenderCopy(ren, tex, nullptr, &r);
    SDL_DestroyTexture(tex);
}

int main(int argc, char** argv){
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)!=0){ std::cerr<<"SDL_Init: "<<SDL_GetError()<<"\n"; return 1; }
    if(IMG_Init(IMG_INIT_PNG)==0){ std::cerr<<"IMG_Init failed: "<<IMG_GetError()<<"\n"; /* continue */ }
    if(TTF_Init()!=0){ std::cerr<<"TTF_Init: "<<TTF_GetError()<<"\n"; return 1; }

    SDL_Window* win = SDL_CreateWindow("Clicker (SDL2)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, 0);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // --- assets paths (extracted from sb3)
    std::string assets = "./クリッカー_assets/";
    std::string mainBtnImg = assets + "e7d6fb78caf8beff0988482f9b9dbccc.png";
    std::string autoclickBtnImg = assets + "0d505ad780bd62497226e9665e0a64ef.png";
    std::string clickAmtBtnImg = assets + "450fc596d8070e6c3a2893b3660d15e3.png";

    int w,h;
    Texture mainBtn, autoBtn, upBtn;
    mainBtn.tex = loadTexture(ren, mainBtnImg, mainBtn.w, mainBtn.h);
    autoBtn.tex  = loadTexture(ren, autoclickBtnImg, autoBtn.w, autoBtn.h);
    upBtn.tex    = loadTexture(ren, clickAmtBtnImg, upBtn.w, upBtn.h);

    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 20);
    if(!font){ std::cerr<<"TTF_OpenFont failed: "<<TTF_GetError()<<"\n"; }

    // --- initial variables (taken from project.json)
    long long score = 1279997LL;
    long long clickAmount = 155LL;
    long long clickPrice = 21693LL;
    long long autoclicks = 155LL;
    long long autoclickPrice = 22778LL;
    int autoclickValue = 2; // 加算されるクリック数（購入click数）
    double priceMult = 1.05;

    // layout rects
    SDL_Rect mainRect = { (WIN_W - 300)/2, 120, 300, 200 };
    SDL_Rect autoRect = { WIN_W - 220, 100, 180, 80 };
    SDL_Rect upRect   = { WIN_W - 220, 220, 180, 80 };

    // scale texture sizes if available
    if(mainBtn.tex){ mainRect.w = 300; mainRect.h = 200; }
    if(autoBtn.tex){ /* keep rects */ }
    if(upBtn.tex){ /* keep rects */ }

    bool running = true;
    SDL_Event e;

    Uint32 lastAutoTick = SDL_GetTicks();

    while(running){
        while(SDL_PollEvent(&e)){
            if(e.type==SDL_QUIT) running=false;
            else if(e.type==SDL_MOUSEBUTTONDOWN && e.button.button==SDL_BUTTON_LEFT){
                int mx=e.button.x, my=e.button.y;
                // main click
                if(mx>=mainRect.x && mx<=mainRect.x+mainRect.w && my>=mainRect.y && my<=mainRect.y+mainRect.h){
                    score += clickAmount;
                }
                // autoclick buy
                if(mx>=autoRect.x && mx<=autoRect.x+autoRect.w && my>=autoRect.y && my<=autoRect.y+autoRect.h){
                    if(score >= autoclickPrice){
                        score -= autoclickPrice;
                        autoclicks += 1;
                        // increase price (round)
                        autoclickPrice = (long long)std::floor(autoclickPrice * priceMult + 0.5);
                    }
                }
                // clickAmount upgrade buy
                if(mx>=upRect.x && mx<=upRect.x+upRect.w && my>=upRect.y && my<=upRect.y+upRect.h){
                    if(score >= clickPrice){
                        score -= clickPrice;
                        clickAmount += autoclickValue; // increase per buy
                        clickPrice = (long long)std::floor(clickPrice * priceMult + 0.5);
                    }
                }
            }
        }

        // autoclick tick every 1000 ms
        Uint32 now = SDL_GetTicks();
        if(now - lastAutoTick >= 1000){
            long long gain = autoclicks * (long long)autoclickValue;
            score += gain;
            lastAutoTick = now;
        }

        // render
        SDL_SetRenderDrawColor(ren, 20, 24, 30, 255);
        SDL_RenderClear(ren);

        // draw main button (image if available)
        if(mainBtn.tex){
            SDL_RenderCopy(ren, mainBtn.tex, nullptr, &mainRect);
        } else {
            SDL_SetRenderDrawColor(ren, 200, 180, 50, 255);
            SDL_RenderFillRect(ren, &mainRect);
        }

        // draw autoclick button
        if(autoBtn.tex) SDL_RenderCopy(ren, autoBtn.tex, nullptr, &autoRect);
        else { SDL_SetRenderDrawColor(ren, 100,100,200,255); SDL_RenderFillRect(ren,&autoRect); }

        // draw clickAmount upgrade button
        if(upBtn.tex) SDL_RenderCopy(ren, upBtn.tex, nullptr, &upRect);
        else { SDL_SetRenderDrawColor(ren, 100,200,100,255); SDL_RenderFillRect(ren,&upRect); }

        // HUD text
        if(font){
            renderText(ren, font, "Score: " + std::to_string(score), 20, 20);
            renderText(ren, font, "Click per click: " + std::to_string(clickAmount), 20, 50);
            renderText(ren, font, "Autoclicks: " + std::to_string(autoclicks), 20, 80);
            renderText(ren, font, "Autoclick price: " + std::to_string(autoclickPrice), WIN_W - 210, 190);
            renderText(ren, font, "Upgrade price: " + std::to_string(clickPrice), WIN_W - 210, 310);
            renderText(ren, font, "Autoclick gain/sec: " + std::to_string(autoclicks * autoclickValue), 20, 110);
        }

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    // cleanup
    if(mainBtn.tex) SDL_DestroyTexture(mainBtn.tex);
    if(autoBtn.tex) SDL_DestroyTexture(autoBtn.tex);
    if(upBtn.tex) SDL_DestroyTexture(upBtn.tex);
    if(font) TTF_CloseFont(font);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
