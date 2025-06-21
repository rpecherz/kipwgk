#include "animationmenu.h"

AnimationMenu::AnimationMenu()
{
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint);


    setWindowTitle("Animation Controls");
    setFixedSize(300, 200);

    QLabel* labelPunkty = new QLabel("Punkty:", this);
    Punkty = new QSlider(Qt::Horizontal, this);
    Punkty->setRange(1, 50);
    Punkty->setValue(16);

    QLabel* labelSzybkosc = new QLabel("Szybkość:", this);
    Szybkość = new QSlider(Qt::Horizontal, this);
    Szybkość->setRange(1, 100);
    Szybkość->setValue(50);

    Player    = new QPushButton("▶️", this);
    Stop      = new QPushButton("⏹️", this);
    Rewind2   = new QPushButton("⏪", this);
    Rewind1   = new QPushButton("Prev", this);
    Forward1  = new QPushButton("Next", this);
    Forward2  = new QPushButton("⏩", this);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(labelPunkty);
    mainLayout->addWidget(Punkty);
    mainLayout->addWidget(labelSzybkosc);
    mainLayout->addWidget(Szybkość);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(Rewind2);
    buttonLayout->addWidget(Rewind1);
    buttonLayout->addWidget(Player);
    buttonLayout->addWidget(Forward1);
    buttonLayout->addWidget(Forward2);
    buttonLayout->addWidget(Stop);

    mainLayout->addLayout(buttonLayout);


    setLayout(mainLayout);
    connect(Player, &QPushButton::clicked, this, &AnimationMenu::playClicked);
    connect(Stop, &QPushButton::clicked, this, &AnimationMenu::stopClicked);
    connect(Szybkość,&QSlider::valueChanged,this, &AnimationMenu::timer_pace);

}
