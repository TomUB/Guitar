#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>
#include <QFile>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString fileName;
    QString tempoString;
    QString songName;
    QString noteLengthLine;
    QString stringLine[7];

    QTextStream parseIn;
    int tempo;
    int startLine;
    bool doneParsing;
    bool repeatActive;
    bool firstParse;
    QString repeatString;
    QString outFileName;
    QTextStream stream;
    int last[7];
    int linecount;

    bool readLines(void);
    void parseColumn(int column);
    void addNotes(int column, int num);
    void addDotted(int column);
    void addTied(int string);


private slots:
    void on_browseButton_clicked();

    void on_importButton_clicked();

    void on_enterButton_clicked();

    void on_songNameButton_clicked();

    void on_goButton_clicked();

    void on_filePathEdit_returnPressed();

    void on_tempoEdit_returnPressed();

    void on_songNameEdit_returnPressed();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
