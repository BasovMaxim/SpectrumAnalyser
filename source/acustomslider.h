#ifndef ACUSTOMSLIDER_H
#define ACUSTOMSLIDER_H

#include <QLabel>
#include <QMouseEvent>
#include <QSlider>
#include <QStyle>
#include <QWidget>
#include <QWidgetAction>

#include "fft.h"

namespace Ui {
class ACustomSlider;
}

// ACustomSlider class is a widget that presents sliderbar with increment and decrement buttons.

class ACustomSlider : public QWidget
{
    Q_OBJECT

public:
    // Creates slider with the title and the range.
    // Sets current value in the value. If autoTracking is true, the text of label will be update
    // according with current slider value
    ACustomSlider(const QString &title, const QPair<int, int> &range, int value, bool autoTracking = true,
                  QWidget *parent = 0);

    // Creates slider with the title and parent.
    ACustomSlider(const QString &title, QWidget *parent = 0);
    ~ACustomSlider();

    QLabel *label();
    QSlider *slider();

    //Sets the renge as integer values
    void setRange(const QPair<int, int> &range, int value);

    //Returns values if values had been assigned
    QList<int> &values();

    //Sets values. Every value assigned with slider position according the index in list.
    void setValues(const QList<int> &values, int currentValue);

    //Returns current value from values list according of the current slider position
    int currentValue() const;

private:
    Ui::ACustomSlider *ui;
    QList<int> m_values;
    QSlider *m_slider = nullptr;
};

#endif // ACUSTOMSLIDER_H
