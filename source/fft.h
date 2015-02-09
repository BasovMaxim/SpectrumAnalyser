#ifndef FFT
#define FFT

#include <QDebug>
#include <QAudioDeviceInfo>

#include <qmath.h>

#ifdef DEBUGOUTPUT
#define DEBUG(mes) qDebug() << mes
#else
#define DEBUG(mes)
#endif

#define PRINTFORMAT(FORMAT) \
    DEBUG("SampleRate:"    << FORMAT.sampleRate());    \
    DEBUG("SampleSize:"    << FORMAT.sampleSize());    \
    DEBUG("ChanelsCount:"  << FORMAT.channelCount());  \

enum class AColorTheme {
    Twilight  = 0,
    Blue  = 1,
    White = 2
};

enum class APlotType
{
    NoPlot,
    SimplePlot,
    Oscilloscope,
    Spectrograph,
    Spectrogram
};

#endif // FFT

