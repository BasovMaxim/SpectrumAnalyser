#include "aspectrogram.h"
#include "aview.h"

ASpectrogram::ASpectrogram(AView *view)
    : m_view(view)
    , m_tools(nullptr)
    , m_bufferSizeRangeExp(QPair<int, int>(0, 0))
    , m_currentLine(0)
    , m_logScale(true)
    , m_tracking(true)
{
}

ASpectrogram::~ASpectrogram()
{
    DEBUG("ASpectrogramm removed");
    if(m_tools)
        delete m_tools;
}

void ASpectrogram::addData(const QVector<double> &data)
{
    m_buffer += data;
    if(m_buffer.size() > m_bufferSize)
    {
        AFftTransformer::fft(m_buffer, m_bufferSize, m_data);
        fillImage();
        update();
        if(data.size() > m_bufferSize)
            m_buffer.clear();
        else
            m_buffer.remove(0, m_bufferSize);
    }
}

APlotType ASpectrogram::plotType() const
{
    return APlotType::Spectrogram;
}

QToolBar *ASpectrogram::controlWidget(QWidget *parent)
{
    if(!m_tools)
    {
        m_tools = createTools(parent);
        updateControlsData();
    }

    return m_tools;
}

void ASpectrogram::initialize()
{
    m_ampl = qPow(2.0, (double)m_view->inputDevice()->format().sampleSize());
    m_recordTime = 250;

    double exponent = qCeil(qLn(m_view->inputDevice()->periodSize()) / qLn(2.0));
    m_bufferSizeRangeExp.first = exponent - 4;
    m_bufferSizeRangeExp.second = exponent + 4;

    setBufferSizeExp(exponent);
    updateControlsData();
}

void ASpectrogram::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(m_view->linePen());
    painter->drawImage(0, 0, m_image);
    painter->drawLine(m_currentLine, 0.0, m_currentLine, m_image.height());
}

void ASpectrogram::setBufferSizeExp(int size)
{
    m_bufferSizeExp = size;
    m_bufferSize = qPow(2.0, size);
    m_data.resize(m_bufferSize / 2);

    int prefSize = m_view->inputDevice()->periodSize() / 2.0;

    if(m_bufferSize > prefSize)
        m_xRes = m_bufferSize / (double)m_view->inputDevice()->format().sampleRate();
    else
        m_xRes = prefSize / (double)m_view->inputDevice()->format().sampleRate();

    m_yRes = 2.0 / (double)m_bufferSize;

    DEBUG(m_bufferSize << m_xRes << m_yRes << m_recordTime);

    m_currentLine = 0;
    m_image = QImage(m_recordTime / m_xRes, m_bufferSize / 2.0, QImage::Format_RGB32);

    DEBUG(m_image.width() << m_image.height());

    setRect(0.0, 0.0, m_image.width(), m_image.height());
}

void ASpectrogram::setRecordTime(int time)
{
    m_currentLine = 0;
    m_recordTime = time;
    m_image = QImage(m_recordTime / m_xRes, m_bufferSize / 2.0, QImage::Format_RGB32);
    setRect(0.0, 0.0, m_image.width(), m_image.height());
}

void ASpectrogram::updateControlsData()
{
    if(m_tools)
    {
        QObject::connect(m_frameSizeSlider->slider(), &QSlider::valueChanged, [=](int val){
            setBufferSizeExp(val);
            m_frameSizeSlider->label()->setText(QString::number((double)m_view->inputDevice()->format().sampleRate() / exp2(val)));
            m_view->updateSceneSize();
        });
        m_frameSizeSlider->setRange(m_bufferSizeRangeExp, m_bufferSizeExp);
        m_frameSizeSlider->label()->setText(QString::number((double)m_view->inputDevice()->format().sampleRate() / exp2(m_bufferSizeExp)));

        QObject::connect(m_recotdTimeSlider->slider(), &QSlider::valueChanged, [=](int val){
            setRecordTime(val);
            m_view->updateSceneSize();
        });
        m_recotdTimeSlider->setRange(QPair<int, int>(10, 600), m_recordTime);
    }
}

void ASpectrogram::fillImage()
{
    double max = m_logScale ? qLn(m_ampl / m_yRes) : m_ampl / m_yRes;
    for(int i = 0; i < m_data.size(); ++i)
    {
        double v = m_logScale ? qLn(m_data.at(i)) : m_data.at(i);
        m_image.setPixel(m_currentLine, i, color((v / max)  * 0x4FE));
    }

    if(m_currentLine >= m_image.width() - 1)
        m_currentLine = 0;
    else        
        ++m_currentLine;
}

int ASpectrogram::color(int v)
{
    int r = 0; int g = 0; int b = 0;
    int b1 = v & 0x000000FF;
    int b2 = v & 0x0000FF00;

    switch (b2) {
    case 0x0:
        r = 0; g = 0; b = b1;
        break;
    case 0x100:
        r = b1; b = 0xFF;
        break;
    case 0x200:
        r = 0xFF; b = 0xFF - b1;
        break;
    case 0x300:
        r = 0xFF; g = b1;
        break;
    case 0x400:
        r = 0xFF; g = 0xFF; b = b1;
        break;
    default:
        r = 0xFF; g = 0xFF; b = 0xFF;
        break;
    }

    return b + (g << 8) + (r << 16);
}

void ASpectrogram::setLogScale(bool value)
{
    m_logScale = value;
}

QToolBar *ASpectrogram::createTools(QWidget *parent)
{
    QToolBar *tools = new QToolBar("Spectrograph tools", parent);
    tools->setMovable(false);
    tools->setFloatable(false);

    m_frameSizeSlider = new ACustomSlider("Resolution (Hz)", m_bufferSizeRangeExp, m_bufferSizeExp, false, tools);

    QToolButton *tb = new QToolButton(tools);
    tb->setIcon(QIcon(":res/icons/tdaley.png"));
    tb->setPopupMode(QToolButton::InstantPopup);
    tb->setToolTip("Resolution");
    tb->setMenu(new QMenu(tools));

    QWidgetAction *wa = new QWidgetAction(tools);
    wa->setDefaultWidget(m_frameSizeSlider);

    tb->menu()->addAction(wa);
    tools->addWidget(tb);

    m_recotdTimeSlider = new ACustomSlider("Record time (sec)", QPair<int, int>(10, 600), m_recordTime, true, tools);

    QToolButton *tb2 = new QToolButton(tools);
    tb2->setIcon(QIcon(":res/icons/tdaley.png"));
    tb2->setPopupMode(QToolButton::InstantPopup);
    tb2->setToolTip("Record time");
    tb2->setMenu(new QMenu(tools));

    QWidgetAction *wa2 = new QWidgetAction(tools);
    wa2->setDefaultWidget(m_recotdTimeSlider);

    tb2->menu()->addAction(wa2);
    tools->addWidget(tb2);

    return tools;
}

