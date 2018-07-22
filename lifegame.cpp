#include <SDL2/SDL.h>
#include <emscripten.h>
#include <cstdlib>
#include <vector>


using tb = std::vector<std::vector<bool> >;
const int N = 10;

struct context {
  SDL_Renderer *renderer;
  int iteration;
  tb cells;
  // std::vector<std::vector<bool> > cells;
};

void mainloop(void *arg) {
  context *ctx = static_cast<context*>(arg);
  SDL_Renderer *renderer = ctx->renderer;

  // example: draw a moving rectangle

  // red background
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // moving blue rectangle

  // vector<SDL_Rect> cellr;

  SDL_SetRenderDrawColor(renderer, 0, 155, 50, 255);

  SDL_Rect r;
  r.x = ctx->iteration % 255;
  r.y = 50;
  r.w = 50;
  r.h = 50;

  for (int i=0; i<3; i++) {
    SDL_Rect r2;
    r2.y = ctx->iteration % 255 + i*50;
    r2.x = 50;
    r2.w = 10;
    r2.h = 10;
    SDL_RenderFillRect(renderer, &r2);
  }

  SDL_RenderFillRect(renderer, &r);

  SDL_RenderPresent(renderer);

  ctx->iteration++;
}

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_CreateWindowAndRenderer(255, 255, 0, &window, &renderer);

  tb cells = tb(N, std::vector<bool>(N, 0));
  for (int i=0; i<3; i++) {
    cells[N/2+i][N/2] = 1;
    cells[N/2][N/2+i] = 1;
  }

  for (int i=0; i<N; i++) {
    for (int j=0; j<N; j++) {
      printf("%d ", (int)cells[i][j]);
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
