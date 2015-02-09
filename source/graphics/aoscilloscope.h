#ifndef AOSCILLOSCOPE_H
#define AOSCILLOSCOPE_H

#include <QMenu>
#include <QToolButton>
#include <QWidgetAction>

#include "aplot.h"
#include "../acustomslider.h"

class AOscilloscope : public APlot
{
public:
    AOscilloscope(AView *view);
    ~AOscilloscope();

    void addData(const QVector<double> &data) override;

    virtual APlotType plotType() const override;
    virtual QToolBar *controlWidget(QWidget *parent) override;

    virtual void initialize() override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

    //control
    void setBufferSize(int size);
    void updateControlsData();

private:
    QPointer<AView> m_view;
    QPointer<QToolBar> m_tools;
    QPointer<ACustomSlider> m_slider;


    QVector<double> m_data;
    QVector<double> m_buffer;

    int m_bufferSize;
    QPair<int, int> m_bufferSizeRange;

    double m_xRes;
    double m_ampl;

    QToolBar *createTools(QWidget *parent);
};

#endif // AOSCILLOSCOPE_H
