#include "widget.h"

#include <QApplication>
#include <QDebug>
#include <chrono>
#include <thread>
#include <string>
#include <iostream>

extern "C" {
#include <spnav.h>
}

using std::to_string;

pollfd* fusionSetup();

int main(int argc, char *argv[])
{
    daemon(0, 1);
    bool isFusion = false;
    pollfd* fusionfd = nullptr;

    QApplication a(argc, argv);
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
            .fusionAction = "DrawCircle"
        }
    };


    if (argc >= 2 && !strcmp(argv[1], "--fusion360")) {
        isFusion = true;
        if ((fusionfd = fusionSetup()) == nullptr) {
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
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            if (spnav_poll_event(&sev) == SPNAV_EVENT_BUTTON && sev.button.press == 0) {
                QPoint cLoc = QCursor::pos();
                menus[sev.button.bnum]->setGeometry(
                            cLoc.x() - (lMenu.width() / 2),
                            cLoc.y() - (lMenu.height() / 2),
                            cLoc.x() + (lMenu.width() / 2),
                            cLoc.y() + (lMenu.height() /2)
                            );
                menus[sev.button.bnum]->setWindowState(Qt::WindowMinimized);
                menus[sev.button.bnum]->activateWindow();
                menus[sev.button.bnum]->raise();
                menus[sev.button.bnum]->activateWindow();
                menus[sev.button.bnum]->show();
                a.exec();
                spnav_remove_events(SPNAV_EVENT_BUTTON);
            }
        }
    } else {
        printf("Failed to connect to spacenavd, exiting...");
        fflush(stdout);
    }

    spnav_close();
    return 0;
}

// Sets up fusionfd and returns pointer to fd object
pollfd* fusionSetup() {
    std::cout <<  "Initializing connection to Fusion360..." << std::endl;

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        std::cout << "Could not create socket, exiting..." << std::endl;
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

    std::cout << "Waiting for connection..." << std::endl;
    while ((fusionfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) < 0) {}

    std::cout << "Connection Accepted." << std::endl;

    static struct pollfd pollfd;
    pollfd.fd = fusionfd;
    pollfd.events = POLLIN;

    return &pollfd;
}
