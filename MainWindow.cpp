#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      chainComboBox(new QComboBox(this)),
      komododRadio(new QRadioButton("komodod", this)),
      komodoQtRadio(new QRadioButton("komodo-qt", this)),
      launchButton(new QPushButton("Launch", this)),
      networkManager(new QNetworkAccessManager(this))
{
    // setMinimumHeight(350);
    setupUI();
    downloadAssetChains();

    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onDownloadFinished);
    connect(launchButton, &QPushButton::clicked, this, &MainWindow::onLaunchButtonClicked);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Create menu bar
    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *menu = menuBar->addMenu("Options");

    QAction *donateAction = new QAction("Donate", this);
    QAction *aboutAction = new QAction("About", this);

    menu->addAction(donateAction);
    menu->addAction(aboutAction);

    setMenuBar(menuBar);

    // Connect actions to slots
    connect(donateAction, &QAction::triggered, this, &MainWindow::openDonatePage);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);

    // Radio Buttons for Executable Choice
    QGroupBox *execGroup = new QGroupBox("Select Executable:", this);
    QHBoxLayout *execLayout = new QHBoxLayout(execGroup);
    execLayout->addWidget(komododRadio);
    execLayout->addWidget(komodoQtRadio);
    komododRadio->setChecked(true);

    // Chain Selection
    QHBoxLayout *chainLayout = new QHBoxLayout();
    chainLayout->addWidget(new QLabel("Select Assetchain:", this));
    chainLayout->addWidget(chainComboBox);

    // Additional parameters layout
    QHBoxLayout *additionalParamsLayout = new QHBoxLayout();
    additionalParamsLineEdit = new QLineEdit(this);
    additionalParamsLineEdit->setPlaceholderText("-param1=value1 -param2=value2");
    additionalParamsLayout->addWidget(new QLabel("Additional Parameters:", this));
    additionalParamsLayout->addWidget(additionalParamsLineEdit);

    // Add to Main Layout
    mainLayout->addWidget(execGroup);
    mainLayout->addLayout(chainLayout);
    mainLayout->addLayout(additionalParamsLayout);
    mainLayout->addWidget(launchButton);

    setCentralWidget(centralWidget);
    setWindowTitle("Komodo Launcher");
    resize(400, 150);
}

void MainWindow::downloadAssetChains() {
    QUrl url("https://raw.githubusercontent.com/KomodoPlatform/dPoW/refs/heads/master/iguana/assetchains.json");
    QNetworkRequest request(url);
    networkManager->get(request);
}

void MainWindow::onDownloadFinished(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        parseAssetChains(data);
    } else {
        // Get the error string
        QString errorString = reply->errorString();

        // Display the actual error message
        QMessageBox::critical(this, "Error", QString("Failed to download assetchains.json:\n%1").arg(errorString));
    }
    reply->deleteLater();
}

void MainWindow::parseAssetChains(const QByteArray& data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isArray()) {
        QJsonArray array = doc.array();
        for (const QJsonValue &value : array) {
            if (value.isObject()) {
                QVariantMap chain = value.toObject().toVariantMap();
                assetChains.append(chain);
                chainComboBox->addItem(chain["ac_name"].toString());
            }
        }
    } else {
        QMessageBox::critical(this, "Error", "Invalid JSON format in assetchains.json");
    }
}

void MainWindow::onLaunchButtonClicked() {
    QString selectedChainName = chainComboBox->currentText();
    QVariantMap selectedChain;

    // Find the selected chain
    for (const QVariantMap &chain : assetChains) {
        if (chain["ac_name"].toString() == selectedChainName) {
            selectedChain = chain;
            break;
        }
    }

    if (selectedChain.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Selected chain not found.");
        return;
    }

    // Determine the executable based on the platform
#ifdef Q_OS_WIN
    QString execExtension = ".exe";
#else
    QString execExtension = "";
#endif

    QString baseExecutable;
    if (komododRadio->isChecked()) {
        baseExecutable = "komodod";
    } else {
        baseExecutable = "komodo-qt";
    }

    // Append the extension if necessary
    selectedExecutable = baseExecutable + execExtension;

    // Build the arguments
    QStringList args;
    for (auto it = selectedChain.begin(); it != selectedChain.end(); ++it) {
        QString key = it.key();
        QVariant value = it.value();

        if (key == "addnode") {
            // Handle addnode separately
            QStringList addnodes = value.toStringList();
            for (const QString &node : addnodes) {
                args << QString("-addnode=%1").arg(node);
            }
        } else {
            args << QString("-%1=%2").arg(key).arg(value.toString());
        }
    }

     // Parse additional parameters entered by the user
    QString additionalParamsText = additionalParamsLineEdit->text().trimmed();
    if (!additionalParamsText.isEmpty()) {
        // Split the additional parameters by spaces
        QRegularExpression regex("\\s+");
        QStringList additionalParams = additionalParamsText.split(regex);
        args << additionalParams;
    }

    // Debug output
    qDebug() << "Launching" << selectedExecutable << "with arguments:" << args;

    // Prepare to launch the executable
    QProcess *process = new QProcess(this);

    // Attempt to start the process
    process->start(selectedExecutable, args);

    // If the process fails to start, try launching from the current directory
    if (!process->waitForStarted()) {
        // Construct the path to the executable in the current working directory
        QString currentDirExecutable = QDir::current().absoluteFilePath(selectedExecutable);

        // Update the process and try again
        process->start(currentDirExecutable, args);

        if (!process->waitForStarted()) {
            QMessageBox::critical(this, "Error",
                                  QString("Failed to start %1. Please ensure it is installed or located in the current directory.")
                                      .arg(selectedExecutable));
            delete process;
            return;
        }
    }

    // Inform the user that the process has started
    QMessageBox::information(this, "Success", QString("%1 started successfully.").arg(selectedExecutable));
}

void MainWindow::openDonatePage() {
    QDesktopServices::openUrl(QUrl("https://getalby.com/p/decker"));
}

void MainWindow::showAboutDialog() {
    QMessageBox::about(this, "About Komodo Launcher",
                       "Komodo Launcher\nVersion 1.0\n\nA simple launcher for Komodo assetchains.");
}