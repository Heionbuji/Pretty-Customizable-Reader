#include "menubar.h"
#include "ui_menubar.h"
#include <QDebug>

MenuBar::MenuBar(QWidget *parent) :
    QMenuBar(parent),
    ui(new Ui::MenuBar)
{
    ui->setupUi(this);
}

MenuBar::~MenuBar()
{
    delete ui;
}

void MenuBar::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    QSettings settings;
    if(settings.value("reader/fullscreen").toBool()) {
        makeNormal();
    }

}

void MenuBar::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    QSettings settings;
    if(settings.value("reader/fullscreen").toBool()) {
        makeSmall();
    }
}

void MenuBar::makeSmall()
{
    QSettings settings;
    this->setMaximumHeight(1);
    this->setStyleSheet("QMenuBar { background-color: " + settings.value("mainWindow/bg-color").toString() + "}"
                        "QMenu { background-color: #ABABAB }"
                        "QMenu::item:selected { background-color: #654321 }");
}

void MenuBar::makeNormal()
{
    this->setMaximumHeight(100);
    this->setStyleSheet("QMenu { background-color: #ffffff }"
                        "QMenu::item { color: #000000 }"
                        "QMenu::item:selected { background-color: #f4f4f4 }");
}
