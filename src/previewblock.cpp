#include "previewblock.h"
#include "ui_previewblock.h"
#include <QPainter>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>

PreviewBlock::PreviewBlock(QString cover, QString title, QString path) :
    QWidget(),
    ui(new Ui::PreviewBlock)
{
    ui->setupUi(this); // rework this to be just a QLabel or something? not painting on the image itself (no need to change dpi etc). (fixed by using pixmap at least partly)

    this->setMaximumSize(width, height);
    this->setMinimumSize(width, height);
    coverLabel.resize(width, height);
    this->path = path;
    this->title = title;
    layout.addWidget(&coverLabel);
    this->setLayout(&layout);

    QFuture<void> future = QtConcurrent::run(this, &PreviewBlock::loadImage, cover);

}

PreviewBlock::~PreviewBlock()
{
    delete ui;
}

void PreviewBlock::mousePressEvent(QMouseEvent *)
{
    emit clicked(this->path);
}

void PreviewBlock::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(0, 0, width, height, QBrush(QColor(20, 20, 20, 180)));
}

void PreviewBlock::loadImage(QString path)
{
    QPixmap image(path);
    if(image.isNull()) {
        image = QPixmap(path, "PNG"); // I had a problem where a PNG had a .jpg extension and it wouldn't load,
        if(image.isNull()) {          // this is a bandaid solution for that particular case.
            return;                   // try imagereader?
        }
    }
    if(image.width() > image.height()) {
        image = image.copy(0, 0, image.width() / 2, image.height());
    }
    image = image.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPainter p(&image);
    p.fillRect(0, height - height / 5, width, height / 5, QBrush(QColor(0, 0, 0, 180)));
    p.setPen(QPen(Qt::white));
    p.setFont(QFont("Meiryo", 12, QFont::Bold));
    p.drawText(QRect(1, height - height / 5, width - 21, height / 5), Qt::AlignCenter | Qt::TextWrapAnywhere, title);
    this->coverLabel.setPixmap(image);

}
