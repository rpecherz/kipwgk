#include "mainwindow.h"
#include "beziercurve.h"
#include "./ui_mainwindow.h"
#include "openglcanvaswidget.h"
#include "curvedialog.h"
#include "compactinfowidget.h"

#include <QMenuBar>
#include <QRandomGenerator>
#include <QAction>
#include <QApplication>
#include <QGuiApplication>
#include <QVulkanInstance>
#include <QPalette>
#include <QPixmap>
#include <QFileDialog>
#include <QDir>
#include <QVulkanWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QMessageBox>
#include <QcolorDialog>
#include <cstdlib>
#include <QSlider>
#include <QCheckBox>
#include <QWidgetAction>
#include <QButtonGroup>
#include <QRadioButton>
#include <QStackedLayout>

std::string Filename;


CurveMode currentCurveMode = CurveMode::Bezier;
drawMode currentDrawMode = drawMode::Curve;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupCanvas();
    setupMenus();
    setupToolbar();
    setupBezierToolbar();
    setupAnimationMenu();
    setWindowTitle("Edytor Graficzny");
}

void MainWindow::changeBackground(const QString& imagePath)
{
    QPixmap bkgnd(imagePath);
    if (!bkgnd.isNull())
    {
        if (bkgnd.width() * this->height() > bkgnd.height() * this->width())
        {
            bkgnd = bkgnd.scaledToWidth(this->width(), Qt::SmoothTransformation);
        }
        else
        {
            bkgnd = bkgnd.scaledToHeight(this->height(), Qt::SmoothTransformation);
        }
        QPalette palette = this->palette();
        palette.setBrush(QPalette::Window, bkgnd);
        this->setPalette(palette);
    }
}

void MainWindow::setupMenus()
{
    QMenu* fileMenu = menuBar()->addMenu("Plik");
    QMenu* bgsetter = menuBar()->addMenu("Background");
    QMenu* Personalizator = menuBar() -> addMenu("Personalization");
    QMenu* Widok = menuBar() -> addMenu("Widok");

    QCheckBox* otoczka = new QCheckBox("Draw convex hull", this);
    otoczka->setChecked(false);

    connect(otoczka, &QCheckBox::toggled, this, [=](bool checked) {
        canvas->otoczka_if();
    });
    QWidgetAction* checkAction = new QWidgetAction(this);
    checkAction->setDefaultWidget(otoczka);
    Personalizator->addAction(checkAction);
    QCheckBox* kratka = new QCheckBox("Czy krata nie jest potrzebna?", this);
    kratka->setChecked(false);


    connect(kratka, &QCheckBox::toggled, this, [=](bool checked) {
        canvas->kratka_if();
    });

    QWidgetAction* kratkaAction = new QWidgetAction(this);
    kratkaAction->setDefaultWidget(kratka);
    Widok->addAction(kratkaAction);
    //todo alko widget


    fileMenu->addAction("Nowy projekt", [this]() {
        if(canvas)
        {
            delete canvas;
        }
        //Todo if saved ok else popup czy chcesz zapisac
        setupCanvas();

    });
    fileMenu->addAction("Wczytaj projekt", []()
    {
        // TODO: Implement loading project
    });
    fileMenu->addAction("Zapisz projekt", [this]()
    {
        auto* Pointsy = canvas->get_list();
        //for(auto* curve : Pointsy)

        wasSaved = true;
        // TODO: Implement saving project
    });
    fileMenu->addAction("Zapisz jako...", [this]()
    {
        wasSaved =  true;
        // TODO: Implement save as
    });

    bgsetter->addAction("Choose file", [this]()
    {
        QString fileName = QFileDialog::getOpenFileName(
            this,
            "Select Background Image",
            QDir::homePath(),
            "Images (*.png *.jpg *.jpeg *.bmp)"
            );
        if (!fileName.isEmpty())
        {
            changeBackground(fileName);
        }
    });
    bgsetter->addAction("Choose file for opengl", [this]()
    {
        QString fileName = QFileDialog::getOpenFileName
            (
            this,
            "Select Background Image",
            QDir::homePath(),
            "Images (*.png *.jpg *.jpeg *.bmp)"
            );
        if(!fileName.isEmpty())
        {
            QImage image(fileName);
            if (!image.isNull())
            {
                canvas->setBackgroundImage(image);
            }
        }
    });


    Personalizator->addAction("");
}



void MainWindow::setupAnimationMenu()
{
    AniMenu = new AnimationMenu();
    AniMenu -> setVisible(false);
    connect(AniMenu, &AnimationMenu::timer_pace, canvas, [=](int speed) {
        canvas->setTimerSpeed(60-speed);
    });
    connect(AniMenu, &AnimationMenu::playClicked, canvas, [=]()
    {
        //canvas->setTimerSpeed(speed);
        canvas->update();
    });
    connect(AniMenu, &AnimationMenu::stopClicked, canvas, [=]() {
        canvas->setTimerSpeed(0);
    });
}


void MainWindow::setupToolbar()
{
    std::vector<CurveEntry>* curves = canvas->get_list();
    toolbar->setOrientation(Qt::Vertical);
    addToolBar(Qt::LeftToolBarArea, toolbar);
    bezierskitoolbar->setOrientation(Qt::Vertical);
    addToolBar(Qt::LeftToolBarArea, bezierskitoolbar);
    bezierskitoolbar->setVisible(false);
    toolbar->setMaximumWidth(this->width()*0.2f);


    //toolbar -> setMovable(false);
    //toolbar -> setOrientation(Qt::Vertical);

    /*QComboBox* curveTypeComboBox = new QComboBox(toolbar);
    curveTypeComboBox->addItems({"Bézier", "NIFS3", "OIFS3"});
    toolbar->addWidget(new QLabel("Typ krzywej:", toolbar));
    toolbar->addWidget(curveTypeComboBox);*/

    toolbar->addSeparator();

    toolbar->addWidget(new QLabel("Aktywne krzywe:", toolbar));
    curveListComboBox = new QComboBox(toolbar);
    toolbar->addWidget(curveListComboBox);

    toolbar->addSeparator();
    QAction* addCurveAction = toolbar->addAction("➕ Dodaj krzywą");
    QAction* addPointAction = toolbar->addAction("➕ Dodaj punkt");
    QAction* removeLastPointAction = toolbar->addAction("➖ Usuń ostatni punkt");
    QAction* deleteCurveAction = toolbar->addAction("🗑️ Usuń krzywą");
    QAction* colorPickerAction = toolbar->addAction("🎨 Wybierz kolor");



    QSlider* grubosz = new QSlider(toolbar);
    toolbar->addWidget(grubosz);
    toolbar->addSeparator();
    auto* infoWidget = new CompactInfoWidget();
    toolbar->addWidget(infoWidget);
    connect(canvas, &OpenGLCanvasWidget::PointAdded, this, [=](const QVector2D& pt)
    {
        qDebug() << "Last point:" << pt.x() << pt.y();
        infoWidget->setFixedNumbers(pt.x(), pt.y());
        infoWidget->setDynamicText(curveListComboBox->currentText());
    });

    grubosz->setOrientation(Qt::Horizontal);
    grubosz->setRange(1.0f, 10.0f);
    grubosz->setValue(10);

    //TODO musisz ogarnac ze currentindex zwraca tak naprawde numer indeksu a nie numerindeksu beziercurve i nifscurve
    connect(grubosz, &QSlider::valueChanged, this, [=](float value)
    {
        int currentIndex = curveListComboBox->currentIndex();
        if(currentIndex < 0 || currentIndex >= curves->size())
        {
            return;
        }
        float width = value / 10.0f;

        std::visit([&,this](auto&& curveObj)
        {
            curveObj.setWidth(width);
        }, (*curves)[currentIndex].curve);
        canvas->update();
    });

    /*connect(curveTypeComboBox, &QComboBox::currentTextChanged, this, [=](const QString& text)
    {
        if (text == "Bézier")
        {
            currentCurveMode = CurveMode::Bezier;
        }
        else if (text == "NIFS3")
        {
            currentCurveMode = CurveMode::NIFS3;
        }
        else if (text == "OIFS3")
        {
            currentCurveMode = CurveMode::OIFS3;
        }
    });*/


    connect(curveListComboBox, &QComboBox::currentIndexChanged, this, [=](int index)
    {
        if(index < 0)
            return;
        std::visit([&,this](auto&& curveObj)
        {
            using T = std::decay_t<decltype(curveObj)>;
            if constexpr (std::is_same_v<T, BezierCurve>)
            {
                bezierskitoolbar->setVisible(true);
            }
            else
                bezierskitoolbar->setVisible(false);
        }, (*curves)[index].curve);
        canvas->setCurrentCurveIndex(index);
        canvas->update();
    });

    connect(addPointAction, &QAction::triggered, this, [=]()
    {
        int index = canvas -> getCurrentCurveIndex();
        if(index < 0 || index >=curves->size())
            return;
        std::visit([&, this](auto&& curve)
        {
            float x = QRandomGenerator::global()->bounded(0, 500);
            float y = QRandomGenerator::global()->bounded(0, 500);
            curve.addControlPoint(QVector2D(x, y));
            using T = std::decay_t<decltype(curve)>;
            if constexpr (std::is_same_v<T, BezierCurve>)
            {
                canvas -> drawBezier();
            }
            else if constexpr(std::is_same_v<T, nifs3curve>)
                canvas -> drawnifs3();
            //todo jak dodasz bspline to tutaj masz dodac drawbspline
            else
            {}
                //canvas -> drawnifs3();
            emit canvas->sizchange();
        }, (*curves)[index].curve);

    });



    connect(canvas, &OpenGLCanvasWidget::sizchange, this, [=]() {
        int index = canvas->getCurrentCurveIndex();
        if (index < 0 || index >= curves->size())
        {
            infoWidget->changedSize(0);
            return;
        }
        std::visit([&, this](auto&& curve)
        {
            infoWidget->changedSize(curve.getControlPoints().size());
        }, (*curves)[index].curve);
    });



    //TODO musisz zmienić że dodajesz danego typu
    connect(addCurveAction, &QAction::triggered, this, [=]()
    {
        CurveCreationDialog dialog(this);
        if(dialog.exec() == QDialog::Accepted)
        {
            QString type = dialog.getCurveType();
            QString name = dialog.getCurveName().isEmpty() ? "Krzywa bez nazwy" : dialog.getCurveName();
            QColor color = dialog.getCurveColor();
            float width = dialog.getCurveWidth();

            if(type == "Bézier")
            {
                currentCurveMode = CurveMode::Bezier;
                bezierskitoolbar->setVisible(true);
            }
            else if(type == "NIFS3")
            {
                currentCurveMode = CurveMode::NIFS3;
                bezierskitoolbar->setVisible(false);
            }
            else if (type == "OIFS3")
            {
                currentCurveMode = CurveMode::OIFS3;
                bezierskitoolbar->setVisible(false);
            }
            canvas->addNewCurve(currentCurveMode);
            int index = curves->size() - 1;
            std::visit([&,this](auto&& curveObj)
            {
                curveObj.setColour(color.red(), color.green(), color.blue());
                curveObj.setWidth(width);
            }, (*curves)[index].curve);

            canvas->setCurrentCurveIndex(index);
            curveListComboBox->addItem(name);
            curveListComboBox->setCurrentIndex(curveListComboBox->count()-1);
            emit canvas->sizchange();
            canvas->update();
        }
    });


    connect(removeLastPointAction, &QAction::triggered, this, [=]()
    {
        //todo
        qDebug() << "is this ok ";
        canvas->removeLastPointFromCurrentCurve();
        canvas->update();
    });
    //TODO
    connect(deleteCurveAction, &QAction::triggered, this, [=]()
    {
        canvas->deleteCurrentCurve();
        curveListComboBox->removeItem(curveListComboBox->currentIndex());
        //curveListComboBox->addItem(QString("Krzywa %1").arg(i + 1));
        canvas->setCurrentCurveIndex(curves->empty() ? -1 : curves->size() - 1);
    });

    connect(colorPickerAction, &QAction::triggered, this, [=]()
    {
        //todo mozliwe zapetlenie
        while(true)
        {
            QColor chosenColor = QColorDialog::getColor(Qt::white, this, "Wybierz kolor krzywej");

            if(!chosenColor.isValid())
                return;

            int currentIndex = curveListComboBox->currentIndex();
            if(currentIndex < 0 || currentIndex >= static_cast<int>(curves->size()))
            {
                QMessageBox::warning(this, "Brak krzywej", "Wybierz krzywą do pokolorowania.");
                continue;
            }
            std::visit([&,this](auto&& curveObj)
            {
               curveObj.setColour(chosenColor.red(), chosenColor.green(), chosenColor.blue());
            }, (*curves)[currentIndex].curve);
            canvas->update();
            break;
        }
    });
}

//polacz go do canvas
void MainWindow::setupBezierToolbar()
{
    std::vector<CurveEntry>* curves = canvas->get_list();

    //toolbar -> setMovable(false);
    //toolbar -> setOrientation(Qt::Vertical);

    /*QComboBox* curveTypeComboBox = new QComboBox(toolbar);
    curveTypeComboBox->addItems({"Bézier", "NIFS3", "OIFS3"});
    toolbar->addWidget(new QLabel("Typ krzywej:", toolbar));
    toolbar->addWidget(curveTypeComboBox);*/

    bezierskitoolbar->addSeparator();

    bezierskitoolbar->addWidget(new QLabel("Aktywne krzywe:", bezierskitoolbar));
    //curveListComboBox = new QComboBox(bezierskitoolbar);
    //bezierskitoolbar->addWidget(curveListComboBox);

    bezierskitoolbar->addSeparator();
    QAction* c0lacznik = bezierskitoolbar->addAction("Połącz C0");
    QAction* c1lacznik = bezierskitoolbar->addAction("Połącz C1");
    QAction* c2lacznik = bezierskitoolbar->addAction("Połącz C2");
    bezierskitoolbar->addSeparator();
    BezieryDoLaczenia = new QComboBox(bezierskitoolbar);
    bezierskitoolbar->addWidget(BezieryDoLaczenia);




    QLineEdit* translateInput = new QLineEdit(bezierskitoolbar);
    translateInput->setPlaceholderText("dx, dy");
    QPushButton* translateButton = new QPushButton("Translate", bezierskitoolbar);
    bezierskitoolbar->addWidget(translateInput);
    bezierskitoolbar->addWidget(translateButton);
    connect(translateButton, &QPushButton::clicked, this, [=]() {
        int index = canvas->getCurrentCurveIndex();
        if(index<0 || index>=curves->size())
            return;
        QStringList parts = translateInput->text().split(',');
        if (parts.size() == 2)
        {
            bool ok1, ok2;
            float dx = parts[0].trimmed().toFloat(&ok1);
            float dy = parts[1].trimmed().toFloat(&ok2);
            if (ok1 && ok2 && canvas)
            {
                std::visit([=](auto& curve)
                {
                    if constexpr (std::is_same_v<std::decay_t<decltype(curve)>, BezierCurve>) {
                        curve.translateBezier(dx, dy);
                    }
                }, (*curves)[index].curve);
                canvas->drawBezier();
                canvas->update();
                translateInput->clear();
            }
        }
    });

    QLineEdit* rotateInput = new QLineEdit(bezierskitoolbar);
    rotateInput->setPlaceholderText("Degrees");
    QPushButton* rotateButton = new QPushButton("Rotate", bezierskitoolbar);
    bezierskitoolbar->addWidget(rotateInput);
    bezierskitoolbar->addWidget(rotateButton);
    connect(rotateButton, &QPushButton::clicked, this, [=]() {
        int index = canvas->getCurrentCurveIndex();
        if(index<0 || index>=curves->size())
            return;
        bool ok;
        float degrees = rotateInput->text().toFloat(&ok);
        if(ok)
        {
            std::visit([=](auto& curve) {
                if constexpr (std::is_same_v<std::decay_t<decltype(curve)>, BezierCurve>) {
                    curve.rotateBezier(degrees);
                }
            }, (*curves)[index].curve);
            canvas->drawBezier();
            canvas->update();
            rotateInput->clear();
        }
    });

    QLineEdit* scaleInput = new QLineEdit(bezierskitoolbar);
    scaleInput->setPlaceholderText("Scale factor");
    QPushButton* scaleButton = new QPushButton("Scale", bezierskitoolbar);
    bezierskitoolbar->addWidget(scaleInput);
    bezierskitoolbar->addWidget(scaleButton);
    connect(scaleButton, &QPushButton::clicked, this, [=]() {
        int index = canvas->getCurrentCurveIndex();
        if(index<0 || index>=curves->size())
            return;
        bool ok;
        float factor = scaleInput->text().toFloat(&ok);
        if (ok && canvas)
        {
            std::visit([=](auto& curve) {
                if constexpr (std::is_same_v<std::decay_t<decltype(curve)>, BezierCurve>) {
                    curve.scaleBezier(factor);
                }
            }, (*curves)[index].curve);
            canvas->drawBezier();
            canvas->update();
            scaleInput->clear();
        }
    });
    QComboBox* changeBezier = new QComboBox(bezierskitoolbar);
    bezierskitoolbar->addWidget(changeBezier);
    changeBezier->addItem("PwoChudy");
    changeBezier->addItem("De Casteljau");

    connect(changeBezier, &QComboBox::currentIndexChanged, this, [=](int index)
            {
                switch(index)
                {
                case 0:
                    canvas -> change_bezier_eval(index);
                    canvas -> drawBezier();
                    break;
                case 1:
                    canvas -> change_bezier_eval(index);
                    canvas -> drawBezier();
                    break;
                default:
                    break;
                }
            });

}

void MainWindow::SaveToPNG()
{
    QImage image = canvas->grabFramebuffer();
    QString fileName = QFileDialog::getSaveFileName(this, "Save Screenshot", "", "PNG Images (*.png)");
    if (!fileName.isEmpty()) {
        image.save(fileName);
    }
}



void MainWindow::setupCanvas()
{
    //todo zrobic ten aktualny wskaznik na prawej dolnej czesci canvasu xdd
    canvas = new OpenGLCanvasWidget(this);

    QWidget* container = canvas;

    container->setMinimumSize(this->size() * 0.7);

    QWidget* prawyPanel = new QWidget(this);
    prawyPanel->setMinimumWidth(this->width() * 0.12);
    prawyPanel->setMaximumWidth(this->width() * 0.12);
    QVBoxLayout* prawy =  new QVBoxLayout(prawyPanel);

    QWidget* topPanel = new QWidget(this);

        topPanel->setMinimumHeight(this->height()*0.1);
        QHBoxLayout* topLayout = new QHBoxLayout(topPanel);
        QLabel* drawModeLabel = new QLabel("&Draw Mode");
        QComboBox* ModeSelector = new QComboBox(topPanel);

        drawModeLabel->setBuddy(ModeSelector);

        topLayout->addWidget(drawModeLabel);
        topLayout->addWidget(ModeSelector);

        ModeSelector->addItem("Curve");
        ModeSelector->addItem("Surface");
        ModeSelector->addItem("Animation");
        topLayout->addStretch(1);

        QPushButton *saveButton = new QPushButton(QString::fromUtf8("💾"));
        topLayout->addStretch();
        saveButton->setFixedSize(40, 40);
        saveButton->setStyleSheet(
            "QPushButton {"
            "  font-size: 20px;"
            "  border-radius: 5px;"
            "  color: white;"
            "}"
            "QPushButton:hover { background: #bdc3c7; }"
            );

        topLayout->addWidget(saveButton);
        connect(saveButton,&QPushButton::clicked,this,[=]()
        {
            SaveToPNG();
        });

        connect(ModeSelector, &QComboBox::currentIndexChanged, this, [=](int index)
                {
                    switch(index)
                    {
                    case 0:
                        currentDrawMode = drawMode::Curve;
                        AniMenu -> setVisible(false);
                        toolbar -> setVisible(true);
                        break;
                    case 1:
                        currentDrawMode = drawMode::Surface;
                        AniMenu -> setVisible(false);
                        toolbar -> setVisible(false);
                        bezierskitoolbar -> setVisible(false);
                        break;
                    case 2:
                        currentDrawMode = drawMode::Animation;
                        prawyPanel -> setVisible(false);
                        AniMenu -> setVisible(true);
                        toolbar -> setVisible(false);
                        bezierskitoolbar -> setVisible(false);
                        break;
                    default:
                        break;
                    }
                });
        auto* wspolrzednik = new CompactInfoWidget();
        topLayout -> addWidget(wspolrzednik);
        wspolrzednik->setDynamicText("Współrzedne");


        connect(canvas, &OpenGLCanvasWidget::AktPozycja, this, [=](const QVector2D& pt)
        {
            wspolrzednik->setFixedNumbers(pt.x(), pt.y());
        });


        QHBoxLayout* mainLayout = new QHBoxLayout();
        mainLayout->setContentsMargins(0, 0, 0, 0);
        //mainLayout->addWidget(sidePanel);
        mainLayout->addWidget(container);
        mainLayout->addWidget(prawyPanel);

        QWidget* central = new QWidget(this);
        QVBoxLayout* outerLayout = new QVBoxLayout(central);
        outerLayout->setContentsMargins(0, 0, 0, 0);
        outerLayout->setSpacing(0);
        outerLayout->addWidget(topPanel);
        outerLayout->addLayout(mainLayout);

        setCentralWidget(central);



    /*
    QWidget* sidePanel = new QWidget(this);
    sidePanel->setMinimumWidth(this->width() * 0.12);
    sidePanel->setMaximumWidth(this->width() * 0.12);
    QVBoxLayout* menuLayout = new QVBoxLayout(sidePanel);
    sidePanel->setLayout(menuLayout);


    std::vector<QPushButton*> subbuttons1 =
        {
            new QPushButton("Dodaj losowy punkt kontrolny"),
            new QPushButton("Podziel"),
            new QPushButton("Dodaj punkt swobody")
        };

    QPushButton* mainButton1 = new QPushButton(QIcon(":\\lanna.jpg"), "Krzywa Beziera");
    for (auto& but : subbuttons1)
        but->setVisible(false);

    QPushButton* mainButton2 = new QPushButton(QIcon(":/icons/shape.png"), "Usuń wszystkie punkty");

    auto wrapCentered = [](QWidget* widget)
    {
        QHBoxLayout* hLayout = new QHBoxLayout;
        hLayout->addStretch();
        hLayout->addWidget(widget);
        hLayout->addStretch();
        return hLayout;
    };

    menuLayout->addWidget(mainButton1);
    for (auto& but : subbuttons1)
        menuLayout->addLayout(wrapCentered(but));
    menuLayout->addWidget(mainButton2);
    menuLayout->addStretch();

    for (auto& but : subbuttons1)
        but->setFixedSize(mainButton1->sizeHint());

    connect(mainButton1, &QPushButton::clicked, this, [=]() {
        bool visible = subbuttons1[0]->isVisible();
        for (auto& but : subbuttons1)
            but->setVisible(!visible);
    });

    connect(mainButton2, &QPushButton::clicked, this, [=]() {
        canvas->clearPoints();
    });
    */
}

MainWindow::~MainWindow() {
    delete ui;
}
