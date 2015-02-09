#ifndef ASPECTROGRAM_H
#define ASPECTROGRAM_H

#include <QScrollBar>

#include "aplot.h"
#include "../acustomslider.h"
#include "../affttransformer.h"

class ASpectrogram : public APlot
{
public:
    ASpectrogram(AView *view);
    ~ASpectrogram();

    void addData(const QVector<double> &data) override;

    APlotType plotType() const override;
    QToolBar *controlWidget(QWidget *parent) override;

    void initialize() override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    void setBufferSizeExp(int size);
    void setRecordTime(int time); //sec
    void updateControlsData();

private:   
    QImage m_image;
    QPointer<AView> m_view;
    QPointer<QToolBar> m_tools;
    QPointer<ACustomSlider> m_frameSizeSlider;
    QPointer<ACustomSlider> m_recotdTimeSlider;

    QVector<double> m_buffer;
    QVector<double> m_data;
    QPair<int, int> m_bufferSizeRangeExp;

    int m_bufferSize;
    int m_bufferSizeExp;
    int m_recordTime;

    double m_xRes;
    double m_yRes;
    double m_ampl;

    int m_currentLine;

    bool m_logScale;
    bool m_tracking;

    void fillImage();
    int color(int v);

    void setLogScale(bool value);
    QToolBar *createTools(QWidget *parent);
};

#endif // ASPECTROGRAM_H
