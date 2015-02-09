#ifndef AVIEW_H
#define AVIEW_H

#include <QGestureEvent>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPointer>
#include <QResizeEvent>
#include <QWheelEvent>

#include "fft.h"

#include "aoscilloscope.h"
#include "aspectrograph.h"
#include "aspectrogram.h"

#include "../ainputdevice.h"

class AView : public QGraphicsView
{
    Q_OBJECT
public:
    AView(AInputDevice *device, QWidget *parent = 0, APlotType type = APlotType::NoPlot);
    ~AView();

    APlotType plotType() const;
    void setPlotType(APlotType type);

    //Returns a toolbar with control tools for the plot. The toolbar will be removed automaticaly during
    //the plot type is changed.
    QToolBar *getControlTools(QWidget *parent = nullptr);

    AInputDevice *inputDevice();
    void setInputDevice(AInputDevice *device);

    //Updates sceneRect according of the plot size.
    void updateSceneSize();


    //Style properties
    void setColors(const QColor &backgroundColor,
                   const QColor &lineColor,
                   const QColor &textColor,
                   const QColor &meshColor);

    const QPen &linePen();
    const QPen &textPen();
    const QPen &meshPen();
    const QBrush &backgroundBrush();


protected:
    void mouseDoubleClickEvent(QMouseEvent *) override;
    void resizeEvent(QResizeEvent *re) override;
    void wheelEvent(QWheelEvent *we) override;

    bool event(QEvent *event) override;
    virtual bool gestureEvent(QGestureEvent *event);

private:
    const double scaleStep = 1.5;

    QPointer<AInputDevice> m_inputDevice;
    APlot *m_plot = nullptr; //Can't use QPointer because the Plot type isn't a child of the QObject type

    QPen m_linePen{QPen(Qt::black, 0)};
    QPen m_meshPen{QPen(Qt::gray, 0, Qt::DashLine)};
    QPen m_textPen{QPen(Qt::black, 0)};
    QBrush m_backgroundBrush{QBrush(Qt::white)};

    QRectF unmapedSceneRect();

public slots:
    void setData();

    void fitInWidth();
    void fitInHeight();
};

#endif // AVIEW_H
