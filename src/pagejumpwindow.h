#ifndef PAGEJUMPWINDOW_H
#define PAGEJUMPWINDOW_H

#include <QWidget>
#include "reader.h"

namespace Ui {
class PageJumpWindow;
}

class PageJumpWindow : public QWidget
{
    Q_OBJECT

signals:
    void jumpPage(int page);

public:
    explicit PageJumpWindow(QWidget *parent = nullptr, Reader *reader = nullptr);
    ~PageJumpWindow();

private slots:
    void on_cancelButton_clicked();

    void on_okButton_clicked();

private:
    Ui::PageJumpWindow *ui;
};

#endif // PAGEJUMPWINDOW_H
