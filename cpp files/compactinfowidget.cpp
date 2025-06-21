#include "CompactInfoWidget.h"
#include "openglcanvaswidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFontDatabase>
#include <QIntValidator>
#include <QStyle>
#include <QStyleOption>

CompactInfoWidget::CompactInfoWidget(QWidget* parent)
    : QWidget(parent)
{
    // Monospace font
    QFont monoFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);

    numberLabel = new QLabel("000 | 000");
    numberLabel->setFont(monoFont);
    numberLabel->setAlignment(Qt::AlignCenter);

    CurveSize = new QLabel("0");

    dynamicTextLabel = new QLabel("Status...");
    dynamicTextLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout* labelsLayout = new QVBoxLayout();
    labelsLayout->setContentsMargins(0, 0, 0, 0);
    labelsLayout->setSpacing(2);
    labelsLayout->addWidget(numberLabel);
    labelsLayout->addWidget(dynamicTextLabel);
    labelsLayout -> addWidget(CurveSize);

/*

    inputBox = new QLineEdit();
    inputBox->setValidator(new QIntValidator(0, 99999, inputBox));
    inputBox->setFixedWidth(60); // Reasonable width
    inputBox->setPlaceholderText("Value");
    inputBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    connect(inputBox, &QLineEdit::textChanged, this, &CompactInfoWidget::handleInputChanged);
*/
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(4, 0, 4, 0);
    mainLayout->setSpacing(8);
    mainLayout->addLayout(labelsLayout);
    //mainLayout->addWidget(inputBox);
}

void CompactInfoWidget::setFixedNumbers(int left, int right)
{
    numberLabel->setText(QString("%1 | %2").arg(left, 3, 10, QLatin1Char('0'))
                             .arg(right, 3, 10, QLatin1Char('0')));
}

void CompactInfoWidget::setDynamicText(const QString& text)
{
    dynamicTextLabel->setText(text);
}

void CompactInfoWidget::changedSize(int sizer)
{
    CurveSize->setText(QString("%1").arg(sizer, 1, 10, QLatin1Char('0')));

}
/*
QString CompactInfoWidget::getInputValue() const
{
    return inputBox->text();
}

void CompactInfoWidget::handleInputChanged(const QString& text)
{
    bool ok;
    int value = text.toInt(&ok);
    if (ok)
        emit inputValueChanged(value);
}
*/
