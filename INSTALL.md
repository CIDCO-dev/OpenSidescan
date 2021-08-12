# Linux instruction
windows instruction will follow

# Install dependencies
sudo apt install libeigen3-dev qt5-default git cmake

opencv will have to be installed from source
download :
https://github.com/opencv/opencv/archive/refs/tags/3.4.8.tar.gz

extraire :
tar -xvf opencv-3.4.8.tar.gz -C ~/

cd ~/opencv3.4.8
mkdir build && cd build
cmake ..
sudo make install


# Download Opensidescan
git clone --recursive https://github.com/CIDCO-dev/OpenSidescan


# Install
1- navigate to Opensidescan directory

2-mkdir build && cd build

3- cmake ..

4- make

# run:
./opensidescan


# Windows instruction

# Download Opensidescan installer 
https://github.com/CIDCO-dev/OpenSidescan/installer

# Install 
double click
