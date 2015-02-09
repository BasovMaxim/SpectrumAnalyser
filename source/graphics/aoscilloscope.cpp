#include "aoscilloscope.h"
#include "aview.h"

AOscilloscope::AOscilloscope(AView *view)
    : m_view(view)
    , m_tools(nullptr)
    , m_bufferSizeRange(QPair<int, int>(0, 0))
{
}

AOscilloscope::~AOscilloscope()
{
    DEBUG("AOscilloscope removed");
    if(m_tools)
        delete m_tools;
}

void AOscilloscope::addData(const QVector<double> &data)
{
    m_buffer += data;
    if(m_buffer.size() > m_bufferSize)
    {
        m_data = m_buffer.mid(0, m_bufferSize);
        m_buffer.remove(0, m_bufferSize);
        update();
    }
}

APlotType AOscilloscope::plotType() const
{
    return APlotType::Oscilloscope;
}

QToolBar *AOscilloscope::controlWidget(QWidget *parent)
{
    if(!m_tools)
        m_tools = createTools(parent);
    return m_tools;
}

void AOscilloscope::initialize()
{
    QAudioFormat format = m_view->inputDevice()->format();

    m_xRes = 1.0 / (double)format.sampleRate();
    m_ampl = qPow(2.0, (double)format.sampleSize());

    m_bufferSizeRange.first = m_view->inputDevice()->periodSize() / 2;
    m_bufferSizeRange.second = m_view->inputDevice()->periodSize() * 100;
    DEBUG("MAX" << m_bufferSizeRange.second);

    setBufferSize(m_bufferSizeRange.first);

    DEBUG(m_bufferSizeRange << m_bufferSize);
    updateControlsData();
}

void AOscilloscope::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(m_view->linePen());
    //painter->fillRect(rect(), QBrush(Qt::gray));
    //painter->fillRect(scene()->sceneRect(), QBrush(Qt::gray));

    painter->drawLine(QLineF(0.0, 0.0, rect().width(), 0.0));

    QPolygonF polyline;

    for(int i = 0; i < m_data.size(); ++i)
    {
        polyline.append(QPointF((double)i * m_xRes, m_data.at(i)));
    }

    painter->drawPolyline(polyline);

    painter->save();
    painter->resetTransform();
    painter->setPen(m_view->meshPen());
    painter->setBrush(m_view->backgroundBrush());
    painter->setBackground(m_view->backgroundBrush());
    painter->setBackgroundMode(Qt::OpaqueMode);

    int right = m_view->viewport()->rect().right();
    int bottom = m_view->viewport()->rect().bottom();

    QPoint o = m_view->mapFromScene(QPointF(0.0, 0.0));

    int dx = 100;
    int dy = 100;
    int x = 0;
    int y = o.y();

    while (x < right) {
        painter->drawLine(x, 0.0, x, bottom);
        QPointF p = m_view->mapToScene(x, y);
        painter->drawText(QPoint(x + 2, y - 2), QString::number(p.x()));
        x += dx;
    }
    while (y < bottom) {
        painter->drawLine(0.0, y, right, y);
        QPointF p = m_view->mapToScene(x, y);
        painter->drawText(QPoint(2, y - 2), QString::number(p.y()));
        y += dy;
    }
    y = o.y();
    while (y > 0) {
        painter->drawLine(0.0, y, right, y);
        QPointF p = m_view->mapToScene(x, y);
        painter->drawText(QPoint(2, y - 2), QString::number(p.y()));
        y -= dy;
    }

    painter->restore();
}

void AOscilloscope::setBufferSize(int size)
{
    if(size < m_bufferSizeRange.first)
        m_bufferSize = m_bufferSizeRange.first;
    else if(size >= m_bufferSizeRange.second)
        m_bufferSize = m_bufferSizeRange.second;
    else
        m_bufferSize = size;
    prepareGeometryChange();
    setRect(QRectF(0.0, - m_ampl / 2.0, (double)m_bufferSize * m_xRes, m_ampl));

    m_view->updateSceneSize();
}

void AOscilloscope::updateControlsData()
{
    if(m_slider)
    {
        QObject::connect(m_slider->slider(), &QSlider::valueChanged, [=](int val){
            setBufferSize(val);
            m_slider->label()->setText(QString::number(val * m_xRes));
            m_view->updateSceneSize();
        });
        m_slider->setRange(m_bufferSizeRange, m_bufferSize);
        m_slider->label()->setText(QString::number(m_bufferSize * m_xRes));
            DEBUG("MAX" << m_bufferSizeRange.second);
    }
}

QToolBar *AOscilloscope::createTools(QWidget *parent)
{
    QToolBar *tools = new QToolBar("Oscilloscope tools", parent);

    m_slider = new ACustomSlider("Time delay (sec)", m_bufferSizeRange, m_bufferSize, false, tools);
    updateControlsData();

    QToolButton *tb = new QToolButton(tools);
    tb->setIcon(QIcon(":res/icons/tdaley.png"));
    tb->setPopupMode(QToolButton::InstantPopup);
    tb->setToolTip("Time delay");
    tb->setMenu(new QMenu(tools));

    QWidgetAction *wa = new QWidgetAction(tools);
    wa->setDefaultWidget(m_slider);

    tb->menu()->addAction(wa);

    tools->setMovable(false);
    tools->setFloatable(false);
    tools->addWidget(tb);
    return tools;
}

