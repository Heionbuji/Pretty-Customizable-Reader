#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QWidget
{
    Q_OBJECT

signals:
    void settingsUpdated();

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

private slots:
    void on_cancelButton_clicked();

    void on_okButton_clicked();

private:
    void loadSettings();
    Ui::SettingsWindow *ui;
    QSettings settings;
};

#endif // SETTINGSWINDOW_H
