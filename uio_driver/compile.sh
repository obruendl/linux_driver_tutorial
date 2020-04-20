#Kernel-Sources (this path must be changed on your machine!)
export BSP_XILINX=~/Ebe/bsp-xilinx

#Setup compiler path
pushd $BSP_XILINX
export PATH=$PATH:$(pwd)/bin/arm-none-linux-gnueabi-static/bin/
popd

#Make
make clean
make
