#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>

class CompactInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompactInfoWidget(QWidget* parent = nullptr);

    void setFixedNumbers(int left, int right);
    void changedSize(int sizer);
    void setDynamicText(const QString& text);
     /*
    QString getInputValue() const;

signals:
    void inputValueChanged(int newValue);

private slots:
    void handleInputChanged(const QString& text);
*/
private:
    QLabel* numberLabel;
    QLabel* CurveSize;
    QLabel* dynamicTextLabel;
    //QLineEdit* inputBox;
};
