# life-game
Life game with web assembly

## Compile
```
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
- Emscripten

## References
- http://secret-garden.hatenablog.com/entry/2017/07/21/145712
