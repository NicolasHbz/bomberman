#include "constants.h"
#include "game.h"

void menu()
{
    SDL_Surface *screen = NULL, *menu = NULL;
    SDL_Rect positionMenu;
    SDL_Event event;

    int playing = 1;

    SDL_Init(SDL_INIT_VIDEO);

    screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Bomberman", NULL);
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
    {
        printf("%s", Mix_GetError());
    }
    Mix_AllocateChannels(32);
    Mix_Music *music;
    Mix_Chunk *start_sound = NULL;
    start_sound = Mix_LoadWAV("assets/son/start.wav");
    music = Mix_LoadMUS("assets/son/game.mp3");
    Mix_PlayMusic(music, -1);
    menu = IMG_Load("assets/images/menu.png");
    positionMenu.x = 0;
    positionMenu.y = 0;

    while (playing)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                playing = 0;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        playing = 0;
                        break;
                    case SDLK_j: // Join
                        Mix_PlayChannel(0, start_sound, 0);
                        game(screen);
                        break;
                    case SDLK_h: // Host
                        game(screen);
                        break;
                    case SDLK_q: //Quit
                        playing = 0;
                        break;
                }
                break;
        }
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        SDL_BlitSurface(menu, NULL, screen, &positionMenu);
        SDL_Flip(screen);
    }
    Mix_CloseAudio();
    SDL_Quit();
}
