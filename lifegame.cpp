#include <SDL2/SDL.h>
#include <cstdlib>
#include <emscripten.h>
#include <time.h>
#include <unistd.h>
#include <vector>


using tb = std::vector<std::vector<int>>;

const int MAX_SIZE = 800;
const int N = 32;
int SIZE = MAX_SIZE / N;
const int TITLE[5][32] = {
  {1,0,0,0,1,0,1,1,1,0,1,1,1,0,0,1,1,1,0,1,1,1,0,1,0,0,0,1,0,1,1,1},
  {1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,1,0,1,1,0,1,1,0,1,0,0},
  {1,0,0,0,1,0,1,1,1,0,1,1,1,0,0,1,0,0,0,1,1,1,0,1,1,1,1,1,0,1,1,1},
  {1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0},
  {1,1,1,0,1,0,1,0,0,0,1,1,1,0,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1}
};

struct context {
  SDL_Renderer *renderer;
  int iteration;
  tb cells;
};

int at_cell(tb &cells, int x, int y){
  if ((x < 0 || N <= x) || (y < 0 || N <= y)) {
    return 0;
  }
  return cells[y][x];
}

int dead_or_alive(tb &cells, int x, int y) {
  int cell = at_cell(cells, x, y);
  auto count
    = at_cell(cells, x - 1, y - 1)
    + at_cell(cells, x - 0, y - 1)
    + at_cell(cells, x + 1, y - 1)
    + at_cell(cells, x + 1, y - 0)
    + at_cell(cells, x + 1, y + 1)
    + at_cell(cells, x - 0, y + 1)
    + at_cell(cells, x - 1, y + 1)
    + at_cell(cells, x - 1, y - 0);
  if (count == 3 || (cell == 1 && count == 2)) {
    return 1;
  }
  return 0;
}

void update(tb &cells) {
  for (int y=0; y<N; y++) {
    for (int x=0; x<N; x++) {
      cells[y][x] = dead_or_alive(cells, x, y);
    }
  }
}

void mainloop(void *arg) {
  context *ctx = static_cast<context*>(arg);
  SDL_Renderer *renderer = ctx->renderer;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
  SDL_Rect t;
  for (int y=0; y<5; y++) {
    for (int x=0; x<34; x++) {
      if (TITLE[y][x] == 1) {    
        t.x = x * SIZE;
        t.y = y * SIZE;
        t.w = SIZE;
        t.h = SIZE;
        SDL_RenderFillRect(renderer, &t);
      }
    }
  }

  SDL_Rect r;
  SDL_SetRenderDrawColor(renderer, 0, 155, 50, 255); // Green
  for (int y=0; y<N; y++) {
    for (int x=0; x<N; x++) {
      if (ctx->cells[y][x] == 1) {
        r.x = x * SIZE;
        r.y = (y + 5) * SIZE;
        r.w = SIZE;
        r.h = SIZE;
        SDL_RenderFillRect(renderer, &r);
      }
    }
  }
  SDL_RenderPresent(renderer);

  update(ctx->cells);
  ctx->iteration++;

  sleep(1);
}

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_CreateWindowAndRenderer(MAX_SIZE, MAX_SIZE, 0, &window, &renderer);

  tb cells = tb(N, std::vector<int>(N, 0));

  srand((unsigned int) time(NULL));
  for (int i=0; i<N*N/4; i++) {
    cells[rand() % N][rand() % N] = 1;
  }

  context ctx;
  ctx.renderer = renderer;
  ctx.iteration = 0;
  ctx.cells = cells;

  const int simulate_infinite_loop = 1; // call the function repeatedly
  const int fps = -1; // call the function as fast as the browser wants to render (typically 60fps)
  emscripten_set_main_loop_arg(mainloop, &ctx, fps, simulate_infinite_loop);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
