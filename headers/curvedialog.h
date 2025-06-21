#pragma once

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QColor>
#include <QSlider>
#include <QPushButton>

class CurveCreationDialog : public QDialog
{
    Q_OBJECT

public:
    CurveCreationDialog(QWidget* parent = nullptr);

    QString getCurveName() const;
    QString getCurveType() const;
    QColor getCurveColor() const;
    float getCurveWidth() const;

private:
    QLineEdit* nameEdit;
    QComboBox* typeComboBox;
    QPushButton* colorButton;
    QSlider* widthSlider;

    QColor selectedColor;

private slots:
    void chooseColor();
};
