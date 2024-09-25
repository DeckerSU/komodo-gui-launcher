#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QRadioButton>
#include <QPushButton>
#include <QGroupBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QProcess>
#include <QLineEdit>
#include <QMenuBar>
#include <QAction>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void downloadAssetChains();
    void onDownloadFinished(QNetworkReply* reply);
    void onLaunchButtonClicked();
    void openDonatePage();
    void showAboutDialog();

private:
    void parseAssetChains(const QByteArray& data);
    void setupUI();

    QLineEdit *additionalParamsLineEdit;
    QComboBox *chainComboBox;
    QRadioButton *komododRadio;
    QRadioButton *komodoQtRadio;
    QPushButton *launchButton;

    QNetworkAccessManager *networkManager;
    QList<QVariantMap> assetChains;

    QString selectedExecutable;
};

#endif // MAINWINDOW_H
