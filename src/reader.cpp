#include "reader.h"
#include "ui_reader.h"
#include <QMouseEvent>
#include <QPixmap>
#include <QPainter>
#include <QGraphicsPixmapItem>

Reader::Reader(int length, QStringList images, QDir directory) :
    QWidget(),
    ui(new Ui::Reader)
{
    ui->setupUi(this);
    this->length = length;
    this->images = images;
    this->directory = directory;
    currIndex = 0;
    loadInitialImage();
}


Reader::~Reader()
{
    delete ui;
    emit destroyed();
}

void Reader::mousePressEvent(QMouseEvent *event) // USE WINDOW SIZE FOR STUFF
{
    if(length > 0) {
        if(settings.value("reader/resetZoomOnPageChange").toBool()) {
            scaleFactor = 1;
        }
        if(event->x() > 950 && currIndex < length - 1) {
            nextImage();
        } else if(event->x() < 950 && currIndex != 0) { // nothing happens if you click on exactly 950
            previousImage();
        }
        // if reset scroll on page change is disabled, scroll doesn't work properly if
        // the pages are of different sizes. Maybe use percentages for scroll?
        if(settings.value("reader/resetScrollOnPageChange").toBool()) {
            emit pageChanged();
        }

    }
    event->accept();
}

void Reader::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() & Qt::ControlModifier) {
        if(event->angleDelta().y() > 0) {
            scaleFactor *= 1.25;

        } else if(event->angleDelta().y() < 0) {
            scaleFactor *= 0.8;
        }
        image2.resize(image2.pixmap()->size() * scaleFactor);
        this->resize(this->width(), image2.height() < 970 ? 970 : image2.height());
        this->repaint();
        event->accept();
    }

}

void Reader::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    QPainter p(this);
    p.setRenderHint(QPainter::SmoothPixmapTransform, true);
    p.drawPixmap(QRect(this->width() / 2 - image2.width() / 2, 0, image2.width(), image2.height()), *image2.pixmap());
}

void Reader::nextImage()
{
    currIndex++;
    pm.load(directory.absolutePath() + "/" + images[currIndex]); // !IMPORTANT doesn't handle images with the wrong extension
    if(pm.width() > 1900) {
        pm = pm.scaledToWidth(1900, Qt::SmoothTransformation); // maybe add a maximum size
    }
    image2.setPixmap(pm);
    image2.resize(image2.pixmap()->size() * scaleFactor);
    this->resize(1900, int(image2.pixmap()->height() * scaleFactor)); // casting to int might be a small problem
    this->repaint();
}

void Reader::previousImage()
{
    currIndex--;
    pm.load(directory.absolutePath() + "/" + images[currIndex]);
    if(pm.width() > 1900) {
        pm = pm.scaledToWidth(1900, Qt::SmoothTransformation);
    }
    image2.setPixmap(pm);
    image2.resize(image2.pixmap()->size() * scaleFactor);
    this->resize(1900, int(image2.pixmap()->height() * scaleFactor));
    this->repaint();
}

void Reader::loadInitialImage()
{
    image2.setScaledContents(true);
    pm.load(directory.absolutePath() + "/" + images[currIndex]);
    if(pm.width() > 1900) {
        pm = pm.scaledToWidth(1900, Qt::SmoothTransformation);
    }
    image2.setPixmap(pm);
    image2.resize(image2.pixmap()->size() * scaleFactor);
    this->resize(1900, image2.pixmap()->height());
    this->repaint();
}

void Reader::resetZoom()
{
    image2.resize(pm.size());
    scaleFactor = 1.0;
    this->resize(this->width(), image2.height() < 1000 ? this->height() : image2.height());
    this->repaint();
}

void Reader::jumpToPage(int page)
{
    this->currIndex = page - 1;
    nextImage();
}
