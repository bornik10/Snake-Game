#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "inc/SDL.h"
#include "inc/SDL_ttf.h"
#include "inc/SDL_mixer.h"
#include "inc/SDL_image.h"
#undef main
// SDL_Renderer* renderer;
Mix_Music* bgm;
using namespace std;

void startscreen(SDL_Renderer* renderer)
{
    IMG_Init(IMG_INIT_JPG);
    SDL_RenderClear(renderer);
    SDL_Surface* imgS = IMG_Load("game start.jpg");
    if(imgS == NULL) cout << SDL_GetError() << endl;
    SDL_Texture* imgTex = SDL_CreateTextureFromSurface(renderer, imgS);
    if(imgTex == NULL) cout << SDL_GetError() << endl;
    SDL_RenderCopy(renderer, imgTex, NULL, NULL);
    SDL_RenderPresent(renderer);
}
/*void overscreen(SDL_Renderer* renderer)
{
    IMG_Init(IMG_INIT_JPG);
    SDL_RenderClear(renderer);
    SDL_Surface* imgS = IMG_Load("game over.jpg");
    if(imgS == NULL) cout << SDL_GetError() << endl;
    SDL_Texture* imgTex = SDL_CreateTextureFromSurface(renderer, imgS);
    if(imgTex == NULL) cout << SDL_GetError() << endl;
    SDL_RenderCopy(renderer, imgTex, NULL, NULL);
    SDL_RenderPresent(renderer);
}*/

void renderPlayer(SDL_Renderer* renderer, SDL_Rect player, int x, int y, int scale, vector<int> tailX, vector<int> tailY, int tailLength) {
    
    player.w = scale;
    player.h = scale;
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
       // SDL_RenderFillRect(renderer, &player);
    // Gets x and y of all tail blocks and renders them
    for (int i = 0; i < tailLength; i++) {
        if(i==tailLength-1)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
            player.x = tailX[i];
            player.y = tailY[i];
            SDL_RenderFillRect(renderer, &player);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
            player.x = tailX[i];
            player.y = tailY[i];
            SDL_RenderFillRect(renderer, &player);
        }
        
    }

    player.x = x;
    player.y = y;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &player);

    
}

void renderFood(SDL_Renderer* renderer, SDL_Rect food) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &food);
}

void renderB(SDL_Renderer* renderer, SDL_Rect b) {
    SDL_SetRenderDrawColor(renderer, 0, 25, 200, 255);
    SDL_RenderFillRect(renderer, &b);
}

void renderScore(SDL_Renderer* renderer, int tailLength, int scale, int wScale) {
    SDL_Color White = { 255, 255, 255 };

    // Get the font used for displaying text
    TTF_Font* font = TTF_OpenFont("OpenSans-SemiboldItalic.ttf", 10);
    if (font == NULL) {
        cout << "Font loading error: " << TTF_GetError() << endl;
        return;
    }

    SDL_Surface* score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(tailLength * 1)).c_str(), White);
    SDL_Texture* scoreMessage = SDL_CreateTextureFromSurface(renderer, score);
    SDL_Rect scoreRect;
    scoreRect.w = 100;
    scoreRect.h = 30;
    scoreRect.x = ((scale*wScale) / 2) - (scoreRect.w / 2);
    scoreRect.y = 0;
    SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);

    TTF_CloseFont(font);
}

bool checkCollision(int foodx, int foody, int playerx, int playery) {
    return (playerx == foodx && playery == foody);
}

pair<int, int> getFoodSpawn(vector<int> tailX, vector<int> tailY, int playerX, int playerY, int scale, int wScale, int tailLength) {
    bool valid = false;
    int x = 0;
    int y = 0;
    srand(time(0));
    x = scale * (rand() % wScale);
    y = scale * (rand() % wScale);
    valid = true;

    // Check all tail blocks and player block
    for (int i = 0; i < tailLength; i++) {
        if ((x == tailX[i] && y == tailY[i])||(x == playerX && y == playerY)) {
            valid = false;
        }
    }

    if((x == 72 && y == 72)||(x == 96 && y == 72)|| (x == 120 && y == 72)||
    (x == 144 && y == 72)||(x == 72 && y == 96)||(x == 72 && y == 120)||
    (x == 72 && y == 144)||(x == 480 && y == 72)||(x == 456 && y == 72)||
    (x == 432 && y == 72)||(x == 408 && y == 72)||(x == 480 && y == 96)||
    (x == 480 && y == 120)||(x == 480 && y == 144)||(x == 72 && y == 480)||
    (x == 96 && y == 480)||(x == 120 && y == 480)||(x == 144 && y == 480)||
    (x == 72 && y == 456)||(x == 72 && y == 432)||(x == 72 && y == 408)||
    (x == 480 && y == 480)||(x == 456 && y == 480)||(x == 432 && y == 480)||
    (x == 408 && y == 480)||(x == 480 && y == 456)||(x == 480 && y == 432)||(x == 480 && y == 408))
    {
        valid=false;
    }

    if (!valid) {
        pair<int, int> foodLoc;
        foodLoc = make_pair(-100, -100);
        return foodLoc;
    }

    pair<int, int> foodLoc;
    foodLoc = make_pair(x, y);

    return foodLoc;
}

pair<int, int> bSpawn(vector<int> tailX, vector<int> tailY, int playerX, int playerY, int scale, int wScale, int tailLength) {
    bool bvalid = false;
    int x = 0;
    int y = 0;
    srand(time(0));
    x = scale * (rand() % wScale);
    y = scale * (rand() % wScale);
    bvalid = true;

    // Check all tail blocks and player block
    for (int i = 0; i < tailLength; i++) {
        if ((x == tailX[i] && y == tailY[i])||(x == playerX && y == playerY)) {
            bvalid = false;
        }
    }

    if((x == 72 && y == 72)||(x == 96 && y == 72)|| (x == 120 && y == 72)||
    (x == 144 && y == 72)||(x == 72 && y == 96)||(x == 72 && y == 120)||
    (x == 72 && y == 144)||(x == 480 && y == 72)||(x == 456 && y == 72)||
    (x == 432 && y == 72)||(x == 408 && y == 72)||(x == 480 && y == 96)||
    (x == 480 && y == 120)||(x == 480 && y == 144)||(x == 72 && y == 480)||
    (x == 96 && y == 480)||(x == 120 && y == 480)||(x == 144 && y == 480)||
    (x == 72 && y == 456)||(x == 72 && y == 432)||(x == 72 && y == 408)||
    (x == 480 && y == 480)||(x == 456 && y == 480)||(x == 432 && y == 480)||
    (x == 408 && y == 480)||(x == 480 && y == 456)||(x == 480 && y == 432)||(x == 480 && y == 408))
    {
        bvalid=false;
    }
    pair<int, int> bLoc;
    if (!bvalid) {
        bLoc = make_pair(-100, -100);
        return bLoc;
    }

    //pair<int, int> foodLoc;
    bLoc = make_pair(x, y);

    return bLoc;
}

void gameOver(SDL_Renderer* renderer, SDL_Event event, int scale, int wScale, int tailLength) {
    SDL_Color Red = { 255, 0, 0 };
    SDL_Color White = { 255, 255, 255 };
    SDL_Color Black = { 0, 0, 0 };

    // Get the font used for displaying text
    TTF_Font* font = TTF_OpenFont("arial.ttf", 10);
    if (font == NULL) {
        cout << "Font loading error: " << TTF_GetError() << endl;
        return;
    }
    /*overscreen(renderer);
    SDL_Delay(5000);*/

    SDL_Surface* gameover = TTF_RenderText_Solid(font, "Game Over", Red);
    SDL_Surface* retry = TTF_RenderText_Solid(font, "Press Enter to retry", White);
    SDL_Surface* score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(tailLength * 1)).c_str(), Black);
    SDL_Texture* gameoverMessage = SDL_CreateTextureFromSurface(renderer, gameover);
    SDL_Texture* retryMessage = SDL_CreateTextureFromSurface(renderer, retry);
    SDL_Texture* scoreMessage = SDL_CreateTextureFromSurface(renderer, score);
    SDL_Rect gameoverRect;
    SDL_Rect retryRect;
    SDL_Rect scoreRect;
    gameoverRect.w = 200;
    gameoverRect.h = 100;
    gameoverRect.x = ((scale*wScale) / 2)-(gameoverRect.w/2);
    gameoverRect.y = ((scale*wScale) / 2)-(gameoverRect.h/2)-50;
    retryRect.w = 300;
    retryRect.h = 50;
    retryRect.x = ((scale*wScale) / 2) - ((retryRect.w / 2));
    retryRect.y = (((scale*wScale) / 2) - ((retryRect.h / 2))+150);
    scoreRect.w = 100;
    scoreRect.h = 30;
    scoreRect.x = ((scale*wScale) / 2) - (scoreRect.w / 2);
    scoreRect.y = 0;
    SDL_RenderCopy(renderer, gameoverMessage, NULL, &gameoverRect);
    SDL_RenderCopy(renderer, retryMessage, NULL, &retryRect);
    SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);

    TTF_CloseFont(font);

    // Show game over screen while space has not been pressed
    while (true) {
        SDL_RenderPresent(renderer);

        if (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                exit(0);
            }

            if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                return;
            }

        }

    }
}

int main(int argc, char* argv[]) {
    // Init everything
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "SDL initialization error: " << SDL_GetError() << endl;
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        cout << "SDL_ttf initialization error: " << TTF_GetError() << endl;
        return 1;
    }

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;

    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 1024);
    bgm = Mix_LoadMUS("heda.mp3");
    Mix_PlayMusic(bgm, -1);

    int fCount=0;

    //Player rectangle
    SDL_Rect player;
    player.x = 0;
    player.y = 0;
    player.h = 0;
    player.w = 0;

    // tailLength is incremented if the snake eats food
    int tailLength = 0;

    // Vectors for storage of tail block positions
    vector<int> tailX;
    vector<int> tailY;

    // Size of tiles
    int scale = 24;
    int wScale = 24;

    // Player position variables
    int x = 0;
    int y = 0;
    int prevX = 0;
    int prevY = 0;

    // Movement controls
    bool up = false;
    bool down = false;
    bool right = false;
    bool left = false;

    bool inputThisFrame = false;
    bool redo = false;
    bool bredo = false;

    // Food rectangle
    SDL_Rect food;
    food.w = scale;
    food.h = scale;
    food.x = 0;
    food.y = 0;

    SDL_Rect b;
    b.w = scale;
    b.h = scale;
    b.x = -20;
    b.y = -20;

    pair<int, int> foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
    food.x = foodLoc.first;
    food.y = foodLoc.second;

    pair<int, int> bLoc;

    // Show the window
    window = SDL_CreateWindow("RUSSIAN NAGIN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, scale * wScale , scale * wScale , SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    startscreen(renderer);
    SDL_Delay(4000);

    float time = SDL_GetTicks() / 100;

    // Game loop
    while (true) {
        float newTime = SDL_GetTicks() / 100; 
        float delta = newTime - time;
        time = newTime;

        inputThisFrame = false;

        // Controls
        if (SDL_PollEvent(&event)) {
            // Simply exit the program when told to
            if (event.type == SDL_QUIT) {
                exit(0);
            }

            // If a key is pressed
            if (event.type == SDL_KEYDOWN && inputThisFrame == false) {
                if (down == false && event.key.keysym.scancode == SDL_SCANCODE_UP) {
                    up = true;
                    left = false;
                    right = false;
                    down = false;
                    inputThisFrame = true;
                } else if (right == false && event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
                    up = false;
                    left = true;
                    right = false;
                    down = false;
                    inputThisFrame = true;
                } else if (up == false && event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
                    up = false;
                    left = false;
                    right = false;
                    down = true;
                    inputThisFrame = true;
                } else if (left == false && event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
                    up = false;
                    left = false;
                    right = true;
                    down = false;
                    inputThisFrame = true;
                }
            }
        }

        //previous position of the player block
        prevX = x;
        prevY = y;

        if (up) {
            y -= delta * scale;
        } else if (left) {
            x -= delta * scale;
        } else if (right) {
            x += delta * scale;
        } else if (down) {
            y += delta * scale;
        }

        if (redo == true) {
            redo = false;
            foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
            food.x = foodLoc.first;
            food.y = foodLoc.second;

            if (food.x == -100 && food.y == -100) {
                redo = true;
            }
        }

        if (bredo == true) {
            bredo = false;
            bLoc = bSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
            b.x = bLoc.first;
            b.y = bLoc.second;

            if (b.x == -100 && b.y == -100) {
                bredo = true;
            }
        }


        // Collision detection with food
        if (checkCollision(food.x, food.y, x, y)) {
            // Spawn new food
            foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
            food.x = foodLoc.first;
            food.y = foodLoc.second;

            if (food.x == -100 && food.y == -100) {
                redo = true;
            }

            tailLength++;
            fCount++;
        }

        if (checkCollision(b.x, b.y, x, y)) {
            // Spawn new food
            //foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
            b.x = -20;
            b.y = -20;

            tailLength++;
        }

        if (fCount==3){
            bLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
            b.x = bLoc.first;
            b.y = bLoc.second;

            if (b.x == -100 && b.y == -100) {
                bredo = true;
            }
            fCount=0;
        }

        cout << food.x << " " << food.y << endl;
        cout << b.x << " " << b.y << endl;

        // Only runs in the frames
        if (delta * scale == 24) {
            // Update tail size and position
            if (tailX.size() != tailLength) {
                tailX.push_back(prevX);
                tailY.push_back(prevY);
            }

            // Loop through every tail block, move all blocks to the nearest block in front
            // This updates the blocks from end (farthest from player block) to the start (nearest to player block)
            for (int i = 0; i < tailLength; i++) {
                if (i > 0) {
                    tailX[i - 1] = tailX[i];
                    tailY[i - 1] = tailY[i];
                }
            }

            if (tailLength > 0) {
                tailX[tailLength - 1] = prevX;
                tailY[tailLength - 1] = prevY;
            }
        }

        //if snake has collided with a tail block, game over
        for (int i = 0; i < tailLength; i++) {
            if (x == tailX[i] && y == tailY[i]) {
                cout << "collided with a tail block." << endl;
                gameOver(renderer, event, scale, wScale, tailLength);
                x = 0;
                y = 0;
                up = false;
                left = false;
                right = false;
                down = false;
                tailX.clear();
                tailY.clear();
                tailLength = 0;
                redo = false;

                foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
                if (food.x == -100 && food.y == -100) {
                    redo = true;
                }

                food.x = foodLoc.first;
                food.y = foodLoc.second;

                b.x = -20;
                b.y = -20;
            }
        }

        if(x>scale*wScale-scale)
        {
            x=0;
        }
        if(y>scale*wScale-scale)
        {
            y=0;
        }
        if(x<0)
        {
            x=scale*wScale-scale;
        }
        if(y<0)
        {
            y=scale*wScale-scale;
        }
        // Game over if snake collide with the walls
        if ((x == 72 && y == 72)||(x == 96 && y == 72)||
        (x == 120 && y == 72)||(x == 144 && y == 72)||
        (x == 72 && y == 96)||(x == 72 && y == 120)||
        (x == 72 && y == 144)||(x == 480 && y == 72)||
        (x == 456 && y == 72)||(x == 432 && y == 72)||
        (x == 408 && y == 72)||(x == 480 && y == 96)||
        (x == 480 && y == 120)||(x == 480 && y == 144)||
        (x == 72 && y == 480)||(x == 96 && y == 480)||
        (x == 120 && y == 480)||(x == 144 && y == 480)||
        (x == 72 && y == 456)||(x == 72 && y == 432)||
        (x == 72 && y == 408)||(x == 480 && y == 480)||
        (x == 456 && y == 480)||(x == 432 && y == 480)||
        (x == 408 && y == 480)||(x == 480 && y == 456)||
        (x == 480 && y == 432)||(x == 480 && y == 408)) {    
            gameOver(renderer, event, scale, wScale, tailLength);
            cout << "snake collide with the walls" << endl;
            x = 0;
            y = 0;
            up = false;
            left = false;
            right = false;
            down = false;
            tailX.clear();
            tailY.clear();
            tailLength = 0;
            redo = false;
            foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
            food.x = foodLoc.first;
            food.y = foodLoc.second;

            b.x = -20;
            b.y = -20;

            if (food.x == -100 && food.y == -100) {
                redo = true;
            }
        }

        //red blocks for showing walls
        //wall 1
        SDL_Rect redBlock;
        redBlock.w = 96;
        redBlock.h = 24;
        redBlock.x = 72;
        redBlock.y = 72;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
        SDL_RenderFillRect(renderer, &redBlock);

        redBlock.w = 24;
        redBlock.h = 72;
        redBlock.x = 72;
        redBlock.y = 96; 
        SDL_RenderFillRect(renderer, &redBlock);

        //wall 2
        redBlock.w = 96;
        redBlock.h = 24;
        redBlock.x = 408;
        redBlock.y = 72;
        SDL_RenderFillRect(renderer, &redBlock);
        
        redBlock.w = 24;
        redBlock.h = 72;
        redBlock.x = 480;
        redBlock.y = 96;
        SDL_RenderFillRect(renderer, &redBlock);

        //wall 3
        redBlock.w = 96;
        redBlock.h = 24;
        redBlock.x = 72;
        redBlock.y = 480;
        SDL_RenderFillRect(renderer, &redBlock);

        redBlock.w = 24;
        redBlock.h = 72;
        redBlock.x = 72;
        redBlock.y = 408;
        SDL_RenderFillRect(renderer, &redBlock);

        
        //wall 4
        redBlock.w = 96;
        redBlock.h = 24;
        redBlock.x = 408;
        redBlock.y = 480; 
        SDL_RenderFillRect(renderer, &redBlock);

        redBlock.w = 24;
        redBlock.h = 72;
        redBlock.x = 480;
        redBlock.y = 408; 
        SDL_RenderFillRect(renderer, &redBlock);

        // Render food, player, score, and window border
        renderFood(renderer, food);
        renderB(renderer, b);
        renderPlayer(renderer, player, x, y, scale, tailX, tailY, tailLength);
        renderScore(renderer, tailLength, scale, wScale);

        // Put everything on screen
        SDL_RenderPresent(renderer);

        // Choose a color to fill the window
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderClear(renderer);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
