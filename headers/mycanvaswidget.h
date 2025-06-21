#ifndef MYCANVASWIDGET_H
#define MYCANVASWIDGET_H

#pragma once


#include <QWidget>

class MyCanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyCanvasWidget(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent* event) override;

signals:
};

#endif // MYCANVASWIDGET_H
