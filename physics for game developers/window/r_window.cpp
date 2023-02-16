#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <string>
#include "r_window.h"

using namespace std;

static r_window *mainwindow = NULL;
static unordered_map<int, int> keyboard;

r_window::r_window(const string &title, int x, int y, int width, int height)
{
  this->title = title;
  int res = SDL_Init(SDL_INIT_VIDEO);
  if (res != 0)
  {
    throw runtime_error("Erro ao iniciar a SDL.");
  }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
handle = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!handle)
    {
SDL_Quit();
throw runtime_error("Erro ao criar a janela sdl.");
    }
renderer= SDL_GL_CreateContext(handle);
    if (!renderer)
    {
SDL_DestroyWindow(handle);
SDL_Quit();
throw runtime_error("Erro ao criar o contexto opengl.");
    }
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
SDL_DestroyWindow(handle);
SDL_Quit();
throw runtime_error("Error to init GLEW!");
    }
  keyboard.clear();
  mainwindow = this;
}

r_window::~r_window()
{
  if (this->handle != NULL)
  {
    SDL_GL_DeleteContext(renderer);
    SDL_DestroyWindow(handle);
  }
  this->handle = NULL;
  this->renderer = NULL;
  this->title = "";
  mainwindow = NULL;
  SDL_Quit();
}

void r_window::setTitle(const string &title)
{
  if (this->handle != NULL)
  {
    this->title = title;
    SDL_SetWindowTitle(this->handle, title.c_str());
  }
}

string r_window::getTitle() const
{
  return this->title;
}

bool r_window::is_open() const
{
  return this->handle != NULL;
}

void r_window::close()
{
  if (this->handle != NULL)
  {
    SDL_GL_DeleteContext(renderer);
    SDL_DestroyWindow(handle);
  }
  this->handle = NULL;
  this->renderer = NULL;
  this->title = "";
}

void r_window::make_loop()
{
  if (!is_open())
  {
    return;
  }
  SDL_Event ev;
  while (SDL_PollEvent(&ev) != 0)
  {
    if (key_pressed(SDLK_ESCAPE))
    {
      this->close();
    }
    switch (ev.type)
    {
    case SDL_QUIT:
    case SDL_APP_TERMINATING:
    {
      this->close();
      break;
    }
    case SDL_KEYDOWN:
    {
      auto it = keyboard.find(ev.key.keysym.sym);
      if (it == keyboard.end())
      {
        keyboard.insert(make_pair(ev.key.keysym.sym, sk_pressed));
      }
      else
      {
        switch (it->second)
        {
        case sk_up:
        {
          it->second = sk_pressed;
          break;
        }
        case sk_pressed:
        case sk_down:
        {
          break;
        }
        }
      }
      break;
    }
    case SDL_KEYUP:
    {
      auto it = keyboard.find(ev.key.keysym.sym);
      if (it != keyboard.end())
      {
        it->second = sk_up;
      }
      break;
    }
    case SDL_WINDOWEVENT:
    case SDL_TEXTEDITING:
    case SDL_TEXTINPUT:
    case SDL_MOUSEMOTION:
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEWHEEL:
    {
      break;
    }
    }
  }
    SDL_RenderPresent(this->renderer);
}
void r_window::clear(unsigned char r, unsigned char g, unsigned char b)
{
  SDL_SetRenderDrawColor(this->renderer, r, g, b, 0);
  SDL_RenderClear(this->renderer);
}
bool r_window::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
  SDL_SetRenderDrawColor(this->renderer, r, g, b, 0);
  for (int i = (x - 5); i <= (x + 5); i++)
  {
    for (int i1 = (y - 5); i1 <= (y + 5); i1++)
    {
      SDL_RenderDrawPoint(renderer, i, i1);
    }
  }
  return SDL_RenderDrawPoint(renderer, x, y) == 0;
}
void r_window::setRegion(int x1, int x2, int y1, int y2, unsigned char r, unsigned char g, unsigned char b)
{
  SDL_SetRenderDrawColor(this->renderer, r, g, b, 0);
  int w = 0, h = 0;
  SDL_GetWindowSize(handle, &w, &h);
  for (int i = x1; i <= x2; i++)
  {
    for (int i1 = y1; i1 <= y2; i1++)
    {
      setPixel(i, i1, r, g, b);
    }
  }
}
void r_window::setBackGroundColor(unsigned char r, unsigned char g, unsigned char b)
{
  SDL_SetRenderDrawColor(this->renderer, r, g, b, 0);
  int w = 0, h = 0;
  SDL_GetWindowSize(handle, &w, &h);
SDL_Rect r2;
r2.x=0;
r2.y=0;
r2.w=w;
r2.h=h;
SDL_RenderFillRect(renderer, &r2);
}
void r_window::drawBox(box3d* box, uint8 r, uint8 g, uint8 b)
{
int width=0, height=0;
  SDL_GetWindowSize(handle, &width, &height);
    SDL_Rect r2;
    r2.x = box->min.x;
    r2.y = height - box->min.y;
    r2.w = box->measures.x;
    r2.h = box->measures.y;
    SDL_SetRenderDrawColor(this->renderer, r, g, b, 0);
    SDL_RenderFillRect(this->renderer, &r2);
}
void r_window::drawCircle(sphere3d* s, uint8 r, uint8 g, uint8 b)
{
profiler_snap();
int width=0, height=0;
  SDL_GetWindowSize(handle, &width, &height);
    SDL_SetRenderDrawColor(this->renderer, r, g, b, 0);
vector3d center=s->center;
center.y=height-center.y;
    for (int w = 0; w < s->radius * 2; w++)
    {
        for (int h = 0; h < s->radius * 2; h++)
        {
            int dx = s->radius - w; // horizontal offset
            int dy = s->radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (s->radius * s->radius))
            {
                SDL_RenderDrawPoint(this->renderer, center.x + dx, center.y + dy);
            }
        }
    }
}
// Funções de teclado...
bool key_pressed(int key_code)
{
  auto it = keyboard.find(key_code);
  if (it == keyboard.end())
  {
    return false;
  }
  if (it->second == sk_pressed)
  {
    it->second = sk_down;
    return true;
  }
  return false;
}
bool key_down(int key_code)
{
  auto it = keyboard.find(key_code);
  return ((it == keyboard.end()) ? false : it->second == sk_down);
}
bool key_up(int key_code)
{
  auto it = keyboard.find(key_code);
  return ((it == keyboard.end()) ? false : it->second == sk_up);
}
