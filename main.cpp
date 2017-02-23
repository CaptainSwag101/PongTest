#include "main.h"

int main ( int argc, char** argv )
{
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    if ( !screen )
    {
        printf("Unable to set 800x600 video: %s\n", SDL_GetError());
        return 1;
    }

    // create the player paddles and ball
    player1.w = 32;
    player1.h = 128;
    player1.x = (screen->w - player1.w) / 8;
    player1.y = (screen->h - player1.h) / 2;

    player2.w = 32;
    player2.h = 128;
    player2.x = (screen->w - player2.w) / 8 * 7;
    player2.y = (screen->h - player2.h) / 2;

    ball.w = 32;
    ball.h = 32;
    ball.x = (screen->w - ball.w) / 2;
    ball.y = (screen->h - ball.h) / 2;

    float timeStepMs = 1000.f / 60; //60Hz
    float timeCurrentMs = 0.f, timeLastMs = 0.f, timeDeltaMs = 0.f, timeAccumulatedMs = 0.f;

    // program main loop
    while (!done)
    {
        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // check for messages
            switch (event.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                done = true;
                break;
            }
        }

        timeLastMs = timeCurrentMs;
        timeCurrentMs = SDL_GetTicks();
        timeDeltaMs = timeCurrentMs - timeLastMs;
        timeAccumulatedMs += timeDeltaMs;

        while (timeAccumulatedMs >= timeStepMs)
        {
              processInput();
              updateBall();
              timeAccumulatedMs -= timeStepMs;
        }

        // DRAWING STARTS HERE

        // clear screen
        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

        // draw players and ball
        SDL_FillRect(screen, &player1, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));

        SDL_FillRect(screen, &player2, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));

        SDL_FillRect(screen, &ball, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));

        // DRAWING ENDS HERE

        // finally, update the screen :)
        SDL_Flip(screen);
    } // end main loop

    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}

void processInput()
{
    Uint8 *keystate = SDL_GetKeyState(NULL);

    // check for keypresses
    if (keystate[SDLK_ESCAPE])
    {
        done = true;
        return;
    }

    if (keystate[SDLK_RETURN])
    {
        ball.w = 32;
        ball.h = 32;
        ball.x = (screen->w - ball.w) / 2;
        ball.y = (screen->h - ball.h) / 2;
    }

    if (keystate[SDLK_w] && (player1.y - screen->h / 90) >= 0)
    {
        player1.y -= screen->h / 90;
    }
    else if (keystate[SDLK_s] && (player1.y + player1.h + screen->h / 90) <= screen->h)
    {
        player1.y += screen->h / 90;
    }

    if (keystate[SDLK_i] && (player2.y - screen->h / 90) >= 0)
    {
        player2.y -= screen->h / 90;
    }
    else if (keystate[SDLK_k] && (player2.y + player2.h + screen->h / 90) <= screen->h)
    {
        player2.y += screen->h / 90;
    }
}

void updateBall()
{
    ball.x += ballSpeedX;
    ball.y += ballSpeedY;
    // we need to constantly reset this because otherwise the ball will shrink when it overlaps the top/bottom of the screen
    ball.h = 32;

    // check for collision with top/bottom of screen
    if (ball.y <= 0 || ball.y + ball.h >= screen->h)
    {
        ballSpeedY = -ballSpeedY;
    }

    // check for collision with player1's paddle
    if (ball.y + ball.h >= player1.y && ball.y <= player1.y + player1.h)
    {
        if (ball.x <= player1.x + player1.w && ball.x + ball.w >= player1.x)
        {
            if (ballSpeedX < 0)
            {
                ballSpeedX = -ballSpeedX;
                int oldBallSpeedX = ballSpeedX;
                int oldBallSpeedY = ballSpeedY;
                ballSpeedY += ((ball.y + (ball.h / 2)) - (player1.y + (player1.h / 2))) / 32;
                ballSpeedX += abs(oldBallSpeedY) - abs(ballSpeedY);

                if (ballSpeedX == 0)
                {
                    ballSpeedX = oldBallSpeedX;
                    ballSpeedY = oldBallSpeedY;
                }
            }
        }
    }

    // check for collision with player2's paddle
    if (ball.y + ball.h >= player2.y && ball.y <= player2.y + player2.h)
    {
        if (ball.x <= player2.x + player2.w && ball.x + ball.w >= player2.x)
        {
            if (ballSpeedX > 0)
            {
                ballSpeedX = -ballSpeedX;
                int oldBallSpeedX = ballSpeedX;
                int oldBallSpeedY = ballSpeedY;
                ballSpeedY += ((ball.y + (ball.h / 2)) - (player2.y + (player2.h / 2))) / 32;
                ballSpeedX -= abs(oldBallSpeedY) - abs(ballSpeedY);

                if (ballSpeedX == 0)
                {
                    ballSpeedX = oldBallSpeedX;
                    ballSpeedY = oldBallSpeedY;
                }
            }
        }
    }
}
