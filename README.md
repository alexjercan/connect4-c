# Connect 4

A simple implemenation of the Connect 4 game in C.

The goal of this project is to implement a modular way of injecting different
player strategies using Dynamic Link Libraries (DLLs).

### Quickstart

```console
make
LD_LIBRARY_PATH=dist ./dist/main ./dist/libplayer.so ./dist/libminmax.so
```

or WASM build (player vs minmax):

```console
npm run serve
# open http://localhost:8080
```
