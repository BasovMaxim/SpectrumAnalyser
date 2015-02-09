#include "asettingsdialog.h"
#include "ui_asettingsdialog.h"

ASettingsDialog::ASettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ASettingsDialog)
{
    ui->setupUi(this);

    ui->colorThemeBox->setItemDelegate(new QStyledItemDelegate());
    ui->deviceComboBox->setItemDelegate(new QStyledItemDelegate());

    ui->formatLayer->addWidget(&m_sampleRateSlider);
    ui->formatLayer->addWidget(&m_sampleSizeSlider);

    m_devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);

    foreach (QAudioDeviceInfo info, m_devices) {
        ui->deviceComboBox->addItem(info.deviceName());
    }

    connect(ui->deviceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setDeviceInfo(int)));
    connect(ui->okBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    setDeviceInfo(0);
}

ASettingsDialog::~ASettingsDialog()
{
    delete ui;
}

QAudioFormat ASettingsDialog::preferredFormat()
{
    QAudioFormat format;
    format.setSampleRate(22050);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::UnSignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/pcm");
    format.setSampleType(QAudioFormat::SignedInt);

    return format;
}

QAudioFormat ASettingsDialog::nearestPreferredFormat(QAudioDeviceInfo &device)
{
    QAudioFormat format = preferredFormat();

    if(!device.isFormatSupported(format))
        format = device.nearestFormat(format);

    return format;
}

QAudioFormat ASettingsDialog::foramt() const
{
    QAudioFormat format = preferredFormat();

    if(!m_devices.at(m_deviceIndex).isFormatSupported(format))
        format = m_devices.at(m_deviceIndex).nearestFormat(format);

    format.setSampleRate(m_sampleRateSlider.currentValue());
    format.setSampleSize(m_sampleSizeSlider.currentValue());

    return format;
}

QAudioDeviceInfo ASettingsDialog::deviceInfo() const
{
    return m_devices.at(m_deviceIndex);
}

int ASettingsDialog::colorTheme() const
{
    return ui->colorThemeBox->currentIndex();
}

void ASettingsDialog::setDeviceInfo(int index)
{
    m_deviceIndex = index;

    DEBUG(m_devices.at(m_deviceIndex).deviceName());

    QAudioFormat format = preferredFormat();

    if(!m_devices.at(m_deviceIndex).isFormatSupported(format))
        format = m_devices.at(m_deviceIndex).nearestFormat(format);

    PRINTFORMAT(format);

    QList<int> sampleRatesList = m_devices.at(index).supportedSampleRates();
    m_sampleRateSlider.setValues(sampleRatesList, sampleRatesList.indexOf(format.sampleRate()));

    QList<int> sampleSizesList = m_devices.at(index).supportedSampleSizes();
    m_sampleSizeSlider.setValues(sampleSizesList, sampleSizesList.indexOf(format.sampleSize()));
}
