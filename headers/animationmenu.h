#ifndef ANIMATIONMENU_H
#define ANIMATIONMENU_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QPushButton>

class AnimationMenu : public QDialog
{
    Q_OBJECT
public:
    AnimationMenu();
private:
    //Casteljau
    QSlider* Punkty;
    QSlider* Szybkość;
    QPushButton* Player;
    QPushButton* Stop;
    QPushButton* Rewind1;
    QPushButton* Rewind2;
    QPushButton* Forward1;
    QPushButton* Forward2;
signals:
    void playClicked();
    void stopClicked();
    void timer_pace(int speed);

};

#endif // ANIMATIONMENU_H
