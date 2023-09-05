#!/bin/bash 

cd
sudo apt-get -y install git libxtst-dev # There's a lot that should be in this list...
git clone https://github.com/ryryog25/SpacenavdRadialMenus.git

xdg-open https://raw.githubusercontent.com/ryryog25/SpacenavdRadialMenus/main/FusionCommands.txt
cd SpacenavdRadialMenus
nano spacenavd-radial-menus/config.h

mkdir build
cd build

cmake ..
make

sudo cp ~/SpacenavdRadialMenus/spacenavd-radial-menus/build/spacenavd-radial-menus /usr/local/bin/

sudo mkdir /usr/local/share/spacenavd-radial-menus && sudo cp -r ~/SpacenavdRadialMenus/spacenavd-radial-menus/resources /usr/local/share/spacenavd-radial-menus/

echo -e "Spacenavd Radial Menus can now be started via \e[31mspacenavd-radial-menus [args]"
