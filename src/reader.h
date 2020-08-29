#ifndef READER_H
#define READER_H

#include <QWidget>
#include <QGraphicsScene>
#include <QDir>
#include <QImage>
#include <QLabel>
#include <QSettings>
#include <QDebug>

namespace Ui {
class Reader;
}

class Reader : public QWidget
{
    Q_OBJECT

signals:
    void zoomChanged(int newHeight);
    void pageChanged();
    void destroyed();

public slots:
    void resetZoom();
    void jumpToPage(int page);

public:
    explicit Reader(int, QStringList, QDir);
    ~Reader();
    int getImageCount() { return this->length; }

protected:
    virtual void paintEvent(QPaintEvent *e);

private:
    Ui::Reader *ui;
    int length;
    int currIndex;
    double scaleFactor = 1.0;
    QStringList images;
    QPixmap pm;
    QDir directory;
    QLabel image2;
    QSettings settings;
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void nextImage();
    void previousImage();
    void loadInitialImage();

};

#endif // READER_H
