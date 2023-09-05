# SpacenavdRadialMenus


![image](/spacenavd-radial-menus/resources/spacenavd.svg)

Radial Menus for use with spacenavd (and optionally Fusion360 under Wine specifically)

Allows activation of radial menus by pressing side buttons on SpaceMouse devices

## Requirements

1. Spacenavd must be installed and running
1. `libspnav` must be installed
1. `libXtst` must be installed
1. Several Qt dependencies/libs. (Which specifically I'm unsure, if someone with a clean install can test this and let me know, would be appreciated)

## Usage

1. Ensure `spacenavd` is running, then run `spacenavd-radial-menus` from a shell. (The shell can be closed after the command is executed)

1. To close, open either radial menu and hold "close" for 5 seconds.

Currently two arguments are supported:
- `--fusion360` to connect to Fusion 360 Plugin and send Fusion Commands
	- If no connection is established within 15 seconds, `spacenavd-radial-menus` will exit.
- `--no-daemon` to keep the program running tethered to the current shell

Logging while running normally (not with `--no-daemon`) can be accessed at `/var/log/spacenavd-radial-menus.log`

### Fusion 360

1. Copy Add-In folder (`Fusion-SpacenavdRadialMenus`) to your wine prefix in `C:\\users\USERNAME\AppData\Roaming\Autodesk\Autodesk Fusion 360\API\AddIns\`

1. Restart Fusion360 if currently open

1. Run `spacenavd-radial-menus --fusion360` (Shell can be closed after execution)

1. Open the AddIns window under Utilities in Fusion360, and run `SpacenavdRadialMenus`

1. Profit

## Installation

1. Run the following command in your terminal (You will be asked for your password for file manipulation):
	- `/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/ryryog25/SpacenavdRadialMenus/main/install.sh)"`

1. `config.h` will be opened to edit:
	
	In `config.h` is an array with actions for each mouse button with entries for each menu slice (`.top`, `.bottom`, `.left`, `.right`)

	- The `.name` is fairly straightforward, this will be the name which appears on the menu item itself when the radial menu is opened.
	- `.keystroke` is used when running w/o any arguments, and is a string of keysyms to be executed, seperated by `+` (though only intended for modifiers plus a key due to the way the keys get pressed) The keys are pressed in something of a clamshell order. That is, the first key will be the first pressed and last released (the second key the second pressed and second to last released, etc.), so that it is pressed for the duration of the rest of the keystrokes.
	- `fusionAction`s are specifically intended for running with the `--fusion360` argument and the Fusion 360 Add-In. These are Fusion API command IDs to be used to execute the respective command when the respective radial menu option is pressed. A list of these commands can be found in repository (`FusionCommands.txt`). The first string is the command name, and the second is the command ID, which would be used in `.fusionAction`.

	Additional buttons can be configured (for e.g. the SpaceMouse Enterprise) via adding entries to this array. Each new entry represents another button, and the button numbering convention is derived from `spacenavd`, and will be the same. For example, the default config has two structs, the first being for button 0, the second for button 1.

1. Once done configuring, `CTRL+X`, then `Y` to exit and save changes, and the script will continue the installation.

1. `spacenavd-radial-menus` is now installed and can be run.

## Reconfiguring

In the event you wish to change button mappings, run the following command to re-configure `spacenavd-radial-menus` and reinstall:
- `~/SpacenavdRadialMenus/reconfig.sh`

## Uninstall

- `~/SpacenavdRadialMenus/uninstall.sh` to uninstall SpacenavdRadialMenus and cleanup

## Known Issues

- Polling spacenavd is done seperately from Qt event handler
- Window focus can be tempermental on Wayland
	- The following workaround seems to work on Wayland, but has reportedly caused issues with other DEs:
	 	1. Replace lines `111-113` in `main.cpp()` with the following:
		 	```
			menus[sev.button.bnum]->setWindowState(Qt::WindowMinimized);
			menus[sev.button.bnum]->activateWindow();
			menus[sev.button.bnum]->raise();
			menus[sev.button.bnum]->activateWindow();
			menus[sev.button.bnum]->show();
			```
	  	1. See [Reconfiguring](#Reconfiguring) for reinstall.

## Acknowledgements

- Thanks to BrianEkins on the Fusion360 forums for his explanation of the Fusion API where Autodesk's documentation left much to be desired.
- Thanks to [DD1984's SpaceMouse_Fusion360_Wine](https://github.com/DD1984/SpaceMouse_Fusion360_Wine/tree/master/AddIns) Add-In which was invaluable for figuring out how to use UNIX sockets to communicate between the Fusion360 Add-In and `spacenavd-radial-menus` (the code should look curiously... very... similar between the two... sockets are honestly beyond me).
- Thanks to the folks behind FreeSpacenav for the libspacenav API which made interfacing with the SpaceMouse very easy.
- Thanks to jtsiomb specifically for his guidance which inspired this project. 
