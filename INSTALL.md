# Install dependencies
sudo apt install libeigen3-dev qt5-default git cmake

sudo apt install libopencv-dev # opencv will have to be installed from source if using debian https://github.com/opencv/opencv/archive/refs/tags/3.4.8.tar.gz

# Download Opensidescan
git clone --recursive https://github.com/CIDCO-dev/OpenSidescan


# Install
1- cd Opensidescan && mkdir build && cd build

2- cmake ..

3- make

# run:
./opensidescan
