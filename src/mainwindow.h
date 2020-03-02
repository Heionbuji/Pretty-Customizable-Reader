#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWindow>
#include <QScreen>
#include <QDir>
#include <QWheelEvent>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include "reader.h"
#include <QScrollArea>
#include <QDebug>
#include <QScrollBar>
#include "previewblock.h"
#include <QAction>
#include <QListWidget>
#include <QPointer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public slots:
    void readerZoom(int newHeight) {
        scr->resize(scr->width(), newHeight);
    }
    void resetScroll() {
        scr->verticalScrollBar()->setValue(0);
    }

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void loadStuff(QString);
    void on_actionSettings_triggered();

private:
    Ui::MainWindow *ui;
    QScreen *screen;
    Reader *reader = nullptr;
    QScrollArea *scr = nullptr;
    QGridLayout layout;
    QAction *resetZoomAct;
    QMenu *menuView;
    QString maindir = "E:\\Manga";
    QList<PreviewBlock*> blocks;
    void setupLayout(QString, bool);
    void setupActions();
    void cleanUpActions();
    void clearLayout(QLayout* layout, bool deleteWidgets);
    void connectBlocks(bool);
    void loadReader(QString);
    void loadDefaultSettings();
};

#endif // MAINWINDOW_H
