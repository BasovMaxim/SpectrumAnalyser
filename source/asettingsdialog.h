#ifndef ASETTINGSDIALOG_H
#define ASETTINGSDIALOG_H

#include <QAudioFormat>
#include <QDialog>
#include <QResizeEvent>
#include <QStyledItemDelegate>

#include "acustomslider.h"

namespace Ui {
class ASettingsDialog;
}

class ASettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ASettingsDialog(QWidget *parent = 0);
    ~ASettingsDialog();

    static QAudioFormat preferredFormat();
    static QAudioFormat nearestPreferredFormat(QAudioDeviceInfo &device);

    QAudioFormat foramt() const;
    QAudioDeviceInfo deviceInfo() const;
    int colorTheme() const;

private:
    Ui::ASettingsDialog *ui;
    QAudioFormat m_defaultFormat;

    ACustomSlider m_sampleRateSlider{"Sample rate"};
    ACustomSlider m_sampleSizeSlider{"Sample size"};

    QList<QAudioDeviceInfo> m_devices;

    int m_deviceIndex;

protected slots:
    void setDeviceInfo(int index);
};

#endif // ASETTINGSDIALOG_H
