#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h> 

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 400

typedef struct 
{
    SDL_Rect rect ;
    int xState ; 
    int yState ; 
}shape;


void renderRect(SDL_Renderer *renderer , SDL_Rect rect)
{
    SDL_SetRenderDrawColor(renderer , 51,255,255,255);
    SDL_RenderFillRect(renderer , &rect);
}
void detectHit(shape *square)
{
    if ((square->rect.x - 10 >= 0 && square->rect.x + square->rect.w + 10 < WINDOW_WIDTH ) && (square->rect.y - 10 >= 0  && square->rect.y + square->rect.h + 10 < WINDOW_HEIGHT))
    return ;
   // printf("square->rect.x : %d square->rect.y : %d \n", square->rect.x , square->rect.y);
    if (square->rect.x - 10<=0)
    square->xState = 1 ;
    if (square->rect.x + square->rect.w + 10> WINDOW_WIDTH)
    square->xState = 0;
    if (square->rect.y - 10<=0)
    square->yState = 1 ;
    if (square->rect.y + square->rect.h + 10> WINDOW_HEIGHT)
    square->yState = 0;
}
void moveRect(shape *square)
{

    int k = 10 ;

    switch (square->xState)
    {
    case 1:
        square->rect.x+=k;
        break;
    
    default:
        square->rect.x-=k;
        break;
    }
    switch (square->yState)
    {
    case 1:
        square->rect.y+=k;
        break;
    
    default:
        square->rect.y-=k;
        break;
    }

}


int main(int argc, char* argv[]) {
    srand(time(0));
    shape square  ;
    square.rect.h = square.rect.w  = 20 ;
    square.rect.x = WINDOW_WIDTH / 2 ;
    square.rect.y = WINDOW_HEIGHT / 2 ;
    square.xState = 1 ;
    square.yState = 0 ;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }


    SDL_Window* window = SDL_CreateWindow("snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,WINDOW_HEIGHT , SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED) ;
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    bool running = true ;
    SDL_Event event ;


    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false ;
                break;
            }
            else if(event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    running=false ;
                    break;


                }

            }
        }

        SDL_SetRenderDrawColor(renderer , 0,0,0,255) ;
        SDL_RenderClear(renderer);

        // start
        renderRect(renderer, (square.rect));
        moveRect(&square);
        detectHit(&square);

        // end

        SDL_RenderPresent(renderer) ;

        SDL_Delay(100) ;

    }


    SDL_DestroyRenderer(renderer) ;
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}