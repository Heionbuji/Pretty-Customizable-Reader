#ifndef PREVIEWBLOCK_H
#define PREVIEWBLOCK_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>

namespace Ui {
class PreviewBlock;
}

class PreviewBlock : public QWidget
{
    Q_OBJECT

signals:
    void clicked(QString dir);

public:
    explicit PreviewBlock() {}
    explicit PreviewBlock(QString cover, QString title, QString path);
    ~PreviewBlock();

private:
    Ui::PreviewBlock *ui;
    QLabel coverLabel;
    QVBoxLayout layout;
    QString path;
    QString title;
    int width = 300;
    int height = 350;

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void loadImage(QString path);
};

#endif // PREVIEWBLOCK_H
