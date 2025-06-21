#include "curvedialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QColorDialog>

CurveCreationDialog::CurveCreationDialog(QWidget* parent) : QDialog(parent), selectedColor(Qt::black)
{
    setWindowTitle("Dodaj nową krzywą");

    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Typ krzywej:"));
    typeComboBox = new QComboBox(this);
    typeComboBox->addItems({"Bézier", "NIFS3", "OIFS3"});
    layout->addWidget(typeComboBox);

    layout->addWidget(new QLabel("Nazwa krzywej:"));
    nameEdit = new QLineEdit(this);
    layout->addWidget(nameEdit);

    layout->addWidget(new QLabel("Kolor krzywej:"));
    colorButton = new QPushButton("Wybierz kolor", this);
    layout->addWidget(colorButton);
    connect(colorButton, &QPushButton::clicked, this, &CurveCreationDialog::chooseColor);

    layout->addWidget(new QLabel("Grubość krzywej:"));
    widthSlider = new QSlider(Qt::Horizontal, this);
    widthSlider->setRange(1, 100);
    widthSlider->setValue(10);
    layout->addWidget(widthSlider);

    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    QPushButton* okButton = new QPushButton("Dodaj", this);
    QPushButton* cancelButton = new QPushButton("Anuluj", this);
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);
    layout->addLayout(buttonsLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

QString CurveCreationDialog::getCurveName() const {
    return nameEdit->text();
}

QString CurveCreationDialog::getCurveType() const {
    return typeComboBox->currentText();
}

QColor CurveCreationDialog::getCurveColor() const {
    return selectedColor;
}

float CurveCreationDialog::getCurveWidth() const {
    return widthSlider->value() / 10.0f;
}

void CurveCreationDialog::chooseColor()
{
    QColor color = QColorDialog::getColor(selectedColor, this, "Wybierz kolor");
    if(color.isValid())
    {
        selectedColor = color;
        colorButton->setStyleSheet(QString("background-color: %1").arg(color.name()));
    }
}
