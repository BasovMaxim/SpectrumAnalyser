#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setStyle(new AProxyStyle());
    setStyleSheet(loadTextFromFile(":/styleSheet.css"));

    initializeAudio();
    initializeIterface();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeAudio()
{
    QAudioDeviceInfo device = QAudioDeviceInfo::defaultInputDevice();

    m_inputDevice = new AInputDevice(QAudioDeviceInfo::defaultInputDevice(),
                                     ASettingsDialog::nearestPreferredFormat(device), this);

    PRINTFORMAT(ASettingsDialog::nearestPreferredFormat(device));
    m_inputDevice->start();
}

void MainWindow::initializeIterface()
{
    //main toolbar
    QToolBar *topToolBar = new QToolBar();
    topToolBar->setIconSize(QSize(32, 32));

    QAction *showMainMenuAction = new QAction(QIcon(":/res/icons/menu.png"), "Menu", this);
    connect(showMainMenuAction, &QAction::triggered, this, &MainWindow::showMainMenu);

    QAction *startAction = new QAction(QIcon(":/res/icons/start.png"), "Start", this);
    startAction->setCheckable(true);
    startAction->setChecked(true);
    connect(startAction, &QAction::triggered, this, &MainWindow::start);

    QAction *stopAction = new QAction(QIcon(":/res/icons/stop.png"), "Stop", this);
    stopAction->setCheckable(true);
    connect(stopAction, &QAction::triggered, this, &MainWindow::stop);

    QAction *fitInWidthAction = new QAction(QIcon(":/res/icons/fitw.png"), "Fit in width", this);
    connect(fitInWidthAction, &QAction::triggered, [=](){
        if(m_view) m_view->fitInWidth();
    });

    QAction *fitInHeightAction = new QAction(QIcon(":/res/icons/fith.png"), "Fit in height", this);
    connect(fitInHeightAction, &QAction::triggered, [=](){
        if(m_view) m_view->fitInHeight();
    });

    QActionGroup *playActionGroupe = new QActionGroup(this);
    playActionGroupe->addAction(startAction);
    playActionGroupe->addAction(stopAction);
    playActionGroupe->setExclusive(true);

    topToolBar->setMovable(false);
    topToolBar->setFloatable(false);
    topToolBar->addAction(showMainMenuAction);
    topToolBar->addSeparator();
    topToolBar->addAction(startAction);
    topToolBar->addAction(stopAction);
    topToolBar->addSeparator();
    topToolBar->addAction(fitInWidthAction);
    topToolBar->addAction(fitInHeightAction);

    addToolBar(Qt::TopToolBarArea, topToolBar);

    //main menu
    m_mainMenu = new QMenu(this);

    m_mainMenu->setWindowFlags(m_mainMenu->windowFlags() | Qt::FramelessWindowHint);
    m_mainMenu->setAttribute(Qt::WA_TranslucentBackground); //For transparency

    QActionGroup *modeActionGroupe = new QActionGroup(this);

    QAction *oscilloscopeModeAction = new QAction(QIcon(":/res/icons/osci_48p.png"), "Oscilloscope", this);
    oscilloscopeModeAction->setCheckable(true);
    oscilloscopeModeAction->setChecked(true);
    connect(oscilloscopeModeAction, &QAction::triggered, [=](){this->setPlotType(APlotType::Oscilloscope);});

    QAction *spectrographModeAction = new QAction(QIcon(":/res/icons/spectr_48p.png"), "Spectrograph", this);
    spectrographModeAction->setCheckable(true);
    connect(spectrographModeAction, &QAction::triggered, [=](){this->setPlotType(APlotType::Spectrograph);});

    QAction *spectrogrammModeAction = new QAction(QIcon(":/res/icons/spectrogram_48p.png"), "Spectrogram", this);
    spectrogrammModeAction->setCheckable(true);
    connect(spectrogrammModeAction, &QAction::triggered, [=](){this->setPlotType(APlotType::Spectrogram);});

    modeActionGroupe->setExclusive(true);
    modeActionGroupe->addAction(oscilloscopeModeAction);
    modeActionGroupe->addAction(spectrographModeAction);
    modeActionGroupe->addAction(spectrogrammModeAction);

    QAction *settingsAction = new QAction(QIcon(":/res/icons/settings_48p.png"), "Settings", this);
    connect(settingsAction, &QAction::triggered, this, &MainWindow::showSettingsDialog);

    m_mainMenu->addAction(oscilloscopeModeAction);
    m_mainMenu->addAction(spectrographModeAction);
    m_mainMenu->addAction(spectrogrammModeAction);
    m_mainMenu->addSeparator();
    m_mainMenu->addAction(settingsAction);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    //plot view
    m_view = new AView(m_inputDevice, this);
    mainLayout->addWidget(m_view);
    mainLayout->setMargin(0);
    centralWidget()->setLayout(mainLayout);

    setColorTheme(AColorTheme::Twilight);
    setPlotType(APlotType::Spectrograph);
}

void MainWindow::setColorTheme(AColorTheme theme)
{
    switch (theme) {
    case AColorTheme::Twilight:
        m_view->setColors(Qt::black, Qt::green, Qt::white, Qt::gray);
        break;
    case AColorTheme::Blue:
        m_view->setColors(Qt::blue, Qt::white, Qt::white, Qt::white);
        break;
    case AColorTheme::White:
        m_view->setColors(Qt::white, Qt::black, Qt::black, Qt::gray);
        break;
    }
}

QString MainWindow::loadTextFromFile(const QString &filename)
{
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly);

    QString text = file.readAll();

    file.close();

    return text;
}

void MainWindow::changeStyleSheet(QString &styleSheet)
{
    setStyleSheet(loadTextFromFile(styleSheet));
}

void MainWindow::setPlotType(APlotType type)
{
    m_view->setPlotType(type);

    //Add the toolbar with controls widgets.
    //This toolbar will be removed automatically before removing the plot.
    addToolBar(Qt::TopToolBarArea, m_view->getControlTools(this));
}

void MainWindow::start()
{
    m_inputDevice->start();
}

void MainWindow::stop()
{
    m_inputDevice->stop();
}

void MainWindow::showMainMenu()
{
    m_mainMenu->popup(mapToGlobal(QPoint(0, 48)));
}

void MainWindow::showSettingsDialog()
{
    ASettingsDialog *settingsDialog = new ASettingsDialog(this);

#ifdef DESKTOP
    settingsDialog->setFixedSize(settingsDialog->size());
#else
    settingsDialog->showMaximized();
#endif

    if(settingsDialog->exec()) //if accepted
    {
        delete m_inputDevice;
        m_inputDevice = new AInputDevice(settingsDialog->deviceInfo(), settingsDialog->foramt(), this);
        m_inputDevice->start();
        m_view->setInputDevice(m_inputDevice);
        setColorTheme(static_cast<AColorTheme>(settingsDialog->colorTheme()));
    }

    delete settingsDialog;
}
