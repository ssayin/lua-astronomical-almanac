# lua-astronomical-almanac

Lua bindings for Astronomical Almanac (AA) a.k.a [Moshier](http://www.moshier.net/) Ephemeris.

C program computes ephemerides of Sun, Moon, planets, comets, and stars using rigorous reduction methods from the _Astronomical Almanac_ and related sources.
https://salsa.debian.org/alteholz/astronomical-almanac

## Getting Started

### Build
```sh
git clone https://github.com/ssayin/lua-astronomical-almanac.git
cd lua-astronomical-almanac
cmake -S . -B build
make -C build
```
### Run
```sh
cd lua
luajit aa_test.lua
```
