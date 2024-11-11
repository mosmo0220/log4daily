#!/bin/bash

# Create cmake build directory
mkdir -p build
cd build

# Run cmake and compile the application
cmake ..
make

# Determine the OS and copy the binary to the appropriate location
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    sudo cp log4daily /usr/local/bin/
elif [[ "$OSTYPE" == "darwin"* ]]; then
    sudo cp log4daily /usr/local/bin/
elif [[ "$OSTYPE" == "cygwin" ]] || [[ "$OSTYPE" == "msys" ]]; then
    cp log4daily.exe "C:/Program Files/log4daily/"
else
    echo "Unsupported OS"
    exit 1
fi

# Add the binary to the console path
if [[ "$OSTYPE" == "linux-gnu"* ]] || [[ "$OSTYPE" == "darwin"* ]]; then
    echo 'export PATH=$PATH:/usr/local/bin' >> ~/.bashrc
    source ~/.bashrc
elif [[ "$OSTYPE" == "cygwin" ]] || [[ "$OSTYPE" == "msys" ]]; then
    setx PATH "%PATH%;C:\Program Files\log4daily"
fi

echo "Installation complete."