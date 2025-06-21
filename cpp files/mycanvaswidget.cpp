#include "MyCanvasWidget.h"
#include <QPainter>

MyCanvasWidget::MyCanvasWidget(QWidget* parent)
    : QWidget(parent) {
    setStyleSheet("background-color: black;");
}

void MyCanvasWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::white, 2));

    painter.drawText(10, 20, "Canvas – tu będą krzywe");
}
