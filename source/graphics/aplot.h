#ifndef APLOT_H
#define APLOT_H

#include <qmath.h>

#include <QGraphicsRectItem>
#include <QStyleOptionGraphicsItem>
#include <QBrush>
#include <QToolBar>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QPointer>

#include "fft.h"

class AView;

class APlot : public QGraphicsRectItem
{
public:  
    virtual void addData(const QVector<double> &data) = 0;

    virtual APlotType plotType() const = 0;
    virtual QToolBar * controlWidget(QWidget *parent) = 0;

    virtual void initialize() = 0;
};

class AEmptyPlot : public APlot
{
public:
    void addData(const QVector<double> &) {}

    virtual APlotType plotType() const { return APlotType::NoPlot; }
    virtual QToolBar * controlWidget(QWidget *) { return new QToolBar;}

    virtual void initialize() {}
};

#endif // APLOT_H
