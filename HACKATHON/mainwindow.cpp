#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    doneParsing = false;
    repeatActive = false;
    firstParse = true;
    linecount = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_browseButton_clicked()
{
    //open file dialog
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
               tr("Text/Tab Files (*.txt, *.tab)"));
    ui->filePathEdit->setText(fileName);

   if (!fileName.isEmpty()) {
       QFile file(fileName);
       if (!file.open(QIODevice::ReadOnly)) {
           QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
           return;
       }
       QTextStream in(&file);
       ui->textBrowser->setText(in.readAll());
       file.close();
       //ui->lineLabel->setText(QString("Line: %1").arg(lineCount));
   }
   ui->importButton->setFocus();
}



void MainWindow::on_importButton_clicked()
{
   fileName = ui->filePathEdit->text();
   QFile file(fileName);
   if(!fileName.isEmpty()){
       if (file.exists()){
           ui->enterButton->setEnabled(true);
           ui->tempoEdit->setEnabled(true);
           ui->filePathEdit->setEnabled(true);
       }
       else
           QMessageBox::critical(this, tr("Error"), tr("File does not exist"));


   }
   else
       QMessageBox::critical(this, tr("Error"), tr("Not a valid file path"));
   file.close();
   ui->tempoEdit->setFocus();
}

void MainWindow::on_enterButton_clicked()
{
    tempoString = ui->tempoEdit->text();
    tempo = tempoString.toInt();
   // QString str;
    if(tempo==0){
        QMessageBox::critical(this, tr("Error"), tr("Not a valid tempo"));
    }
    else{
        ui->songNameButton->setEnabled(true);
        ui->songNameEdit->setEnabled(true);
    }
    ui->songNameEdit->setFocus();
}

void MainWindow::on_songNameButton_clicked()
{
    songName = ui->songNameEdit->text();
    if(songName!=""){
        ui->goButton->setEnabled(true);
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), tr("Not a valid song name"));
    }
    ui->goButton->setFocus();
}

void MainWindow::on_goButton_clicked()
{
    linecount=0;
    //create output file
    qint64 linecountPos; //position to write linecount to
    outFileName = songName+".PTB";
    QFile outputFile(outFileName);
    if (outputFile.open(QIODevice::ReadWrite)) {
        stream.setDevice(&outputFile);
        stream << tempo << endl;
        linecountPos = stream.pos();
        stream <<  "     " << endl;
        linecount+=2;
    }


    //Find beginning of tabs
    QFile parseFile(fileName);
    parseFile.open(QIODevice::ReadOnly);
    parseIn.setDevice(&parseFile);


    //start parse
    while(!doneParsing){

        readLines();
        int count = 0;

        while(count<noteLengthLine.length()){
            parseColumn(count);
            count++;
        }
    }
    qint64 endfile = stream.pos();
    stream.seek(linecountPos);
    stream << linecount;
    outputFile.resize(endfile-2);
    outputFile.close();
    parseFile.close();

}

void MainWindow::parseColumn(int column)
{
    QChar noteLength = noteLengthLine.at(column);
    if(stringLine[4].at(column)=='*'){
        if(repeatActive){
            repeatActive=false;
            stream << repeatString << endl;
        }
        else{
            qDebug() << "activated repeat";
            repeatActive=true;
        }
    }

    //check for end of measure
    if(stringLine[1].at(column)=='|'){
        if(repeatActive)
            repeatString = repeatString + "\n";
        stream << "-" << endl;
    }
    if(noteLength!=' '&&stringLine[1].length()>column+1)
    {
        QChar nextLength = noteLengthLine.at(column+1);
        if(nextLength=='.'){
            addDotted(column);
        }else{
            if(noteLength=='T'){
                stream << "1,";
                if(repeatActive)
                    repeatString = repeatString + "1,";
                addNotes(column, 1);
            }else if(noteLength=='S'){
                stream << "2,";
                if(repeatActive)
                    repeatString = repeatString + "2,";
                addNotes(column, 1);
            }
            else if(noteLength=='E'){
                stream << "4,";
                if(repeatActive)
                    repeatString = repeatString + "4,";
                addNotes(column, 1);
            }
            else if(noteLength=='Q'){
                stream << "8,";
                if(repeatActive)
                    repeatString = repeatString + "8,";
                addNotes(column, 1);
            }
            else if(noteLength=='H'){
                stream << "16,";
                if(repeatActive)
                    repeatString = repeatString + "16,";
                addNotes(column, 1);
            }
            else if(noteLength=='W'){
                stream << "32,";
                if(repeatActive)
                    repeatString = repeatString + "32,";
                addNotes(column, 1);
            }
            else if(noteLength=='.'){
              //  addDotted(column);
            }
        }
    }
}

void MainWindow::addDotted(int column){
    QChar noteLength=noteLengthLine.at(column);
    if(noteLength=='T'){
        stream << "1,";
        if(repeatActive)
            repeatString = repeatString + "1,";
        addNotes(column, 1);
    }else if(noteLength=='S'){
        stream << "3,";
        if(repeatActive)
            repeatString = repeatString + "3,";
        addNotes(column, 1);
    }
    else if(noteLength=='E'){
        stream << "6,";
        if(repeatActive)
            repeatString = repeatString + "6,";
        addNotes(column, 1);
    }
    else if(noteLength=='Q'){
        stream << "12,";
        if(repeatActive)
            repeatString = repeatString + "12,";
        addNotes(column, 1);
    }
    else if(noteLength=='H'){
        stream << "24,";
        if(repeatActive)
            repeatString = repeatString + "24,";
        addNotes(column, 1);
    }
    else if(noteLength=='W'){
        stream << "48,";
        if(repeatActive)
            repeatString = repeatString + "48,";
        addNotes(column, 1);
    }
    else if(noteLength=='.'){
      //  addDotted(column);
    }
}


void MainWindow::addNotes(int column, int num)
{

    for(int i = 0; i < num; i++)
    {
        for(int string = 1; string < 7; string++){
            if(stringLine[string].at(column)=='-'){
                stream << ",";
                if(repeatActive)
                    repeatString = repeatString + ",";
            }
            else{
                if(stringLine[string].at(column-1)!='-'){
                    if(stringLine[string].at(column-1)>='0'&&stringLine[string].at(column-1)<='9'){
                        last[string]=stringLine[string].at(column-1).digitValue()*10;
                        stream << stringLine[string].at(column-1);
                        if(repeatActive)
                            repeatString = repeatString + stringLine[string].at(column-1);
                        if(stringLine[string].at(column)=='L'){
                            addTied(string);
                        }else{
                            last[string]=last[string]+stringLine[string].at(column).digitValue();
                            stream << stringLine[string].at(column) << ",";
                            if(repeatActive)
                                repeatString = repeatString + stringLine[string].at(column) + ",";
                        }
                    }
                }
                else if(stringLine[string].at(column)=='L'){
                    addTied(string);
                }else{
                    last[string]=0;
                    last[string]=stringLine[string].at(column).digitValue();
                    stream << stringLine[string].at(column) << ",";
                    if(repeatActive)
                        repeatString = repeatString + stringLine[string].at(column) + ",";
                }


            }
        }
        stream << endl;
        linecount++;
        if(repeatActive){
            repeatString = repeatString + "\n";
            linecount++;
        }

    }
}

void MainWindow::addTied(int string)
{
    stream << "L,";
    /*int fret = last[string];
    stream << fret << ",";*/
}

bool MainWindow::readLines()
{
    QString lastLine = "";
    QString line = "";

    //look for || on first parse and '-' on all others
    if (firstParse){
        bool j = 1;
        while(j){
            if(line.length()>2){
                if(line.at(1)=='|'&&line.at(2)=='|'){
                   j = 0;
                }
                else{
                    lastLine=line;
                    line=parseIn.readLine();
                    startLine++;
                }
            }
            else{
                lastLine=line;
                line=parseIn.readLine();
            }
        }
        firstParse = false;
    }else{
        bool j = 1;
        while(j){
            if(line.length()>2){
                if(line.at(1)=='-'){
                   j = 0;
                }
                else{
                    lastLine=line;
                    line=parseIn.readLine();
                    startLine++;
                }
            }
            else{
                lastLine=line;
                line=parseIn.readLine();
            }
        }
    }

    noteLengthLine=lastLine;
    qDebug() << "noteLengthLine: " << noteLengthLine;

    stringLine[1]=line;
    qDebug() << "String1Line   : " << stringLine[1];

    stringLine[2]=parseIn.readLine();
    qDebug() << "String2Line   : " << stringLine[2];

    stringLine[3]=parseIn.readLine();
    qDebug() << "String3Line   : " << stringLine[3];

    stringLine[4]=parseIn.readLine();
    qDebug() << "String4Line   : " << stringLine[4];

    stringLine[5]=parseIn.readLine();
    qDebug() << "String5Line   : " << stringLine[5];

    stringLine[6]=parseIn.readLine();
    qDebug() << "String6Line   : " << stringLine[6];

    //check if done with tab by looking for ||
    if (stringLine[4].at(stringLine[4].length()-1)=='|'&&stringLine[4].at(stringLine[4].length()-2)=='|')
        doneParsing=true;


    return true;
}

void MainWindow::on_filePathEdit_returnPressed()
{
   on_importButton_clicked();
}

void MainWindow::on_tempoEdit_returnPressed()
{
    on_enterButton_clicked();
}

void MainWindow::on_songNameEdit_returnPressed()
{
    on_songNameButton_clicked();
}


