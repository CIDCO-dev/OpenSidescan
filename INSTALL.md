# Linux instruction
windows instruction will follow

# Install dependencies (on Ubuntu)
sudo apt install libeigen3-dev qt5-default git cmake libopencv-dev libmlpack-dev

# Download Opensidescan
git clone --recursive https://github.com/CIDCO-dev/OpenSidescan


# Install
1- navigate to Opensidescan directory

2-mkdir build && cd build

3- cmake ..

4- make

to launch opensidescan from anywhere

5- sudo make install

# Download models (optional)

1- navigate to Opensidescan directory

2- mkdir models && cd models

3- wget http://updates.cidco.ca/Opensidescan-modeles/crabtrapV1.onnx


# run:
./Opensidescan


# Windows instruction

# Download Opensidescan installer
copy paste link in browser 
https://github.com/cidco-dev/OpenSidescan/raw/master/installer/Opensidescan-1.0.0-win64.exe

# Install 
double click

