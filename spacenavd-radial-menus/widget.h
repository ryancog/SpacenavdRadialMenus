#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QWindow>
#include <QPushButton>
#include <QBitmap>
#include <QThread>
#include <QDebug>
#include <QString>

#include <netinet/in.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

#include <string>
#include <iostream>
#include <cstring>

using std::string;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
private:
    struct ButtonAction {
        string name;
        string keystroke;
        string fusionAction;
    };

    class RButton {
    public:
        QPushButton* qButton;
        ButtonAction actions;
        QPixmap mask;
        QRect geometry;
    };

    void m_handleButton(RButton*);
    void m_sendKeystroke(string);
    void m_sendFusionAction(string);
    void m_initButton(RButton*);
    void changeEvent(QEvent *event);

    bool isFusion;
    pollfd* fusionFD;
    Ui::Widget *m_ui;

    RButton topButton, bottomButton, leftButton, rightButton, centerButton;
    RButton* Buttons[5] = {
        &topButton,
        &bottomButton,
        &leftButton,
        &rightButton,
        &centerButton
    };

public:
    struct ButtonActions {
        ButtonAction top;
        ButtonAction bottom;
        ButtonAction left;
        ButtonAction right;
    };

    Widget(QWidget *parent = nullptr, ButtonActions buttonActions = {}, bool isFusion = false, pollfd* fusionfd = nullptr);
    ~Widget();
};
#endif // WIDGET_H
