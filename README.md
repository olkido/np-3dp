# np-3dp

## Compile

Compile using the standard cmake routine:

    cd np-3dp/
    mkdir build; cd build
    cmake -DLIBIGL_DIR=<PATH_TO_LIBIGL> ../
    make

This should find and build the dependencies and create an `np-3dp` binary. For Release mode, do
    cmake -DCMAKE_BUILD_TYPE=Release -DLIBIGL_DIR=<PATH_TO_LIBIGL> ../

To build for an IDE, eg. Xcode, you can do
    cmake -DLIBIGL_DIR=<PATH_TO_LIBIGL> -G Xcode ../

## Run
    ./np-3dp ../../data/radial.OBJ
