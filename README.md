# MD5-near-collisions

## Build Instructions

The different testcases (bytes to attack / code constructions) ared hardcoded and can be enabled by defining the macro HASH_CONFIG in the cmake command. E.g., this will create makefiles for a simple 64 bit version of MD5 with an epsilon of 2:

```
cmake -DCMAKE_BUILD_TYPE=Release -DHASH_CONFIG=CFG_64BIT_EPS2 ../src
```

HASH_CONFIG must be defined, otherwise the source will not compile! Currently, there are the following configurations available:

* CFG_MD5_FULL (Full collision search on MD5)
* CFG_128BIT_EPS16 (MD5, codes: 8x16)
* CFG_96BIT_EPS16_UNIFORM (96 bit version of MD5, codes: 6x8, 1x16, 1x32)
* CFG_96BIT_EPS16_SECOND (96 bit version of MD5, codes: 4x8, 4x16)
* CFG_88BIT_EPS8_UNIFORM (88 bit version of MD5, codes: 2x32, 1x16, 1x8)
* CFG_88BIT_EPS8_SECOND (88 bit version of MD5, codes: 1x64, 3x8)
* CFG_80BIT_FULL (Full collision search on 88 bit version of MD5)
* CFG_64BIT_EPS2 (64 bit version of MD5, codes: 1x64)

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DHASH_CONFIG=[configuration name]  ../src
make
```
## Run attack

You can choose between brents cycle finding and the distinguished points algorithm.

For brents algorithm, there are no configuration parameters:

```
./collider b
```

The distinguished point algorithm can be configured by the number of threads to use and the number of leading zeros to use as the distinguished point property. This will run the distinguished point algorithm with 4 threads and 20 leading zeros:

```
./collider dp 4 20
```
