#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <SDL.h>

// method declarations
void processInput();
void updateBall();
int main(int argc, char** argv);

// variable declarations
bool done = false;
SDL_Window *screen;
SDL_Renderer *sdlRenderer;
SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &screen, &sdlRenderer);

SDL_Rect player1;
SDL_Rect player2;
SDL_Rect ball;

int ballSpeedX = (screen-> / 90);
int ballSpeedY = 0;

#endif // MAIN_H_INCLUDED
