# life-game
Life game with web assembly compiled from C++

https://d0iasm.github.io/life-game.cpp/


## Compile
```
# C++ using SDL
$ emcc -std=c++11 hoge.cpp -s WASM=1 -s USE_SDL=2 -O3 -o hoge.js

# C++
$ emcc hoge.cpp -std=c++14 -s WASM=1 -o hoge.html

# C
$ emcc hoge.c -s WASM=1 -o hoge.html 
```

## Run
```
$ emrun hoge.html
```

## Required
- \>= C++14
- Emscripten (https://kripken.github.io/emscripten-site/index.html)

To enable emscripten commands
```
$ source <path-to-emsdk>/emsdk_env.sh
```

## References
- http://secret-garden.hatenablog.com/entry/2017/07/21/145712
- https://github.com/timhutton/sdl-canvas-wasm
