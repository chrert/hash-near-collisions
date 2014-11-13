#hash-near-collisions

## Configuration

To modify the complexity adjust `TRUNCATED_SIZE` in `configuration.h`. 48 is a good value to find collisions in a few seconds.

## Build

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release  ../src
make
```
