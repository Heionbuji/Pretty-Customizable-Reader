#include "settingswindow.h"
#include "ui_settingswindow.h"
#include <qdebug.h>

SettingsWindow::SettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    loadSettings();
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::on_cancelButton_clicked()
{
    this->~SettingsWindow();
}

void SettingsWindow::on_okButton_clicked()
{
    settings.setValue("reader/resetScrollOnPageChange", this->ui->scrollResetCheckbox->isChecked());
    settings.setValue("reader/resetZoomOnPageChange", this->ui->zoomResetCheckbox->isChecked());
    settings.setValue("mainWindow/bg-color", this->ui->backgroundColor->toPlainText());
    settings.setValue("reader/fullscreen", this->ui->fullscreenCheckbox->isChecked());
    settings.sync();
    this->~SettingsWindow();
}

void SettingsWindow::loadSettings()
{
    this->ui->scrollResetCheckbox->setChecked(settings.value("reader/resetScrollOnPageChange").toBool());
    this->ui->zoomResetCheckbox->setChecked(settings.value("reader/resetZoomOnPageChange").toBool());
    this->ui->backgroundColor->setPlainText(settings.value("mainWindow/bg-color").toString());
    this->ui->fullscreenCheckbox->setChecked(settings.value("reader/fullscreen").toBool());
}
