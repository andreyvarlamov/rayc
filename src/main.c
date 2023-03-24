#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#define internal static
#define local_persist static
#define global_variable static

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int32 bool32;

typedef struct backbuffer
{
    void *Data;

    int Width;
    int Height;

    int BytesPerPixel;
    int Pitch;
} backbuffer;

typedef struct game_data
{
    int BlueOffset;
    int GreenOffset;
} game_data;

global_variable bool32 Running = false;

internal void
UpdateAndRender(backbuffer *Backbuffer, game_data *GameData)
{
    uint8 *Row = (uint8 *)Backbuffer->Data;
    for (int Y = 0;
            Y < Backbuffer->Height;
            ++Y)
    {
        uint32 *Pixel = (uint32 *)Row;
        for (int X = 0;
                X < Backbuffer->Width;
                ++X)
        {
            uint8 Blue = X + GameData->BlueOffset;
            uint8 Green = Y + GameData->GreenOffset;

            *Pixel++ = ((Green << 8) | Blue);
        }

        Row += Backbuffer->Pitch;
    }

    ++(GameData->BlueOffset);
    ++(GameData->GreenOffset);
}

void
HandleEvent(SDL_Event *Event)
{
    switch(Event->type)
    {
        case SDL_QUIT:
        {
            Running = false;
        } break;
    }
}

int
main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize the SDL2 library\n");
        return -1;
    }

    SDL_Window *Window = SDL_CreateWindow("rayc",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          1600, 900,
                                          0);

    if (Window)
    {
        SDL_Renderer *Renderer = SDL_CreateRenderer(Window, -1, 0);

        if (Renderer)
        {
            backbuffer Backbuffer = {0};
            Backbuffer.BytesPerPixel = 4;
            Backbuffer.Width = 1600;
            Backbuffer.Height = 900;
            Backbuffer.Pitch = Backbuffer.Width * Backbuffer.BytesPerPixel;
            Backbuffer.Data = malloc(Backbuffer.Width * Backbuffer.Height * Backbuffer.BytesPerPixel);

            SDL_Texture *Texture = SDL_CreateTexture(Renderer,
                                                    SDL_PIXELFORMAT_ARGB8888,
                                                    SDL_TEXTUREACCESS_STREAMING,
                                                    Backbuffer.Width, Backbuffer.Height);

            game_data GameData = {0};

            Running = true;
            while(Running)
            {
                SDL_Event Event;
                while (SDL_PollEvent(&Event))
                {
                    HandleEvent(&Event);
                }

                UpdateAndRender(&Backbuffer, &GameData);

                SDL_UpdateTexture(Texture, 0, Backbuffer.Data, Backbuffer.Pitch);

                SDL_RenderCopy(Renderer, Texture, 0, 0);

                SDL_RenderPresent(Renderer);
            }
        }
        else
        {
            // TODO: Logging
        }
    }
    else
    {
        // TODO: Logging
    }

    SDL_Quit();
    return 0;
}
