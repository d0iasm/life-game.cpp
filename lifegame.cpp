#include <SDL2/SDL.h>
#include <emscripten.h>
#include <cstdlib>
#include <unistd.h>
#include <vector>


using tb = std::vector<std::vector<int>>;
const int N = 8;
const int SIZE = 10;

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
  // tb cells = ctx->cells;

  // SDL_RenderSetLogicalSize(renderer, 200, 200);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);


  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

  SDL_Rect r;
  r.x = ctx->iteration % 255;
  r.y = 50;
  r.w = 50;
  r.h = 50;
  SDL_RenderFillRect(renderer, &r);

  SDL_SetRenderDrawColor(renderer, 0, 155, 50, 255);
  for (int y=0; y<N; y++) {
    for (int x=0; x<N; x++) {
      printf("%d ", ctx->cells[y][x]);
      if (ctx->cells[y][x] == 1) {
        r.y = y * SIZE;
        r.x = x * SIZE;
        r.w = SIZE;
        r.h = SIZE;
        SDL_RenderFillRect(renderer, &r);
      }
    }
    printf("\n");
  }

  SDL_RenderPresent(renderer);

  printf("Pointer %p \n", &(ctx->cells));
  update(ctx->cells);

  ctx->iteration++;
  
  sleep(1);
}

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_CreateWindowAndRenderer(255, 255, 0, &window, &renderer);

  tb cells = tb(N, std::vector<int>(N, 0));

  cells[N/2][N/2] = 1;
  cells[N/2][N/2+1] = 1;
  cells[N/2][N/2+2] = 1;

  for (int i=0; i<N; i++) {
    for (int j=0; j<N; j++) {
      printf("%d ", cells[i][j]);
    }
    printf("\n");
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
