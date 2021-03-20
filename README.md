# HolmnerChessEngine

## Build

### Windows
#### Prerequisites
CMake (version > 3.16.3 recommended).
SFML library (version 2.51 recommended).
Visual Studio.

#### Steps
1. Start a command prompt in /HolmnerChessEngine
2. mkdir build
3. cd build
4. cmake -D CMAKE_PREFIX_PATH=**<absolute_path>**/SFML-2.5.1/lib/cmake/SFML ..
5. Open the HolmnerChessEngine.sln file and build (Release configuration recommended).
6. Executable (.exe) will be located in HolmnerChessGui/<configuration>/ after build.

Note: in step 4, replace **<absolute_path>** with the missing part of the path to SFML.

### Linux
#### Prerequisites
CMake (version > 3.16.3 recommended), (**sudo apt-get install cmake**)
GCC (**sudo apt install build-essential**)
SFML (version 2.51 recommended) (**sudo apt-get install libsfml-dev**)


#### Steps
1. Start a command prompt in /HolmnerChessEngine
2. mkdir build
3. cd build
4. cmake -DCMAKE_BUILD_TYPE=Release ..
5. make
6. Executable will be located in HolmnerChessGui/ after build.
