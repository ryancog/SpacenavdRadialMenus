#pragma once
#include "widget.h"

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
