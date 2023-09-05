#!/bin/bash

if [ ! -f "/usr/local/bin/spacenavd-radial-menus" ]; then
	echo "Spacenavd Radial Menus not installed, exiting..."
	exit 0;
fi

echo "Opening Fusion Command List..."
xdg-open https://raw.githubusercontent.com/ryryog25/SpacenavdRadialMenus/main/FusionCommands.txt


echo "Opening config.h for editing..."
cd 
cd SpacenavdRadialMenus/spacenavd-radial-menus
nano config.h

echo "Building spacenavd-radial-menus..."
cd build
cmake .. > /dev/null
make > /dev/null

echo "Killing spacenave-radial-menus, if open..."
sudo killall spacenavd-radial-menus

echo "Copying executable..."
sudo cp ~/SpacenavdRadialMenus/spacenavd-radial-menus/build/spacenavd-radial-menus /usr/local/bin/

echo "SpacenavdRadialMenus has been reconfigured and reinstalled!"
