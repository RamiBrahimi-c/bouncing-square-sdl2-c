#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h> 

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 400
#define Row 3
#define Col 10

typedef struct 
{
    SDL_Rect rect ;
    int xState ; 
    int yState ; 
    int move ;
}shape;

typedef struct 
{
    SDL_Rect rect ;
    int state ;
}Block;

typedef struct 
{
    int x ;
    int y;
    int w ;
    int h ;
}Position;


void renderRect(SDL_Renderer *renderer , SDL_Rect rect)
{
    SDL_SetRenderDrawColor(renderer , 51,255,255,255);
    SDL_RenderFillRect(renderer , &rect);
}
void detectHit(shape *square ,SDL_Rect rect )
{
    if ((square->rect.x - 10 >= 0 && square->rect.x + square->rect.w + 10 < WINDOW_WIDTH ) && (square->rect.y - 10 >= 0  && square->rect.y + square->rect.h + 10 < WINDOW_HEIGHT))
    return ;
   // printf("square->rect.x : %d square->rect.y : %d \n", square->rect.x , square->rect.y);
    if (square->rect.x - 10<=0)  
    square->xState = 1 ;
    if (square->rect.x + square->rect.w + 10> WINDOW_WIDTH)
    square->xState = 0 ;
    if (square->rect.y - 10<=0)
    square->yState = 1 ;
    if (square->rect.y + square->rect.h > WINDOW_HEIGHT)
    {
        square->xState = 1;
        square->yState = 0;
        square->rect.x = (2 * rect.x + rect.w) / 2 ;
        square->rect.y = rect.y - square->rect.h ;
        square->move = 0 ;
    }

}
void moveRect(shape *square )
{
    if (square->move == 0)
    return;
    
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

void renderBase(SDL_Renderer *renderer , SDL_Rect rect)
{
    SDL_SetRenderDrawColor(renderer , 255 , 255,255,255);
    SDL_RenderFillRect(renderer , &rect);
}

void detectCollisionWithBase(shape *square ,SDL_Rect rect )
{
    int radius = square->rect.x * square->rect.x + square->rect.y * square->rect.y;
    if(( square->rect.y + square->rect.h == rect.y && ((square->rect.x  >= rect.x && square->rect.x  <= rect.x + rect.w) || (square->rect.x + square->rect.w >= rect.x && square->rect.x + square->rect.w  <= rect.x + rect.w) ) )  )
    {
       switch (square->yState)
       {
       case 1:
        square->yState = 0;
        break;
       
       default:
        square->yState = 1;       
        break;
       }        
    }
}

void initilize( int block[Row][Col])
{
    for (int i = 0; i < Row; i++)
    {
        for (int j = 0; j < Col; j++)
        {
            block[i][j]=1;
        }
        
    }
    
}

void renderBlocks(SDL_Renderer *renderer , int block[Row][Col] )
{
                SDL_SetRenderDrawColor(renderer , 255 , 0 , 0 , 255 );
    int cellsize=60;
    SDL_Rect rect ;
    rect.w = cellsize-30 ;
    rect.h = cellsize-30 ;
    for (int i = 0; i < Row; i++)
    {
        for (int j = 0; j < Col; j++)
        {
            if (block[i][j] > 0 )
            {
                rect.x = 20 + cellsize * j;
                rect.y = 2 + cellsize * i;
                SDL_RenderFillRect(renderer , &rect);
            }
        }
        
    }
    
}
void detectCrash(int block[Row][Col] , shape *square)
{
    for (int i = 0; i < Row; i++)
    {
        for (int j = 0; j < Col; j++)
        {
            // crash from down the block
            if ((square->rect.x >= 20 + 60 * j && square->rect.x <= 20 + 60 * j + 30) &&  (square->rect.y  == 2 + 60 * i +30  )  && block[i][j]>0)
            {
                block[i][j]=0;
                switch (square->yState)
                {
                case 1:
                    square->yState = 0;
                    break;
                
                default:
                    square->yState = 1;       
                    break;
                }
 
            }
            // crash from left the block
            if ((square->rect.x + square->rect.w == 20 + 60 * j ) &&  (square->rect.y  <= 2 + 60 * i +30  && square->rect.y  >= 2 + 60 * i  )  && block[i][j]>0)
            {
                block[i][j]=0;
                switch (square->xState)
                {
                case 1:
                    square->xState = 0;
                    break;
                
                default:
                    square->xState = 1;       
                    break;
                }
            }
            // crash from upside the block
            if ((square->rect.x  >= 20 + 60 * j && square->rect.x  <= 20 + 60 * j + 30) &&  ( square->rect.y + square->rect.h  == 2 + 60 * i  )  && block[i][j]>0)
            {
                block[i][j]=0;
                switch (square->yState)
                {
                case 1:
                    square->yState = 0;
                    break;
                
                default:
                    square->yState = 1;       
                    break;
                }
            }
            // crash from the right of the block
            if (( square->rect.x  == 20 + 60 * j + 30) &&  (square->rect.y  <= 2 + 60 * i +30  && square->rect.y  >= 2 + 60 * i  )  && block[i][j]>0)
            {
                block[i][j]=0;
                switch (square->xState)
                {
                case 1:
                    square->xState = 0;
                    break;
                
                default:
                    square->xState = 1;       
                    break;
                }
            }

            
        }
        
    }
    
}

void initilizeMatrixPosition(Position matrix[Row][Col])
{
    for (int i = 0; i < Row; i++)
    {
        for (int j = 0; j < Col; j++)
        {
            matrix[i][j].w = 30 ;
            matrix[i][j].h = 30 ;
            matrix[i][j].x = 20 + 60 * j ;
            matrix[i][j].y = 2 + 60 * i ;

        }
        
    }
    
}

void detectCrashA(int block[Row][Col] , shape *square , Position matrix[Row][Col])
{
    for (int i = 0; i < Row; i++)
    {
        for (int j = 0; j < Col; j++)
        {
            // crash from down the block
            if (((square->rect.x <= matrix[i][j].x  + matrix[i][j].w && square->rect.x >= matrix[i][j].x ) || (square->rect.x + square->rect.w <= matrix[i][j].x  + matrix[i][j].w && square->rect.x + square->rect.w >= matrix[i][j].x) ) &&  ( square->rect.y <=   matrix[i][j].y + matrix[i][j].h && square->rect.y >= matrix[i][j].y    )  && block[i][j]>0)
            {
                block[i][j]=0;
                switch (square->yState)
                {
                case 1:
                    square->yState = 0;
                    break;
                
                default:
                    square->yState = 1;       
                    break;
                }
 
            }
            // crash from up the block
            int radius = square->rect.x*square->rect.x + square->rect.y * square->rect.y ;
             if (((square->rect.x <= matrix[i][j].x  + matrix[i][j].w && square->rect.x >= matrix[i][j].x ) || (square->rect.x + square->rect.w <= matrix[i][j].x + matrix[i][j].w && square->rect.x + square->rect.w >= matrix[i][j].x )  ) &&  (( square->rect.y + square->rect.h <=   matrix[i][j].y + matrix[i][j].h && square->rect.y + square->rect.h>= matrix[i][j].y    ) )  && block[i][j]>0)
            {
                block[i][j]=0;
                switch (square->yState)
                {
                case 1:
                    square->yState = 0;
                    break;
                
                default:
                    square->yState = 1;       
                    break;
                }
                printf("up\n");
            } 
            // crash from left the block
             if ((square->rect.x + square->rect.w <= matrix[i][j].x  + matrix[i][j].w && square->rect.x + square->rect.w >= matrix[i][j].x   ) &&  (( square->rect.y <=   matrix[i][j].y + matrix[i][j].h && square->rect.y >= matrix[i][j].y) || (square->rect.y + square->rect.h <=   matrix[i][j].y + matrix[i][j].h && square->rect.y + square->rect.h >= matrix[i][j].y)    )  && block[i][j]>0)
            {
                block[i][j]=0;
                switch (square->xState)
                {
                case 1:
                    square->xState = 0;
                    break;
                
                default:
                    square->xState = 1;       
                    break;
                }
                printf("left\n");
            }
            // crash from right the block
             
             if ((square->rect.x  <= matrix[i][j].x  + matrix[i][j].w && square->rect.x >= matrix[i][j].x   ) &&  ( square->rect.y <=   matrix[i][j].y + matrix[i][j].h && square->rect.y >= matrix[i][j].y    )  && block[i][j]>0)
            {
                block[i][j]=0;
                switch (square->xState)
                {
                case 1:
                    square->xState = 0;
                    break;
                
                default:
                    square->xState = 1;       
                    break;
                }
                printf("right\n");
            }
            
        }
        
    }
    
}

void printMatrixPos(Position matrix[Row][Col])
{
    for (int i = 0; i < Row; i++)
    {
        for (int j = 0; j < Col; j++)
        {
            printf("matrix[%d][%d].x=%d , matrix[%d][%d].y=%d \t",i , j ,matrix[i][j].x , i , j,matrix[i][j].y );
        }
        printf("\n");
    }
    
}


int main(int argc, char* argv[]) {
    srand(time(0));
    SDL_Rect rect ;
    rect.w = 100 ;
    rect.h = 10 ;
    rect.x = 0 ;
    rect.y = WINDOW_HEIGHT - rect.h ;
    
    
    shape square   ;
    square.rect.h = square.rect.w  = 20 ;
    square.rect.x = (2 * rect.x + rect.w) / 2 ;
    square.rect.y = rect.y - square.rect.h ;
    square.xState = 1 ;
    square.yState = 0 ;
    square.move= 0 ;


    int block[Row][Col] ;
    initilize(block);

    Position matrix[Row][Col] ;
    initilizeMatrixPosition(matrix) ;
    // printMatrixPos(matrix);
   // printf("\n%d" , matrix[0][0].y);
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }


    SDL_Window* window = SDL_CreateWindow("breaking breaks", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,WINDOW_HEIGHT , SDL_WINDOW_SHOWN);
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
                    case SDLK_RIGHT:
                    if(rect.x  + rect.w < WINDOW_WIDTH )
                    {
                        rect.x+=50 ;
                        if (square.move == 0)
                        {
                            square.rect.x = (2 * rect.x + rect.w) / 2 ;
                            square.rect.y = rect.y - square.rect.h ;
                        }
                    }

                    break;
                    case SDLK_LEFT:
                    if(rect.x   > 0 )
                    {
                        rect.x-=50 ;
                        if (square.move == 0)
                        {
                            square.rect.x = (2 * rect.x + rect.w) / 2 ;
                            square.rect.y = rect.y - square.rect.h ;
                        }                        
                    }                    
                    break;
                    case SDLK_a:
                    square.move = 1 ;
                    printf("yes\n");
                    break;


                }

            }
        }

        SDL_SetRenderDrawColor(renderer , 0,0,0,255) ;
        SDL_RenderClear(renderer);

        // start
        //detectCrashA(block , &square , matrix);
        moveRect(&square );
        detectHit(&square , rect);
        detectCollisionWithBase(&square, rect);
        detectCrash(block , &square);



        renderRect(renderer, (square.rect));
        renderBase(renderer , rect) ;
        renderBlocks(renderer , block) ;

        // end

        SDL_RenderPresent(renderer) ;

        SDL_Delay(100) ;

    }


    SDL_DestroyRenderer(renderer) ;
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}