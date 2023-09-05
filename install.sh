#!/bin/bash 

cd
echo "Installing dependencies...\n"
sudo apt-get -y install git libxtst-dev # There's a lot that should be in this list...
git clone https://github.com/ryryog25/SpacenavdRadialMenus.git

echo "Opening Fusion Command List...\n"
xdg-open https://raw.githubusercontent.com/ryryog25/SpacenavdRadialMenus/main/FusionCommands.txt

echo "Opening config.h for editing...\n"
cd SpacenavdRadialMenus
nano spacenavd-radial-menus/config.h

echo "Moving to build...\n"
mkdir build
cd build

echo "Building in ${pwd}.\n"
cmake ..
make

echo "Copying executable...\n"
sudo cp ~/SpacenavdRadialMenus/spacenavd-radial-menus/build/spacenavd-radial-menus /usr/local/bin/

echo "Copying resources...\n"
sudo mkdir /usr/local/share/spacenavd-radial-menus && sudo cp -r ~/SpacenavdRadialMenus/spacenavd-radial-menus/resources /usr/local/share/spacenavd-radial-menus/

echo -e "Spacenavd Radial Menus can now be started via \e[31mspacenavd-radial-menus [args]"
