#include "ainputdevice.h"

AIODevice::AIODevice(AInputDevice *parent)
    : QIODevice(parent)
    , m_inputDevice(parent)
{
    open(QIODevice::WriteOnly);
}

AIODevice::~AIODevice()
{
}

qint64 AIODevice::readData(char *, qint64)
{
    //Device can not read data
    return 0;
}

qint64 AIODevice::writeData(const char *data, qint64 len)
{
    m_outBuffer.clear();
    QAudioFormat format = m_inputDevice->format();

    Q_ASSERT(m_inputDevice->format().sampleSize() % 8 == 0); //if sample size does not multiple of one byte

    const int channelBytes = format.sampleSize() / 8; //count of byte in one sample per channel
    const int sampleBytes  = format.channelCount() * channelBytes; //count of byte in one sample per all channels

    Q_ASSERT(len % sampleBytes == 0); //if the data does not contain a multiple count of samples

    const int numSamples = len / sampleBytes;
    const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);

    for (int i = 0; i < numSamples; ++i) {
        for (int j = 0; j < format.channelCount(); ++j) {

            qint32 value = 0;

            if (format.sampleSize() == 8 && format.sampleType() == QAudioFormat::UnSignedInt) {
                value = *reinterpret_cast<const quint8*>(ptr) - 0x7F;
            } else if (format.sampleSize() == 8 && format.sampleType() == QAudioFormat::SignedInt) {
                value = *reinterpret_cast<const qint8*>(ptr);

            } else if (format.sampleSize() == 16 && format.sampleType() == QAudioFormat::UnSignedInt) {
                if (format.byteOrder() == QAudioFormat::LittleEndian)
                    value = qFromLittleEndian<quint16>(ptr) - 0x7FFF;
                else
                    value = qFromBigEndian<quint16>(ptr) - 0x7FFF;

            } else if (format.sampleSize() == 16 && format.sampleType() == QAudioFormat::SignedInt) {
                if (format.byteOrder() == QAudioFormat::LittleEndian)
                    value = qFromLittleEndian<qint16>(ptr);
                else
                    value = qFromBigEndian<qint16>(ptr);

//            } else if (format.sampleSize() == 32 && format.sampleType() == QAudioFormat::UnSignedInt) {
//                if (format.byteOrder() == QAudioFormat::LittleEndian)
//                    value = qFromLittleEndian<quint32>(ptr);
//                else
//                    value = qFromBigEndian<quint32>(ptr);

//            } else if (format.sampleSize() == 32 && format.sampleType() == QAudioFormat::SignedInt) {
//                if (format.byteOrder() == QAudioFormat::LittleEndian)
//                    value = qAbs(qFromLittleEndian<qint32>(ptr));
//                else
//                    value = qAbs(qFromBigEndian<qint32>(ptr));

//            } else if (format.sampleSize() == 32 && format.sampleType() == QAudioFormat::Float) {
//                value = qAbs(*reinterpret_cast<const float*>(ptr) * 0x7fffffff); // assumes 0-1.0
            }

            m_outBuffer.append(value);
            ptr += channelBytes;
        }
    }

    emit m_inputDevice->dataIsReady();

    return len;
}


AInputDevice::AInputDevice(const QAudioDeviceInfo &audioDeviceInfo, const QAudioFormat &format, QObject *parent)
    : QAudioInput(audioDeviceInfo, format, parent)
    , m_ioDevice(this)
{
}

AInputDevice::~AInputDevice()
{

}

void AInputDevice::start()
{
    QAudioInput::start(&m_ioDevice);
}

const QVector<double> &AInputDevice::data() const
{
    return m_ioDevice.m_outBuffer;
}
