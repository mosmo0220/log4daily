#!/bin/bash

# Create cmake build directory
mkdir -p build
cd build

# Check if cmake is installed
if ! command -v cmake &> /dev/null
then
    echo "cmake could not be found, please install it first."
    exit 1
fi

if ! command -v python3 &> /dev/null
then
    echo "python3 could not be found, please install it first."
    exit 1
fi

# Run l4dFiles make
python3 ./l4dFiles/make.py

# Run cmake and compile the application
cmake ..
make

# Determine the OS and copy the binary to the appropriate location
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    sudo cp log4daily /usr/local/bin/
elif [[ "$OSTYPE" == "darwin"* ]]; then
    sudo cp log4daily /usr/local/bin/
elif [[ "$OSTYPE" == "cygwin" ]] || [[ "$OSTYPE" == "msys" ]]; then
    cp log4daily.exe "/c/Program Files/log4daily/"
else
    echo "Unsupported OS"
    exit 1
fi

# Add the binary to the console path
if [[ "$OSTYPE" == "linux-gnu"* ]] || [[ "$OSTYPE" == "darwin"* ]]; then
    echo 'export PATH=$PATH:/usr/local/bin' >> ~/.bashrc
    source ~/.bashrc
elif [[ "$OSTYPE" == "cygwin" ]] || [[ "$OSTYPE" == "msys" ]]; then
    echo 'export PATH=$PATH:/c/Program Files/log4daily' >> ~/.bashrc
    source ~/.bashrc
fi

# Clean up build directory
cd ..
rm -rf build

# Installation complete
echo "Installation complete."
