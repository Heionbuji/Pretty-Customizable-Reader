#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QPixmap>
#include <QGridLayout>
#include <QGraphicsView>
#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QWidget>
#include <QPushButton>
#include <QRect>
#include <QPoint>
#include <QDirIterator>
#include <QSettings>
#include "settingswindow.h"

MainWindow::MainWindow(QWidget *parent) : // TODO: clean up includes
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QCoreApplication::setOrganizationName("Pretty Customizable Reader");
    QCoreApplication::setApplicationName("PCR");
    QSettings settings;
    if(settings.value("mainWindow/bg-color").toInt() == 0) {
        loadDefaultSettings();
    }
    scr = new QScrollArea(this);
    setupLayout(maindir, true);
    ui->menuView->setEnabled(false);
    ui->menuTools->addAction("Settings", this, &MainWindow::on_actionSettings_triggered);
    scr->setStyleSheet("background-color: " + settings.value("mainWindow/bg-color").toString());
    scr->setAlignment(Qt::AlignCenter);
    setCentralWidget(scr);
}

MainWindow::~MainWindow()
{
    if(reader != nullptr) {
        QObject::disconnect(reader, &Reader::destroyed, this, &MainWindow::cleanUpActions);
        // because the signal is still caught after ui is deleted
    }

    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QDir directory(QFileDialog::getExistingDirectory(this, QStringLiteral("Select directory")));
    if(directory.path() != ".") {
        loadReader(directory.absolutePath());
    }
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsWindow *window = new SettingsWindow(); // memory leak
    window->show();
}

void MainWindow::setupLayout(QString dir, bool asd) // naming
{
    QWidget *container = new QWidget();
    int rowCount = 6;
    QDirIterator it(dir);
    int counter = 0;
    scr->setAlignment(Qt::AlignCenter);
    while(it.hasNext()) {

        it.next();
        if(it.fileName() == "." || it.fileName() == "..") {
            continue; // is there really no better way to do this? I saw a NoDotorDotDot flag somewhere
        }
        QString title = it.fileName();
        QString coverPath;
        QDirIterator iter(it.filePath(), QStringList() << "*.jpg" << "*.JPG" << "*.png", QDir::NoFilter, QDirIterator::Subdirectories);
        coverPath = iter.next();
        QString path = iter.path();
        PreviewBlock *block = new PreviewBlock(coverPath, title, path);
        layout.addWidget(block, counter == 0 ? 0 : counter / rowCount , counter % rowCount );
        blocks.append(block);
        counter += 1;

    }
    container->setLayout(&layout);
    scr->setWidget(container);

    connectBlocks(asd);
}

void MainWindow::setupActions()
{
    resetZoomAct = this->ui->menuView->addAction("Reset Zoom", reader, &Reader::resetZoom);
    ui->menuView->setEnabled(true);
}

void MainWindow::cleanUpActions()
{
    ui->menuView->removeAction(resetZoomAct);
    ui->menuView->setEnabled(false);
}

void MainWindow::loadStuff(QString path) // epic naming
{
    clearLayout(&layout, true);
    qDeleteAll(blocks);
    blocks.clear(); // are both of these necessary?
    setupLayout(path, false);
}

void MainWindow::clearLayout(QLayout* layout, bool deleteWidgets)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (deleteWidgets)
        {
            if (QWidget* widget = item->widget())
                widget->deleteLater();
        }
        if (QLayout* childLayout = item->layout())
            clearLayout(childLayout, deleteWidgets);
        delete item;
    }
}

void MainWindow::connectBlocks(bool mode)
{
    if(mode) {
        for(int i = 0; i < blocks.length(); i++) {
            QObject::connect(blocks[i], &PreviewBlock::clicked, this, &MainWindow::loadStuff);
        }
    } else {
        for(int i = 0; i < blocks.length(); i++) {
            QObject::connect(blocks[i], &PreviewBlock::clicked, this, &MainWindow::loadReader);
        }
    }

}

void MainWindow::loadReader(QString path)
{
    if(!(reader == nullptr)) {
        delete(reader);
    }
    QDir directory(path);
    QStringList images(directory.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png",QDir::Files)); // not sure if the capitalization matters here
    this->setWindowTitle(directory.dirName());
    int length = images.length();

    this->reader = new Reader(length, images, directory);
    QSettings settings; // consider moving this to be a member variable so don't have to keep creating a new one
    if(settings.value("reader/resetScrollOnPageChange").toBool()) {
        QObject::connect(reader, &Reader::pageChanged, this, &MainWindow::resetScroll); // maybe move to setupactions
    }

    QObject::connect(reader, &Reader::destroyed, this, &MainWindow::cleanUpActions);
    clearLayout(&layout, true);

    scr->takeWidget(); // what happens to this widget? this is probably not good
    scr->setWidget(reader);

    setupActions();
}

void MainWindow::loadDefaultSettings() // need a naming scheme for these
{
    QSettings settings;
    settings.setValue("mainWindow/bg-color", "rgba(40,38,30,.95)");
    settings.setValue("reader/resetScrollOnPageChange", true);
    settings.sync();
}
