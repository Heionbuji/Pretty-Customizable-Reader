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
#include "menubar.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void toggleMenu();

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
    void goBack();

private:
    Ui::MainWindow *ui;
    QScreen *screen;
    // menubar stuff
    QMenuBar *menuBar = new MenuBar();
    QMenu *file = menuBar->addMenu("&File");
    QMenu *view = menuBar->addMenu("&View");
    QMenu *tools = menuBar->addMenu("&Tools");

    Reader *reader = nullptr;
    QScrollArea *scr = nullptr;
    QGridLayout layout;
    QAction *resetZoomAct;
    QMenu *menuView;
    QString maindir = "E:\\Manga";
    QList<PreviewBlock*> blocks;
    QWidget *temp = new QWidget();
    bool atTop = true; // temporary solution (hopefully)
    void setupLayout(QString, bool);
    void setupActions();
    void setupMenuBar();
    void cleanUpActions();
    void clearLayout(QLayout* layout, bool deleteWidgets);
    void connectBlocks(bool);
    void loadReader(QString);
    void loadDefaultSettings();
    void loadSettings();
};

#endif // MAINWINDOW_H
