#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtNetwork>
#include <QWebPage>
#include <QWebFrame>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void pullSite(void);

signals:

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *mManager;

private slots:
    void replyFinished(QNetworkReply *reply);
};

#endif // MAINWINDOW_H
