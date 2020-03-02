#include "settingswindow.h"
#include "ui_settingswindow.h"
#include <QSettings>

SettingsWindow::SettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    QSettings settings;
    this->ui->scrollResetCheckbox->setChecked(settings.value("reader/resetScrollOnPageChange").toBool());

}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}
