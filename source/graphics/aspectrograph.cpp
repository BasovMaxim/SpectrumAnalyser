#include "aspectrograph.h"
#include "aview.h"

ASpectrograph::ASpectrograph(AView *view)
    : m_view(view)
    , m_tools(nullptr)
    , m_bufferSizeRangeExp(QPair<int, int>(0, 0))
{
}

ASpectrograph::~ASpectrograph()
{
    DEBUG("ASpectrograph removed");
    if(m_tools)
        delete m_tools;
}

void ASpectrograph::addData(const QVector<double> &data)
{
    m_buffer += data;
    if(m_buffer.size() > m_bufferSize)
    {
        AFftTransformer::fft(m_buffer, m_bufferSize, m_data);
        m_buffer.remove(0, m_bufferSize);
        update();
        if(data.size() > m_bufferSize)
            m_buffer.clear();
    }
}

APlotType ASpectrograph::plotType() const
{
    return APlotType::Spectrograph;
}

QToolBar *ASpectrograph::controlWidget(QWidget *parent)
{
    if(!m_tools)
        m_tools = createTools(parent);

    return m_tools;
}

void ASpectrograph::initialize()
{
    setRect(0.0, 0.0, m_data.size(), 10000);

    double exponent = qCeil(qLn(m_view->inputDevice()->periodSize()) / qLn(2.0));

    m_ampl = qPow(2.0, (double)m_view->inputDevice()->format().sampleSize());

    m_bufferSizeRangeExp.first = exponent - 4;
    m_bufferSizeRangeExp.second = exponent + 4;

    DEBUG(m_bufferSize << m_xRes << m_yRes);
    setBufferSizeExp(exponent);

    updateControlsData();
}

void ASpectrograph::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //Draw graph
    painter->setPen(m_view->linePen());

    QPolygonF polyline;

    for(int i = 0; i < m_data.size(); ++i)
    {
        polyline.append(QPointF((double)i * m_xRes, m_data.at(i) * m_yRes));
    }

    painter->drawPolyline(polyline);

    //Draw mesh
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
    int x = o.x() % dx;
    int y = bottom + (o.y() - bottom) % dy;

    painter->drawText(QPoint(2, 20), "0.0");

    while (x < right) {
        painter->drawLine(x, 0.0, x, bottom);
        QPointF p = m_view->mapToScene(x, y);
        painter->drawText(QPoint(x + 2, y - 2), QString::number(p.x()));
        x += dx;
    }

    while (y > 0) {
        painter->drawLine(0.0, y, right, y);
        QPointF p = m_view->mapToScene(x, y);
        painter->drawText(QPoint(2, y - 2), QString::number(p.y()));
        y -= dy;
    }

    painter->restore();
}

void ASpectrograph::setBufferSizeExp(int size)
{
    m_bufferSizeExp = size;
    m_bufferSize = qPow(2.0, size);
    m_data.resize(m_bufferSize / 2);

    m_xRes = (double)m_view->inputDevice()->format().sampleRate() / (double)m_bufferSize;
    m_yRes = 2.0 / (double)m_bufferSize;

    setRect(0.0, 0.0, (double)m_bufferSize * m_xRes / 2.0, m_ampl);
}

void ASpectrograph::updateControlsData()
{
    if(m_slider)
    {
        QObject::connect(m_slider->slider(), &QSlider::valueChanged, [=](int val){
            setBufferSizeExp(val);
            m_slider->label()->setText(QString::number((double)m_view->inputDevice()->format().sampleRate() / exp2(val)));
            m_view->updateSceneSize();
        });
        m_slider->setRange(m_bufferSizeRangeExp, m_bufferSizeExp);
        m_slider->label()->setText(QString::number((double)m_view->inputDevice()->format().sampleRate() / exp2(m_bufferSizeExp)));
    }
}

QToolBar *ASpectrograph::createTools(QWidget *parent)
{
    QToolBar *tools = new QToolBar("Spectrograph tools", parent);

    m_slider = new ACustomSlider("Resolution (Hz)", m_bufferSizeRangeExp, m_bufferSizeExp, false, tools);
    updateControlsData();

    QToolButton *tb = new QToolButton(tools);
    tb->setIcon(QIcon(":res/icons/tdaley.png"));
    tb->setPopupMode(QToolButton::InstantPopup);
    tb->setToolTip("Resolution");
    tb->setMenu(new QMenu(tools));

    QWidgetAction *wa = new QWidgetAction(tools);
    wa->setDefaultWidget(m_slider);

    tb->menu()->addAction(wa);

    tools->setMovable(false);
    tools->setFloatable(false);
    tools->addWidget(tb);
    return tools;
}

