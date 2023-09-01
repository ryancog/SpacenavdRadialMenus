#include "widget.h"
#include "./ui_widget.h"


Widget::Widget(QWidget *parent, ButtonActions _buttonActions, bool _isFusion, pollfd* _fusionfd)
    : QWidget(parent)
    , m_ui(new Ui::Widget)
{
    topButton   .mask = QPixmap("/usr/local/share/spacenavd-radial-menus/resources/spacenavd-top-menu.png");
    bottomButton.mask = QPixmap("/usr/local/share/spacenavd-radial-menus/resources/spacenavd-bottom-menu.png");
    leftButton  .mask = QPixmap("/usr/local/share/spacenavd-radial-menus/resources/spacenavd-left-menu.png");
    rightButton .mask = QPixmap("/usr/local/share/spacenavd-radial-menus/resources/spacenavd-right-menu.png");
    centerButton.mask = QPixmap("/usr/local/share/spacenavd-radial-menus/resources/spacenavd-center.png");

    topButton   .geometry = QRect(  53,   0, 244, 125 );
    bottomButton.geometry = QRect(  53, 225, 244, 125 );
    leftButton  .geometry = QRect(   0,  53, 125, 243 );
    rightButton .geometry = QRect( 225,  53, 125, 243 );
    centerButton.geometry = QRect( 135, 135,  80,  80 );

    topButton   .actions = _buttonActions.top;
    bottomButton.actions = _buttonActions.bottom;
    leftButton  .actions = _buttonActions.left;
    rightButton .actions = _buttonActions.right;
    centerButton.actions = {
        .name = "Close"
    };

    for (RButton* button : Buttons) {
        m_initButton(button);
    }


    isFusion = _isFusion;
    fusionFD = _fusionfd;

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);

    m_ui->setupUi(this);
}

void Widget::changeEvent(QEvent *event) {
    QWidget::changeEvent(event);
    if (event->type() == QEvent::ActivationChange) {
        if (!this->isActiveWindow()) {
            this->close();
        }
    }
}

void Widget::m_handleButton(RButton *button, ButtonStates state) {
    switch (state) {
    case ButtonStates::BUTTON_RELEASED:
        button->pressTimer->stop();
        close();
        if (button->qButton == centerButton.qButton) { return; }

        if (isFusion) {
            m_sendFusionAction(button->actions.fusionAction);
        } else {
            m_sendKeystroke(button->actions.keystroke);
        }
        break;

    case ButtonStates::BUTTON_PRESSED:
        button->pressTimer->start(5000);
        break;
    }
}

void Widget::m_sendFusionAction(string _action) {
    static char action[255];
    strncpy(action, _action.c_str(), 255);
    write(fusionFD->fd, action, sizeof(action));
}

void Widget::m_sendKeystroke(string _keystroke) {
    Display *display = XOpenDisplay(NULL);
    KeyCode code;

    char* keystroke = const_cast<char*>(_keystroke.c_str());
    char* keys[3] = {};
    int lp = 0;

    while (true) {
        keys[lp] = std::strtok(lp == 0 ? keystroke : NULL, "+");
        if (keys[lp] == NULL) { break; }

        code = XKeysymToKeycode(display, XStringToKeysym(keys[lp]));
        XTestFakeKeyEvent(display, code, False, 0);
        XFlush(display);
        XTestFakeKeyEvent(display, code, True, 0);
        XFlush(display);

        lp++;
    }
    lp = 0;
    while (true) {
        if (keys[lp] == NULL) { break; }
        code = XKeysymToKeycode(display, XStringToKeysym(keys[lp]));
        XTestFakeKeyEvent(display, code, False, 0);
        XFlush(display);

        lp++;
    }
}


void Widget::m_initButton(RButton* button) {
    button->qButton = new QPushButton(QString::fromLocal8Bit(button->actions.name.c_str()), this);
    button->qButton->setGeometry(button->geometry);
    button->qButton->setMask(button->mask.mask());
    button->qButton->setStyleSheet(
                "QPushButton{color:rgba(225,225,225,1);background-color:rgba(50,50,50,.8);border:none;}"
                "QPushButton:hover{color:rgba(225,225,225,1);background-color:rgba(35,35,35,0.8);border:none;}"
                "QPushButton:pressed{color:rgba(225,225,225,1);background-color:rgba(35,35,35,0.5);border:none;}"
                );
    button->qButton->setFont(QFont("Ubuntu", 12));

    if (button->qButton == centerButton.qButton) {
        button->pressTimer->setSingleShot(true);
        connect(button->pressTimer, &QTimer::timeout, this, []() {
            exit(0);
        });

        connect(button->qButton, &QPushButton::pressed, this, [=]{ Widget::m_handleButton(button, ButtonStates::BUTTON_PRESSED); });
    }
    connect(button->qButton, &QPushButton::released, this, [=]{ Widget::m_handleButton(button, ButtonStates::BUTTON_RELEASED); });
}

Widget::~Widget()
{
    delete m_ui;
}

