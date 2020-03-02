#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "QDebug"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    qDebug() << "constructor";
}

SettingsWindow::~SettingsWindow()
{
    qDebug() << "destructor";
    delete ui;
}
