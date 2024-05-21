# UNI-PathFindingSFML

## Linux
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Windows
```
cmake -S . -B build
cmake --build build --config Release
```
## For an A* sample:
```
./build/bin/CMakeSFMLProject ga.txt
```
## For an Dijkstra sample:
```
./build/bin/CMakeSFMLProject gd.txt
```
## Extra resources:
https://github.com/SFML/cmake-sfml-project/blob/master/README.md
