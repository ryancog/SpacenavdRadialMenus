#pragma once
#include "widget.h"

/*
*    Below is the spacenavd-radial-menus button action config.
*
*    Each entry in the actions[] array corresponds to a radial menu for a SpaceMouse button.
*    Each entry contains several fields for the .top, .bottom, .left, and .right menus
*        (Shown when corresponding button is triggered)
*
*        .name is simply the name to be shown on the radial menu
*        .keystroke is the keysym to be executed when running normally
*        .fusionAction is the command in Fusion360 to be run when running with --fusion360
*            - The list of Fusion360 commands should have opened in your browser (or can be accessed on the repo)
*            - Many of these commands are not intended to be executed at any time, and care should be used
*              when executing them, the file structure is [Name], [Command ID], where the Command ID is
*              what is used for the fusionAction.
*
*    Additional buttons can be enabled by adding a new struct to the array, and filling out the corresponding
*    fields for the new struct.
*
*
*    For more information, please reference the README located on the github repo.
*
*/


Widget::ButtonActions actions[] = {
    { // Button 0 (Left)
        .top = {
            .name         = "Extrude",
            .keystroke    = "E",
            .fusionAction = "Extrude"
        },
        .bottom = {
            .name         = "Revolve",
            .keystroke    = "N",
            .fusionAction = "Revolve"
        },
        .left = {
            .name         = "Chamfer",
            .keystroke    = "Shift_L+C",
            .fusionAction = "FusionChamferCommand"
        },
        .right = {
            .name         = "Fillet",
            .keystroke    = "F",
            .fusionAction = "FusionFilletEdgesCommand"
        }
    },
    { // Button 1 (Right)
        .top = {
            .name         = "Joint",
            .keystroke    = "J",
            .fusionAction = "JointAssembleCmdNew",
        },
        .bottom = {
            .name         = "Measure",
            .keystroke    = "I",
            .fusionAction = "MeasureCommand",
        },
        .left = {
            .name         = "Trim",
            .keystroke    = "T",
            .fusionAction = "TrimSketchCmd",
        },
        .right = {
            .name         = "Circle",
            .keystroke    = "C",
            .fusionAction = "CircleCenterRadius"
        }
    }
};
