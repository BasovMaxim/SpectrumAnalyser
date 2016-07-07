#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAudioInput>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QMenu>
#include <QProxyStyle>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QToolBar>
#include <QToolButton>

#include "fft.h"

#include "acustomslider.h"
#include "ainputdevice.h"
#include "asettingsdialog.h"

#include "graphics/aview.h"

namespace Ui {
class MainWindow;
}

// The class is necessary for touch-screen devices
class AProxyStyle: public QProxyStyle
{
public:
    // Makes scalable menu icons

    int pixelMetric(PixelMetric metric, const QStyleOption * option = 0, const QWidget * widget = 0 ) const override
    {
        if (metric == QStyle::PM_SmallIconSize)
            return 32;
        else
            return QProxyStyle::pixelMetric(metric, option, widget);
    }

    // Makes scrollable all area of slider
    int styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget,
                  QStyleHintReturn *returnData) const override
    {
        if(hint == SH_Slider_AbsoluteSetButtons)
        {
            return Qt::LeftButton;
        }
        return QProxyStyle::styleHint(hint, option, widget, returnData);
    }
};

/*
 * The mainWidow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    AInputDevice *m_inputDevice = nullptr;
    AView *m_view               = nullptr;

    QMenu *m_mainMenu           = nullptr;
    QWidget *m_controlWidget    = nullptr;

    void initializeAudio();
    void initializeIterface();
    void setColorTheme(AColorTheme theme);

    //for load of styleSheets
    QString loadTextFromFile(const QString &filename);
    void changeStyleSheet(QString &styleSheet);

public slots:
    void setPlotType(APlotType type);
    void start();
    void stop();
    void showMainMenu();
    void showSettingsDialog();
};

#endif // MAINWINDOW_H
