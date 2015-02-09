#ifndef ASPECTROGRAPH_H
#define ASPECTROGRAPH_H

#include <QToolButton>
#include <QWidgetAction>

#include "aplot.h"
#include "../acustomslider.h"
#include "../affttransformer.h"

class ASpectrograph : public APlot
{
public:
    ASpectrograph(AView *view);
    ~ASpectrograph();

    void addData(const QVector<double> &data) override;

    virtual APlotType plotType() const override;
    virtual QToolBar *controlWidget(QWidget *parent) override;

    virtual void initialize() override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

    void setBufferSizeExp(int size);
    void updateControlsData();

private:
    QPointer<AView> m_view;
    QPointer<QToolBar> m_tools;
    QPointer<ACustomSlider> m_slider;

    QVector<double> m_data;
    QVector<double> m_buffer;

    QPair<int, int> m_bufferSizeRangeExp;
    int m_bufferSize;
    int m_bufferSizeExp;

    double m_xRes;
    double m_yRes;
    double m_ampl;

    QToolBar *createTools(QWidget *parent);
};

#endif // ASPECTROGRAPH_H
