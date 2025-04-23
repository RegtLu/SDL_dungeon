#include "SDL.h"
#include <map>
#include <string>
#include <vector>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;

int *frame;
SDL_Window *SDLWindow;
SDL_Renderer *SDLRenderer;
SDL_Texture *SDLTexture;
static int Exit;
const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 540;

int tile_width,tile_height,tile_channel;
int *gTexture = (int*)stbi_load("tiles.png", &tile_width, &tile_height, &tile_channel, 4);
int tile_size=32;
vector<string> sprite_names = {
  "house1", "house2", "house3", "house4"
};
map<string,pair<int,int>> sprite;

void init_sprite(){ // size=width=height
  int index=0;
  for (int i=0;i<tile_size;i+=tile_size){
    for (int j=0;j<4*tile_size;j+=tile_size){
      sprite[sprite_names[index]] = make_pair(i, j);
      index++;
    }
  }
}

void render_tile(int x, int y, string name) {
  pair<int, int> p = sprite[name]; //! 未检查是否存在，不存在的key会指向第一个Sprite图
  int tile_x = p.second;
  int tile_y = p.first;
  for (int i = 0; i < tile_size; i++) {
    for (int j = 0; j < tile_size; j++) {
      int dst_x = x * tile_size + j;
      int dst_y = y * tile_size + i;
      if (dst_x >= WINDOW_WIDTH || dst_y >= WINDOW_HEIGHT) continue;
      int src_x = tile_x + j;
      int src_y = tile_y + i;
      frame[dst_y * WINDOW_WIDTH + dst_x] = ((int*)gTexture)[src_y * tile_width + src_x];
    }
  }
}

void clean(){
  for (int i = 0; i < tile_width; i++) {
    for (int j = 0; j < WINDOW_WIDTH; j++) {
      frame[i * WINDOW_WIDTH + j] = 0xff000000;
    }
  }
}

void next(){
  render_tile(10,0,"house1");
  render_tile(1,0,"house2");
  render_tile(0,1,"house3");
  render_tile(1,1,"house4");
}


bool render()
{
  SDL_Event event;
  if (SDL_PollEvent(&event))
  {
    if (event.type == SDL_EVENT_QUIT)
    {
      return false;
    }
    if (event.type == SDL_EVENT_KEY_UP && event.key.key == SDLK_ESCAPE)
    {
      return false;
    }
  }
  uint8_t* pix;
  int pitch;
  SDL_LockTexture(SDLTexture, NULL, (void**)&pix, &pitch);
  for (int i = 0, texture_offset = 0, buffer_offset = 0; i < WINDOW_HEIGHT; i++, texture_offset += pitch, buffer_offset += WINDOW_WIDTH)
    memcpy(pix + texture_offset, frame + buffer_offset, WINDOW_WIDTH * 4);
  SDL_UnlockTexture(SDLTexture);
  SDL_RenderTexture(SDLRenderer, SDLTexture, NULL, NULL);
  SDL_RenderPresent(SDLRenderer);
  return true;
}


void loop(){
  if (render()){
    next();
  }else{
    Exit=true;
  }
}


int main(int argc, char **argv)
{
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
  {
    return -1;
  }

  frame = new int[WINDOW_WIDTH * WINDOW_HEIGHT];
  SDLWindow = SDL_CreateWindow("Test", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  SDLRenderer = SDL_CreateRenderer(SDLWindow, NULL);
  SDLTexture = SDL_CreateTexture(SDLRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

  if (!frame || !SDLWindow || !SDLRenderer || !SDLTexture)
    return -1;

  Exit = 0;
  init_sprite();
  while (!Exit)
  {
    loop();
    SDL_Delay(1000/60);// 60FPS
  }

  SDL_DestroyTexture(SDLTexture);
  SDL_DestroyRenderer(SDLRenderer);
  SDL_DestroyWindow(SDLWindow);
  SDL_Quit();

  return 0;
}