#!/bin/bash 

cd
echo "Installing dependencies..."
sudo apt-get -y install git libxtst-dev > /dev/null # There's a lot that should be in this list...
git clone --quiet https://github.com/ryryog25/SpacenavdRadialMenus.git > /dev/null

echo "Opening Fusion Command List..."
xdg-open https://raw.githubusercontent.com/ryryog25/SpacenavdRadialMenus/main/FusionCommands.txt

echo "Opening config.h for editing..."
cd SpacenavdRadialMenus/spacenavd-radial-menus
nano config.h

echo "Building spacenavd-radial-menus..."
mkdir build
cd build
cmake .. > /dev/null
make > /dev/null

echo "Copying executable..."
sudo cp ~/SpacenavdRadialMenus/spacenavd-radial-menus/build/spacenavd-radial-menus /usr/local/bin/

echo "Copying resources..."
sudo mkdir /usr/local/share/spacenavd-radial-menus
sudo cp -r ~/SpacenavdRadialMenus/spacenavd-radial-menus/resources /usr/local/share/spacenavd-radial-menus/

echo -e "Spacenavd Radial Menus can now be started via \e[32mspacenavd-radial-menus [args]"
