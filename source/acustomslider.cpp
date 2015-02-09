#include "acustomslider.h"
#include "ui_acustomslider.h"

ACustomSlider::ACustomSlider(const QString &title, const QPair<int, int> &range, int value, bool autoTracking, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ACustomSlider)
{
    ui->setupUi(this);

    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setTracking(true);

    ui->horizontalLayout->insertWidget(1, m_slider);

    ui->titleLabel->setText(title);
    setRange(range, value);

    if(autoTracking)
    {
        ui->label->setText(QString::number(value));

        connect(m_slider, &QSlider::valueChanged, [=](int val){
            ui->label->setText(QString::number(val));
        });
    }

    connect(ui->decrBtn, &QToolButton::clicked, [=](){
        m_slider->setValue(m_slider->value() - 1);
    });
    connect(ui->incrBtn, &QToolButton::clicked, [=](){
        m_slider->setValue(m_slider->value() + 1);
    });
}

ACustomSlider::ACustomSlider(const QString &title, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ACustomSlider)
{
    ui->setupUi(this);

    ui->titleLabel->setText(title);
    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setDisabled(true);
    m_slider->setTracking(true);
    ui->horizontalLayout->insertWidget(1, m_slider);

    connect(m_slider, &QSlider::valueChanged, [=](int val){
        ui->label->setText(QString::number(m_values.at(val)));
    });

    connect(ui->decrBtn, &QToolButton::clicked, [=](){
        m_slider->setValue(m_slider->value() - 1);
    });
    connect(ui->incrBtn, &QToolButton::clicked, [=](){
        m_slider->setValue(m_slider->value() + 1);
    });
}

ACustomSlider::~ACustomSlider()
{
    delete ui;
}

QLabel *ACustomSlider::label()
{
    return ui->label;
}

QSlider *ACustomSlider::slider()
{
    return m_slider;
}

void ACustomSlider::setRange(const QPair<int, int> &range, int value)
{
    m_slider->setMinimum(range.first);
    m_slider->setMaximum(range.second);
    m_slider->setValue(value);
}

QList<int> &ACustomSlider::values()
{
    return m_values;
}

void ACustomSlider::setValues(const QList<int> &values, int currentValue)
{
    if(values.isEmpty())
        return;

    m_values = values;

    m_slider->setDisabled(false);
    m_slider->setMinimum(0);
    m_slider->setMaximum(values.size() - 1);
    m_slider->setValue(currentValue);
}

int ACustomSlider::currentValue() const
{
    return m_values.at(m_slider->value());
}
