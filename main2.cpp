#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstring>
#include <iostream>

const int SCREEN_WIDTH = 220;
const int SCREEN_HEIGHT = 240;
const int MENU_WIDTH = 200;
const int MENU_HEIGHT = 200;
const int MENU_PADDING = 10;
const int MENU_ITEM_HEIGHT = 40;

const char* MENU_ITEMS[] = {"Option 1", "Option 2", "Option 3", "Option 4", "Option 5"};

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;

SDL_Rect menuRect = {SCREEN_WIDTH - MENU_WIDTH - MENU_PADDING, MENU_PADDING, MENU_WIDTH, MENU_HEIGHT};

bool showMessage = false;
int selectedItem = -1;

void showMessageDialog(const char* message) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Message", message, window);
    showMessage = false;
}

void handleMenuItem(int index) {
    switch(index) {
        case 0:
            showMessage = true;
            showMessageDialog("You clicked Option 1!");
            break;
        case 1:
            showMessage = true;
            showMessageDialog("You clicked Option 2!");
            break;
        case 2:
            showMessage = true;
            showMessageDialog("You clicked Option 3!");
            break;
        case 3:
            showMessage = true;
            showMessageDialog("You clicked Option 4!");
            break;
        case 4:
            showMessage = true;
            showMessageDialog("You clicked Option 5!");
            break;
    }
}

void renderMenu() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &menuRect);

    for(int i = 0; i < 5; i++) {
        SDL_Rect itemRect = {SCREEN_WIDTH - MENU_WIDTH - MENU_PADDING + MENU_PADDING,
                             MENU_PADDING + i * MENU_ITEM_HEIGHT,
                             MENU_WIDTH - 2 * MENU_PADDING,
                             MENU_ITEM_HEIGHT};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &itemRect);

        if (selectedItem == i) {
            // Highlight the selected item
            SDL_Rect highlightRect = {SCREEN_WIDTH - MENU_WIDTH - MENU_PADDING + MENU_PADDING + 2,
                                      MENU_PADDING + i * MENU_ITEM_HEIGHT + 2,
                                      MENU_WIDTH - 2 * MENU_PADDING - 4,
                                      MENU_ITEM_HEIGHT - 4};
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            SDL_RenderFillRect(renderer, &highlightRect);
        }

        SDL_Color textColor = {0, 0, 0, 255};
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, MENU_ITEMS[i], textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {SCREEN_WIDTH - MENU_WIDTH + MENU_PADDING,
                             MENU_PADDING + i * MENU_ITEM_HEIGHT,
                             textSurface->w,
                             textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

    if(showMessage) {
        handleMenuItem(selectedItem);
    }
}

int main(int argc, char* argv[]) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    if(TTF_Init() < 0) {
        std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("Dynamic Menu",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH,
                              SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if(!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    font = TTF_OpenFont("ariblk.ttf", 24);
    if(!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return 1;
    }

    bool quit = false;
    SDL_Event e;

    while(!quit) {
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(e.button.button == SDL_BUTTON_LEFT) {
                        int x = e.button.x;
                        int y = e.button.y;
                        if(x >= SCREEN_WIDTH - MENU_WIDTH - MENU_PADDING + MENU_PADDING &&
                           x <= SCREEN_WIDTH - MENU_PADDING &&
                           y >= MENU_PADDING &&
                           y <= MENU_PADDING + 5 * MENU_ITEM_HEIGHT) {
                            selectedItem = (y - MENU_PADDING) / MENU_ITEM_HEIGHT;
                            showMessage = true;
                        }
                       break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        renderMenu();

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    font = NULL;

    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    TTF_Quit();
    SDL_Quit();

    return 0;
}