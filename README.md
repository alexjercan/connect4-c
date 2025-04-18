# Connect 4

A simple implemenation of the Connect 4 game in C.

The goal of this project is to implement a modular way of injecting different
player strategies using Dynamic Link Libraries (DLLs).

### Quickstart

```console
make
LD_LIBRARY_PATH=out ./out/main ./out/librandom.so ./out/libplayer.so
```
