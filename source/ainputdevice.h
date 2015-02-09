#ifndef AINPUTDEVICE_H
#define AINPUTDEVICE_H

#include <QtEndian>

#include <QAudioFormat>
#include <QAudioInput>
#include <QIODevice>
#include <QVector>

#include "fft.h"

class AInputDevice;

class AIODevice : public QIODevice
{
    Q_OBJECT
public:
    friend class AInputDevice;

    AIODevice(AInputDevice *parent);
    ~AIODevice();

protected:
    qint64 readData(char *, qint64) override;

    //Performs a static cast from const char to float and writes its in m_outBuffer
    qint64 writeData(const char *data, qint64 len) override;

private:
    AInputDevice *m_inputDevice;
    QVector<double> m_outBuffer;

    int m_periodSize;
};


class AInputDevice : public QAudioInput
{
    Q_OBJECT
public:
    AInputDevice(const QAudioDeviceInfo &audioDeviceInfo, const QAudioFormat &format = QAudioFormat(),
                 QObject *parent = 0);

    ~AInputDevice();

    void start();
    const QVector<double> &data() const;

private:
    AIODevice m_ioDevice;

signals:
    void dataIsReady();
};



#endif // AINPUTDEVICE_H
