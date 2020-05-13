#ifndef MENUBAR_H
#define MENUBAR_H

#include <QWidget>
#include <QMenuBar>
#include <QSettings>

namespace Ui {
class MenuBar;
}

class MenuBar : public QMenuBar
{
    Q_OBJECT

public slots:
    void loaded(bool fullscreen) {
        if(fullscreen) {
            makeSmall();
        } else {
            makeNormal();
        }
    }

public:
    explicit MenuBar(QWidget *parent = nullptr);
    ~MenuBar();

private:
    Ui::MenuBar *ui;
    void makeSmall();
    void makeNormal();
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
};

#endif // MENUBAR_H
