# SpacenavdRadialMenus

Radial Menus for use with spacenavd (and optionally Fusion360 under Wine specifically)

![image](/spacenavd-radial-menus/resources/spacenavd.svg)

## Requirements

1. Spacenavd must be installed and running
1. `libspnav` must be installed
1. `libXtst` must be installed
1. QtCreator must be installed to build program and configure menu mappings (config file to-do)
1. Should work with SpaceMouse devices with 2 buttons (support for additional buttons to-do)

## Installation/Usage

TODO: Create a `install.sh` (and `uninstall.sh`)

1. Clone the repository: `cd && git clone https://github.com/ryryog25/SpacenavdRadialMenus.git`

1. Open the `spacenavd-radial-menus` project with QtCreator: 
	Under "Source Files," open `main.cpp`
	
	At the very top of `main()` are two structs `leftActions` and `rightActions` which contain the configuration for the Radial Menus, and for each struct is an entry for each menu slice (`.top`, `.bottom`, `.left`, `.right`)

	- The `.name` is fairly straightforward, this will be the name which appears on the menu item itself when the radial menu is opened.
	- `.keystroke` is used when running w/o any arguments, and is a string of keysyms to be executed, seperated by `+` (though only intended for modifiers plus a key due to the way the keys get pressed) The keys are pressed in something of a clamshell order. That is, the first key will be the first pressed and last released (the second key the second pressed and second to last released, etc.), so that it is pressed for the duration of the rest of the keystrokes.
	- `fusionAction`s are specifically intended for running with the `--fusion360` argument and the Fusion 360 Add-In. These are Fusion API command IDs to be used to execute the respective command when the respective radial menu option is pressed. A list of these commands can be found in repository (`FusionCommands.txt`). The first string is the command name, and the second is the command ID, which would be used in `.fusionAction`.

1. Once configuration is done, press the hammer icon in the bottom left hand corner of QtCreator to build `spacenavd-radial-menus`, the compiled executable can be found in `spacenavd-radial-menus/build/spacenavd-radial-menus`

1. Copy the executable: `sudo cp ~/SpacenavdRadialMenus/spacenavd-radial-menus/build/spacenavd-radial-menus /usr/local/bin/`

1. Copy resources `sudo mkdir /usr/local/share/spacenavd-radial-menus && sudo cp -r ~/SpacenavdRadialMenus/spacenavd-radial-menus/resources /usr/local/share/spacenavd-radial-menus/`

1. Ensure `spacenavd` is running, then run `spacenavd-radial-menus` from a shell. (The shell can be closed after the command is executed)

## Fusion 360

1. Copy Add-In folder (`Fusion-SpacenavdRadialMenus`) to your wine prefix in `C:\\users\USERNAME\AppData\Roaming\Autodesk\Autodesk Fusion 360\API\AddIns\`

1. Restart Fusion if currently open

1. Run `spacenavd-radial-menus --fusion360` (Shell can be closed after execution)

1. Open the AddIns window under Utilities in Fusion360, and run `SpacenavdRadialMenus`

1. Profit

## Acknowledgements

- Thanks to BrianEkins on the Fusion360 forums for his explanation of the Fusion API where Autodesk's documentation left much to be desired.
- Thanks to [DD1984's SpaceMouse_Fusion360_Wine](https://github.com/DD1984/SpaceMouse_Fusion360_Wine/tree/master/AddIns) Add-In which was invaluable for figuring out how to use UNIX sockets to communicate between the Fusion360 Add-In and `spacenavd-radial-menus` (the code should look curiously... very... similar between the two... sockets are honestly beyond me).
- Thanks to the folks behind FreeSpacenav for the libspacenav API which made interfacing with the SpaceMouse very easy.
- Thanks to jtsiomb specifically for his guidance which inspired this project. 
