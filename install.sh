#!/bin/bash 

cd
echo "Installing dependencies..."
sudo apt-get -y install git libxtst-dev > /dev/null # There's a lot that should be in this list...
git clone https://github.com/ryryog25/SpacenavdRadialMenus.git > /dev/null

echo "Opening Fusion Command List..."
xdg-open https://raw.githubusercontent.com/ryryog25/SpacenavdRadialMenus/main/FusionCommands.txt

echo "Opening config.h for editing..."
cd SpacenavdRadialMenus
nano spacenavd-radial-menus/config.h

echo "Moving to build..."
mkdir build
cd build

echo "Building in ${pwd}."
cmake ..
make

echo "Copying executable..."
sudo cp ~/SpacenavdRadialMenus/spacenavd-radial-menus/build/spacenavd-radial-menus /usr/local/bin/

echo "Copying resources..."
sudo mkdir /usr/local/share/spacenavd-radial-menus && sudo cp -r ~/SpacenavdRadialMenus/spacenavd-radial-menus/resources /usr/local/share/spacenavd-radial-menus/

echo -e "Spacenavd Radial Menus can now be started via \e[31mspacenavd-radial-menus [args]"
