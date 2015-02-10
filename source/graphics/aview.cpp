#include "aview.h"

AView::AView(AInputDevice *device, QWidget *parent, APlotType type)
    : QGraphicsView(new QGraphicsScene(), parent)
    , m_inputDevice{device}
{

//#ifndef DESKTOP
//    setTransformationAnchor(QGraphicsView::NoAnchor);
//    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//#else
    setTransformationAnchor(AnchorUnderMouse);
//#endif
    scale(1.0, -1.0); //Flip horizontal.

    setPlotType(type);
    setInputDevice(device);
    setDragMode(ScrollHandDrag);

    grabGesture(Qt::PanGesture);
    grabGesture(Qt::PinchGesture);
    grabGesture(Qt::SwipeGesture);
}

AView::~AView()
{
}

APlotType AView::plotType() const
{
    return m_plot->plotType();
}

void AView::setPlotType(APlotType type)
{
    if(m_plot)
    {
        scene()->removeItem(m_plot);
        delete m_plot;
        m_plot = nullptr;
    }

    switch (type) {
    case APlotType::Oscilloscope:
        m_plot = new AOscilloscope(this);
        break;
    case APlotType::Spectrograph:
        m_plot = new ASpectrograph(this);
        break;
    case APlotType::Spectrogram:
        m_plot = new ASpectrogram(this);
        break;
    default:
        m_plot = new AEmptyPlot();
        break;
    }

    scene()->addItem(m_plot);
    m_plot->initialize();

    updateSceneSize();
    fitInView(sceneRect());
}

QToolBar *AView::getControlTools(QWidget *parent)
{
    return m_plot->controlWidget(parent);
}

AInputDevice *AView::inputDevice()
{
    return m_inputDevice;
}

void AView::setInputDevice(AInputDevice *device)
{
    disconnect(device, &AInputDevice::dataIsReady, this, &AView::setData);
    m_inputDevice = device;
    connect(device, &AInputDevice::dataIsReady, this, &AView::setData);
    m_plot->initialize();
}

void AView::updateSceneSize()
{
    scene()->setSceneRect(m_plot->rect());

    if(unmapedSceneRect().width() < (double)viewport()->width())
        fitInWidth();
    if(unmapedSceneRect().height() < (double)viewport()->height())
        fitInHeight();
}

void AView::setColors(const QColor &backgroundColor, const QColor &lineColor, const QColor &textColor, const QColor &meshColor)
{
    m_backgroundBrush.setColor(backgroundColor);
    setBackgroundBrush(m_backgroundBrush);
    m_linePen.setColor(lineColor);
    m_textPen.setColor(textColor);
    m_meshPen.setColor(meshColor);
}

const QPen &AView::linePen()
{
    return m_linePen;
}

const QPen &AView::textPen()
{
    return m_textPen;
}

const QPen &AView::meshPen()
{
    return m_meshPen;
}

const QBrush &AView::backgroundBrush()
{
    return m_backgroundBrush;
}

void AView::mouseDoubleClickEvent(QMouseEvent *)
{
    updateSceneSize();
    fitInView(scene()->sceneRect());
}

void AView::resizeEvent(QResizeEvent *re)
{
    QGraphicsView::resizeEvent(re);
    updateSceneSize();
}

void AView::wheelEvent(QWheelEvent *we)
{
    QRectF r = unmapedSceneRect();

    double factor = we->delta() > 0 ? scaleStep : 1.0 / scaleStep;

    if(we->modifiers() & Qt::ControlModifier)
    {
        if(r.width() * factor > viewport()->rect().width())
            scale(factor, 1.0);
        else
            fitInWidth();
    }
    else
    {
        if(r.height() * factor > viewport()->rect().height())
            scale(1.0, factor);
        else
            fitInHeight();
    }

}

bool AView::event(QEvent *event)
{
    if(event->type() == QEvent::Gesture)
    {
        event->setAccepted(true);
        return gestureEvent(static_cast<QGestureEvent *>(event));
    }

    return QGraphicsView::event(event);
}

bool AView::gestureEvent(QGestureEvent *event)
{
    if (QGesture *pinch = event->gesture(Qt::PinchGesture))
    {
        double factor = static_cast<QPinchGesture *>(pinch)->scaleFactor();
        scale(factor, factor);
    }

    return true;
}

QRectF AView::unmapedSceneRect()
{
    return mapFromScene(scene()->sceneRect()).boundingRect();
}

void AView::setData()
{
    m_plot->addData(m_inputDevice->data());
}

void AView::fitInWidth()
{
    scale((double) viewport()->width() / unmapedSceneRect().width(), 1.0);
}

void AView::fitInHeight()
{
    scale(1.0, (double) viewport()->height() / unmapedSceneRect().height());
}
