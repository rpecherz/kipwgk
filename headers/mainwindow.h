#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "animationmenu.h"
#pragma once

#include <QMainWindow>
#include <QVulkanInstance>
#include <QVulkanWindow>
#include <QComboBox>
#include <QToolBar>
#include <QListWidget>
#include <QLabel>
#include "openglcanvaswidget.h"



QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void changeBackground(const QString& imagePath);
    ~MainWindow();

private:
    void setupBezierToolbar();
    bool toolbar_visible = false,wasSaved = false;
    int howmany = -1;
    Ui::MainWindow *ui;
    OpenGLCanvasWidget* canvas;
    void setupMenus();
    void setupToolbar();
    void setupCanvas();
    void setupAnimationMenu();
    QVulkanInstance vulkanInstance;
    QVulkanWindow *vulkanWindow;
    QComboBox* curveListComboBox = nullptr;
    QComboBox* BezieryDoLaczenia = nullptr;
    QToolBar* toolbar = new QToolBar("Krzywe", this);
    QToolBar* bezierskitoolbar = new QToolBar("Bezier Menu", this);
    AnimationMenu* AniMenu = nullptr;
    void SaveToPNG();
};



#endif
