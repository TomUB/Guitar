#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mManager = new QNetworkAccessManager(this);
    connect(mManager, SIGNAL(finished(QNetworkReply*)), SLOT(replyFinished(QNetworkReply*)));
    pullSite();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::pullSite(){
    //QString url = "https://drive.google.com/folderview?id=0Bza68mSFcbkXN2VDWjRKcl9xS00&usp=sharing";
    QString url = "https://doc-0g-as-docs.googleusercontent.com/docs/securesc/ha0ro937gcuc7l7deffksulhg5h7mbp1/7ptsljg5ku3l0fbs7jc24p9e4v937s5g/1397584800000/16706253371665976384/*/0Bza68mSFcbkXWFc3cmNEZ2pwMzQ?h=16653014193614665626&e=download";
    QNetworkRequest r(url);
    mManager->get(r);
}

void MainWindow::replyFinished(QNetworkReply *reply){
    if (reply->error() != QNetworkReply::NoError){
        qWarning() << "ERROR:" << reply->errorString();
        return;
    }

    // Scrap HTML
    QString html = reply->readAll();
    QWebPage page;
    QWebFrame *frame = page.mainFrame();
    frame->setHtml(html);
    ui->textBrowser->setText(html);
}
