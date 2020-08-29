#include "pagejumpwindow.h"
#include "ui_pagejumpwindow.h"

PageJumpWindow::PageJumpWindow(QWidget *parent, Reader *reader) :
    QWidget(parent),
    ui(new Ui::PageJumpWindow)
{
    ui->setupUi(this);
    ui->pageNum->setPlaceholderText("0 - " + QString::number(reader->getImageCount()));
    QObject::connect(this, &PageJumpWindow::jumpPage, reader, &Reader::jumpToPage);
}

PageJumpWindow::~PageJumpWindow()
{
    delete ui;
}

void PageJumpWindow::on_cancelButton_clicked()
{
    this->~PageJumpWindow();
}

void PageJumpWindow::on_okButton_clicked()
{
    emit jumpPage(this->ui->pageNum->toPlainText().toInt());
    this->~PageJumpWindow();
}
