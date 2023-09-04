#include "widget.h"

#include <QApplication>
#include <QSystemSemaphore>
#include <QDebug>
#include <chrono>
#include <thread>
#include <string>
#include <iostream>
#include <fstream>

#define LOG(msg) std::cout << msg << std::endl;

extern "C" {
#include <spnav.h>
}

using std::to_string;

pollfd* fusionSetup();
bool checkArg(string arg, int argc, char* args[]);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!checkArg("--no-daemon", argc, argv)) {
        daemon(1, 0);
        freopen("/var/log/spacenavd-radial-menus.log", "w", stdout);
        LOG("Running as daemon...");
    }

    bool isFusion = false;
    pollfd* fusionfd = nullptr;

    Widget::ButtonActions leftActions = {
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
    };

    Widget::ButtonActions rightActions = {
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
    };


    if (checkArg("--fusion360", argc, argv)) {
        isFusion = true;
        if ((fusionfd = fusionSetup()) == nullptr) {
            LOG("Could not establish connection to Fusion360");
            return -1;
        }
    }

    Widget lMenu(nullptr, leftActions, isFusion, fusionfd);
    Widget rMenu(nullptr, rightActions, isFusion, fusionfd);
    Widget* menus[2] = {
        &lMenu,
        &rMenu
    };

    if(spnav_open() !=  -1) {
        spnav_event sev;

        while(!false) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            if (spnav_poll_event(&sev) == SPNAV_EVENT_BUTTON && sev.button.press == 0) {
                QPoint cLoc = QCursor::pos();
                menus[sev.button.bnum]->setGeometry(
                            cLoc.x() - (lMenu.width() / 2),
                            cLoc.y() - (lMenu.height() / 2),
                            cLoc.x() + (lMenu.width() / 2),
                            cLoc.y() + (lMenu.height() /2)
                            );
                menus[sev.button.bnum]->activateWindow();
                menus[sev.button.bnum]->raise();
                menus[sev.button.bnum]->show();
                a.exec();
                spnav_remove_events(SPNAV_EVENT_BUTTON);
            }
        }
    } else {
        LOG("Failed to connect to spacenavd, exiting...");
    }

    spnav_close();
    return 0;
}

// Sets up fusionfd and returns pointer to fd object
pollfd* fusionSetup() {
    LOG("Initializing connection to Fusion360...");

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        LOG("Could not create socket, exiting...");
        return nullptr;
    }

    int enable = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    struct sockaddr_in addr_in = {};
    addr_in.sin_family = AF_INET;
    addr_in.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr_in.sin_port = htons(/* PORT */ 22222);

    if (bind(listenfd, (struct sockaddr*)&addr_in, sizeof(addr_in))) {
        std::cout << "Could not bind socket, exiting..." << std::endl;
        close(listenfd);
        return nullptr;
    }

    listen(listenfd, 1);

    int fusionfd;

    LOG("Waiting for connection...");
    while ((fusionfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) < 0) {}

    LOG("Connection Accepted.");

    static struct pollfd pollfd;
    pollfd.fd = fusionfd;
    pollfd.events = POLLIN;

    return &pollfd;
}

bool checkArg(string _arg, int argc, char* args[]) {
    const char* arg = _arg.c_str();
    for (int lp = 1; lp < argc; lp++) {
        if (!strcmp(arg, args[lp])) {
            return true;
        }
    }

    return false;
}
