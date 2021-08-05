#Linux instruction
windows instruction will follow

# Install dependencies
sudo apt install libeigen3-dev qt5-default git cmake

sudo apt install libopencv-dev # opencv will have to be installed from source if using debian https://github.com/opencv/opencv/archive/refs/tags/3.4.8.tar.gz

# Download Opensidescan
git clone --recursive https://github.com/CIDCO-dev/OpenSidescan


# Install
1- navigate to Opensidescan directory

2-mkdir build && cd build

3- cmake ..

4- make

# run:
./opensidescan


#Windows instruction

#install dependencies:
suggestion for install dependencies location : C:\LIBS\
download eigen 3.3.X https://eigen.tuxfamily.org/
download the executable file of opencv 3.4.7 https://github.com/opencv/opencv/releases/tag/3.4.7
add to path env : C:\LIBS\opencv\build\x64\vc15\bin   ** restart terminal after adding path ..its not linux

here we can leave default location
download visual studio : during installation select desktop dev c++ kit https://visualstudio.microsoft.com/downloads/
download qtcreator 5.12.X https://www.qt.io/qt-5-12
add to path env : C:\Qt\5.12.11\msvc2015_64\bin   ** restart terminal after adding path ..its not linux


# Download Opensidescan
git clone --recursive https://github.com/CIDCO-dev/OpenSidescan


# Install ( git bash required + add path env for cmake and git )
1- navigate to Opensidescan directory

2-mkdir build && cd build

3- cmake ..

4- make

# run:
./Debug/opensidescan