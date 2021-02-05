#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_2_clicked()
{
    QFile file ("C:\\Users\\PXD\\Desktop\\Fisier.txt");

    if(!file.open(QFile::ReadOnly | QFile :: Text)){
        QMessageBox::warning(this,"title","file not open");
    }
    QTextStream in(&file);
 //   QString text = in.readAll();
    ui->plainTextEdit->setPlainText(in.readAll());
    file.close();
}


void MainWindow::on_pushButton_clicked()
{
    QFile file ("C:\\Users\\PXD\\Desktop\\Fisier.txt");

    if(!file.open(QFile::ReadOnly | QFile :: Text)){
        QMessageBox::warning(this,"title","file not open");
    }

    QString tag[200],group[200], dataType[200], dataValue[200],tagName[200];
    QTextStream in(&file);

    QChar chCurent;
    int CounterOfTags=0,CounterOfTypes=0,CounterOfValues=0,CounterOfNames=0;
    bool ok=0;

    // Facem parsarea pe text
    //Tabelul va fi de forma: TAG -> (tag) || TIP -> 2 litere (Us ,ob,ul etc) || [VALOARE] || # .. NUME
    while (!in.atEnd())
      {
         in >> chCurent;
         // Identificam tag-ul si in punem in variabila tag
         // tagul are forma (...)
         if( chCurent=='(' )
         {
             do
             {
              tag[CounterOfTags].append(chCurent);
              in>>chCurent;
             }while(chCurent!=')');
             tag[CounterOfTags].append(chCurent); // Adaugam ) la finalul tagului
             CounterOfTags ++;
         }

         //Identificam TIPUL care este alcatuit din 2 litere consecutive
             if ((chCurent!=' ' && chCurent != ')') && (CounterOfTypes==CounterOfTags-1)) //(chCurent.isLetter() || chCurent.isSymbol()
             {

                 dataType[CounterOfTypes].append(chCurent); // adaugam litera
                 in >> chCurent; //citim si urmatoarea litera
                 dataType[CounterOfTypes].append(chCurent); // adaugam cea de a doua litera
                 CounterOfTypes ++;
             }


             //Dupa TIP urmeaza intotdeauna VALOAREA care poate avea diverse forme: intre [], intre (), numere etc.
             //Pentru a lua valoarea luam tot pana la # si taiem spatiile albe

               if( CounterOfValues==CounterOfTypes-1)
               {
                   in >> chCurent; // Citim urmatorul caracter pentru ca acum e pe ultima litera de la TIP
                   while(chCurent==' ')
                   {
                         in >> chCurent;  //Sarim spatiile albe
                   }
                 while(chCurent!='#')
                 {
                      if(chCurent=='\n')  //exceptie ca in linia 36 din fisier cu Bistrita
                          in >> chCurent;
                     dataValue[CounterOfValues].append(chCurent);
                     in >> chCurent;
                 }

                        CounterOfValues ++;

               }

               // Citim Numele Tagului
               // Luam toate literele pana la sfarsitul liniei
               if( CounterOfNames ==  CounterOfValues-1)
               {
                 while(chCurent != '\n' && (chCurent.isLetter() || chCurent=='_'))
                 {
                    tagName[CounterOfNames].append(chCurent);
                     in >> chCurent;
                     ok=1;  //Semafor care nu lasa sa se incrementeze conturul de nume pana n-o citit un nume
                 }
                 if(ok) {CounterOfNames ++,ok=0;}
                }
      }

    // Cream Tabelul si il populam

    ui->tableWidget->setRowCount(CounterOfTags);
    ui->tableWidget->setColumnCount(5);
    QStringList labels = {"Tag ",  "Tip" , "Valoare" , "Nume", " "};
    ui->tableWidget->setHorizontalHeaderLabels(labels);

     for(int i=0; i<ui->tableWidget->rowCount(); i++)
         {
             for(int j=0; j<ui->tableWidget->columnCount(); j++)
             {
                 QTableWidgetItem *Tag = ui->tableWidget->item(i, j);
                 QTableWidgetItem *Type = ui->tableWidget->item(i, j);
                  QTableWidgetItem *Value = ui->tableWidget->item(i, j);
                   QTableWidgetItem *Name = ui->tableWidget->item(i, j);
                 if(!Tag)
                 {
                     Tag = new QTableWidgetItem;
                     Type = new QTableWidgetItem;
                     Value = new QTableWidgetItem;
                     Name = new QTableWidgetItem;
                      ui->tableWidget->setItem(i, 0, Tag); //Tag
                      ui->tableWidget->setItem(i, 1, Type);// TIp
                      ui->tableWidget->setItem(i, 2, Value);// Valoarea
                      ui->tableWidget->setItem(i, 3, Name);// Nume
                 }
                 Tag->setText(tag[i]);
                 Type->setText(dataType[i]);
                 Value->setText(dataValue[i]);
                 Name->setText(tagName[i]);
             }
         }
}
