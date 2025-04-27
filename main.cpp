#include "SDL.h"
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <random>
#include "json.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using json = nlohmann::json;
using namespace std;

int *frame;
SDL_Window *SDLWindow;
SDL_Renderer *SDLRenderer;
SDL_Texture *SDLTexture;
static int Exit;
const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 540;
int tile_width, tile_height, tile_channel;
int *gTexture;
int tile_size = 32;
int grid_width;
int grid_height;
vector<string> sprite_names;
map<string, pair<int, int>> sprite;
vector<vector<string>> grid;

// 随机数
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> dis(1, 1000);

void init_sprite(int h, int w, int count)
{
  int index = 0;
  for (int i = 0; i < h * tile_size; i += tile_size)
  {
    for (int j = 0; j < w * tile_size; j += tile_size)
    {
      if (index >= count)
      {
        break;
      }
      sprite[sprite_names[index]] = make_pair(i, j);
      index++;
    }
  }
}

void load_tileset()
{
  std::ifstream f("tiles/tiles.json");
  json data = json::parse(f);
  int h = data["height"];
  int w = data["width"];
  int count = data["count"];
  sprite_names = data["tiles"];
  gTexture = (int *)stbi_load("tiles/tiles.png", &tile_width, &tile_height, &tile_channel, 4);
  init_sprite(h, w, count);
}

void render_tile(int x, int y, string name)
{
  if (find(sprite_names.begin(), sprite_names.end(), name) == sprite_names.end())
  {
    name = "NULL"; //! 应改为提示图像
  }
  pair<int, int> p = sprite[name];
  int tile_x = p.second;
  int tile_y = p.first;
  for (int i = 0; i < tile_size; i++)
  {
    for (int j = 0; j < tile_size; j++)
    {
      int dst_x = x * tile_size + j;
      int dst_y = y * tile_size + i;
      if (dst_x >= WINDOW_WIDTH || dst_y >= WINDOW_HEIGHT)
        continue;
      int src_x = tile_x + j;
      int src_y = tile_y + i;
      if (name == "NULL")
      {
        frame[dst_y * WINDOW_WIDTH + dst_x] = 0xff000000;
      }
      else
      {
        frame[dst_y * WINDOW_WIDTH + dst_x] = ((int *)gTexture)[src_y * tile_width + src_x] | 0xff000000;
      }
    }
  }
}

void clean_board()
{
  for (int i = 0; i < grid_width; i++)
  {
    for (int j = 0; j < grid_height; j++)
    {
      grid[i][j] = "NULL";
    }
  }
}

void next()
{
  for (int i = 0; i < grid_width; i++)
  {
    for (int j = 0; j < grid_height; j++)
    {
      render_tile(i, j, grid[i][j]);
    }
  }
}

void process_input(SDL_Keycode key)
{
  switch (key)
  {
  case SDLK_S:
    clean_board();
    break;
  case SDLK_N:
    grid[dis(gen) % grid_width][dis(gen) % grid_height] = sprite_names[dis(gen) % sizeof(sprite_names)];
    break;
  default:
    return;
  }
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
    if (event.type == SDL_EVENT_KEY_UP)
    {
      process_input(event.key.key);
    }
  }
  uint8_t *pix;
  int pitch;
  SDL_LockTexture(SDLTexture, NULL, (void **)&pix, &pitch);
  for (int i = 0, texture_offset = 0, buffer_offset = 0; i < WINDOW_HEIGHT; i++, texture_offset += pitch, buffer_offset += WINDOW_WIDTH)
    memcpy(pix + texture_offset, frame + buffer_offset, WINDOW_WIDTH * 4);
  SDL_UnlockTexture(SDLTexture);
  SDL_RenderTexture(SDLRenderer, SDLTexture, NULL, NULL);
  SDL_RenderPresent(SDLRenderer);
  return true;
}

void loop()
{
  if (render())
  {
    next();
  }
  else
  {
    Exit = true;
  }
}

void init()
{
  grid_width = WINDOW_WIDTH / tile_size;
  grid_height = WINDOW_HEIGHT / tile_size;
  grid = vector<vector<string>>(grid_width, vector<string>(grid_height));
  load_tileset();
  clean_board();
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
  init();
  while (!Exit)
  {
    loop();
    SDL_Delay(1000 / 60); // 60FPS
  }

  SDL_DestroyTexture(SDLTexture);
  SDL_DestroyRenderer(SDLRenderer);
  SDL_DestroyWindow(SDLWindow);
  SDL_Quit();

  return 0;
}