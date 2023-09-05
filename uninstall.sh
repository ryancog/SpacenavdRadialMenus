#!/bin/bash

cd
echo "Killing spacenavd-radial-menus, if running..."
sudo killall spacenavd-radial-menus 2> /dev/null

echo "Removing git repository..."
sudo rm -r SpacenavdRadialMenus 2> /dev/null

echo "Removing executable..."
sudo rm /usr/local/bin/spacenavd-radial-menus 2> /dev/null

echo "Removing resources..."
sudo rm -r /usr/local/share/spacenavd-radial-menus 2> /dev/null

echo "Cleaning up logs..."
sudo rm /var/log/spacenavd-radial-menus.log 2> /dev/null

echo -e "\nSpacenavdRadialMenus' Fusion360 Add-In must be removed manually if installed.\n(Simply delete the Add-In folder)"
echo -e "\e[31mSpacenavdRadialMenus has been uninstalled!"
