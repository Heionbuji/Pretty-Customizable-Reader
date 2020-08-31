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
#include <QShortcut>
#include <QMimeData>
#include <QInputDialog>
// TODO: clean up includes
// TODO: refactor to series listing and volume listing
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QCoreApplication::setOrganizationName("Pretty Customizable Reader");
    QCoreApplication::setApplicationName("PCR");
    this->setupMenuBar();
    QSettings settings;
    scr = new QScrollArea(this);
    if(settings.value("mainWindow/mainDir").toString() == "") {
        settings.setValue("mainWindow/mainDir", promptForDirectory());
        loadDefaultSettings();
    } else {
        loadSettings();
    }
    new QShortcut(QKeySequence(Qt::Key_Escape), this, SLOT(goBack()));
    new QShortcut(QKeySequence(Qt::Key_F11), this, SLOT(toggleFullscreen()));
    setAcceptDrops(true);
    setupLayout(maindir, true);
    scr->setAlignment(Qt::AlignCenter);
    scr->setFrameShape(QFrame::NoFrame);
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
    SettingsWindow *window = new SettingsWindow();
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->setFixedSize(600, 400);
    QObject::connect(window, &SettingsWindow::settingsUpdated, this, &MainWindow::loadSettings);
    window->show();
}

void MainWindow::handleJumpToPage()
{
    int maxPage = reader->getImageCount();
    bool error = true;
    QString errorMsg = "";
    while(error) {
        int page = QInputDialog::getInt(this, "Jump to page", "Jump to page: (0 - " + QString::number(maxPage) + ")" + "\n" + errorMsg);
        if(page >= 0 && page <= maxPage) {
            emit jumpTo(page);
            error = false;
        }
        else {
            errorMsg = "ERROR: Enter a valid number.";
        }

    }

}

void MainWindow::setupLayout(QString dir, bool asd) // naming
{
    QWidget *container = new QWidget();
    int rowCount = 6;
    QDirIterator it(dir);
    int counter = 0;
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
    resetZoomAct = view->addAction("Reset Zoom", reader, &Reader::resetZoom);
    view->addAction("Jump to Page", this, &MainWindow::handleJumpToPage);
    QObject::connect(this, &MainWindow::jumpTo, reader, &Reader::jumpToPage);
    view->setEnabled(true);
}

void MainWindow::setupMenuBar()
{
    QObject::connect(this, &MainWindow::toggleMenu, static_cast<MenuBar *>(menuBar), &MenuBar::loaded);
    file->addAction("Open", this, &MainWindow::on_actionOpen_triggered);
    view->setEnabled(false);
    tools->addAction("Settings", this, &MainWindow::on_actionSettings_triggered);
    setMenuBar(menuBar);
}

void MainWindow::cleanUpActions()
{
    view->removeAction(resetZoomAct);
    view->setEnabled(false);
}

void MainWindow::loadStuff(QString path) // epic naming
{
    //clearLayout(&layout, true);
    qDeleteAll(blocks);
    blocks.clear(); // are both of these necessary?
    atTop = false;
    setupLayout(path, false);
}

void MainWindow::clearLayout(QLayout* layout, bool deleteWidgets) // this may not be needed
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
    QObject::connect(reader, &Reader::pageChanged, this, &MainWindow::resetScroll); // maybe move to setupactions

    QObject::connect(reader, &Reader::destroyed, this, &MainWindow::cleanUpActions);

    this->temp = scr->takeWidget();
    scr->setWidget(reader);
    atTop = false;
    setupActions();
}

QString MainWindow::promptForDirectory() {
    QString dir = QFileDialog::getExistingDirectory(this, "First time setup: Select a base directory", "", QFileDialog::ShowDirsOnly);
    maindir = dir;
    return dir;
}

void MainWindow::loadDefaultSettings() // need a naming scheme for these
{
    QSettings settings;
    settings.setValue("mainWindow/bg-color", "#f528261e");
    settings.setValue("reader/resetScrollOnPageChange", true);
    settings.setValue("reader/resetZoomOnPageChange", false);
    settings.setValue("reader/fullscreen", false);
    settings.sync();
}

void MainWindow::loadSettings()
{
    QSettings settings;
    scr->setStyleSheet("background-color: " + settings.value("mainWindow/bg-color").toString());
    if(settings.value("reader/fullscreen").toBool()) {
        this->setWindowState(Qt::WindowFullScreen);
        emit toggleMenu(true);
    } else {
        this->setWindowState(Qt::WindowMaximized);
        emit toggleMenu(false);
    }
    maindir = settings.value("mainWindow/mainDir").toString();
}

void MainWindow::toggleFullscreen()
{
    QSettings settings;
    if(settings.value("reader/fullscreen").toBool()) {
        this->setWindowState(Qt::WindowMaximized);
        settings.setValue("reader/fullscreen", false);
        emit toggleMenu(false);
    } else {
        this->setWindowState(Qt::WindowFullScreen);
        settings.setValue("reader/fullscreen", true);
        emit toggleMenu(true);
    }
    settings.sync();
}

void MainWindow::goBack()
{
    if(reader == nullptr && !atTop) {
        atTop = true;
        qDeleteAll(blocks);
        blocks.clear(); // are both of these necessary?
        setupLayout(maindir, true);
    } else if(!atTop) {
        delete(scr->takeWidget());
        reader = nullptr;
        scr->setWidget(temp);
        atTop = false;
    }

}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls() && event->mimeData()->urls().length() == 1) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event) {
    QList<QUrl> urls = event->mimeData()->urls();
    QString fileName = urls[0].toLocalFile();
    loadReader(fileName);
}
