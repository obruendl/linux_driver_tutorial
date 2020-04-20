pushd ../bsp-xilinx
export PATH=$PATH:$(pwd)/bin/arm-none-linux-gnueabi-static/bin/
export ARCH=arm
export CROSS_COMPILE=arm-none-linux-gnueabi-
popd

make clean
make
