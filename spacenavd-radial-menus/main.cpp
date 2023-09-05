#include "widget.h"

#define LOG(msg) std::cout << msg << std::endl;

extern "C" {
#include <spnav.h>
}

using std::to_string;

pollfd* fusionSetup();
bool checkArg(string arg, int argc, char* args[]);

int main(int argc, char *argv[])
{
    if (!checkArg("--no-daemon", argc, argv)) {
        daemon(1, 0);
        freopen("/var/log/spacenavd-radial-menus.log", "w", stdout);
        LOG("Running as daemon...");
    }

    bool isFusion = false;
    pollfd* fusionfd = nullptr;

    #include "config.h"

    if (checkArg("--fusion360", argc, argv)) {
        isFusion = true;
        if ((fusionfd = fusionSetup()) == nullptr) {
            LOG("Could not establish connection to Fusion360");
            return -1;
        }
    }

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("/usr/local/share/spacenavd-radial-menus/resources/spacenavd.svg"));

    Widget* menus[sizeof(actions)/sizeof(actions[0])];
    {
        uint8_t menu = 0;
        for (Widget::ButtonActions action : actions) {
            menus[menu] = new Widget(nullptr, action, isFusion, fusionfd);
            menu++;
        }
    }

    if(spnav_open() !=  -1) {
        spnav_event sev;

        while(!false) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            if (spnav_poll_event(&sev) == SPNAV_EVENT_BUTTON && sev.button.press == 0) {
                if (sev.button.bnum < sizeof(menus) / sizeof(menus[0])) {
                    QPoint cLoc = QCursor::pos();
                    menus[sev.button.bnum]->setGeometry(
                                cLoc.x() - (menus[0]->width() / 2),
                                cLoc.y() - (menus[0]->height() / 2),
                                cLoc.x() + (menus[0]->width() / 2),
                                cLoc.y() + (menus[0]->height() /2)
                                );
                    menus[sev.button.bnum]->activateWindow();
                    menus[sev.button.bnum]->raise();
                    menus[sev.button.bnum]->show();
                    a.exec();
                    spnav_remove_events(SPNAV_EVENT_BUTTON);
                }
                else if (sev.button.bnum > sizeof(menus) / sizeof(menus[0])) {
                    LOG("No Assigned Actions for Button " << sev.button.bnum << ". Ignoring...");
                }
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

    int listenfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
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
        LOG("Could not bind socket, exiting...");
        close(listenfd);
        return nullptr;
    }

    listen(listenfd, 1);

    int fusionfd;

    LOG("Waiting for connection...");
    const auto start = std::chrono::steady_clock::now();
    while ((fusionfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) < 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        if ((std::chrono::duration<double>)(std::chrono::steady_clock::now() - start) > (std::chrono::duration<double>)15) {
            LOG("Connection timed out.");
            return nullptr;
        }
    }

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
