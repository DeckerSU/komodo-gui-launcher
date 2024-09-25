#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      chainComboBox(new QComboBox(this)),
      komododRadio(new QRadioButton("komodod", this)),
      komodoQtRadio(new QRadioButton("komodo-qt", this)),
      launchButton(new QPushButton("Launch", this)),
      networkManager(new QNetworkAccessManager(this))
{
    setupUI();
    downloadAssetChains();

    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onDownloadFinished);
    connect(launchButton, &QPushButton::clicked, this, &MainWindow::onLaunchButtonClicked);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Radio Buttons for Executable Choice
    QGroupBox *execGroup = new QGroupBox("Select Executable", this);
    QHBoxLayout *execLayout = new QHBoxLayout(execGroup);
    execLayout->addWidget(komododRadio);
    execLayout->addWidget(komodoQtRadio);
    komododRadio->setChecked(true);

    // Chain Selection
    QHBoxLayout *chainLayout = new QHBoxLayout();
    chainLayout->addWidget(new QLabel("Select Assetchain:", this));
    chainLayout->addWidget(chainComboBox);

    // Add to Main Layout
    mainLayout->addWidget(execGroup);
    mainLayout->addLayout(chainLayout);
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
        QMessageBox::critical(this, "Error", "Failed to download assetchains.json");
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

    // Determine the executable
    if (komododRadio->isChecked()) {
        selectedExecutable = "komodod";
    } else {
        selectedExecutable = "komodo-qt";
    }

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

    // Debug output
    qDebug() << "Launching" << selectedExecutable << "with arguments:" << args;

    // Start the process
    QProcess *process = new QProcess(this);
    process->start(selectedExecutable, args);

    if (!process->waitForStarted()) {
        QMessageBox::critical(this, "Error", "Failed to start the executable.");
        delete process;
    } else {
        QMessageBox::information(this, "Success", "Process started successfully.");
    }
}
