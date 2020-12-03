#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <QTextEdit>

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

void MainWindow::on_pushButton_size_3_clicked()
{
    QMessageBox::information(this, "Size", "The size of the queue is " + QString::number(this->collection.size()));
}

int check_word(string word)
{
    int flag = 0;
    if(!word.size())
        return 0;
    for (int i = 0; i < word.size(); i++)
    {
        if (isdigit(word[i]))
            return 0;
        if(word[i] != ' ')
            flag = 1;
    }
    if(!flag)
        return 0;
    return 1;
}

void MainWindow::on_pushButton_nor_ok_3_clicked()
{
    string kstring;
    kstring = ui->lineEdit_nor_3->text().toStdString().c_str();
    list<string> vqueue;
    stringstream kstream;
    kstream << kstring;
    string s;
    while (kstream >> s) vqueue.push_back(s);
    vqueue = this->collection.count(vqueue);
    s.clear();
    for (string v: vqueue)
        s.append(v+'\n');
    QMessageBox::information(this, "Vqueue",
                             QString::fromStdString(s));
    ui->lineEdit_nor_3->clear();
}

void MainWindow::on_pushButton_delete_all_3_clicked()
{
    collection.clear();
    ui->listWidget_3->clear();
}

void MainWindow::on_pushButton_file_input_ok_3_clicked()
{
    string name = ui->lineEdit_file_input_name_3->text().toStdString();
    if(name.size())
    {
        this->collection.clear();
        this->collection.input_from_file(name);
        for(auto &i: this->collection.get())
            for(auto &j: i)
                ui->listWidget_3->addItem("Word: " +
                                    QString::fromStdString(j.first) +
                                    "\nCount: " +
                                    QString::fromStdString(j.second));
    }
    QMessageBox::information(this, "Input data", "Successful input data from file");
    ui->lineEdit_file_input_name_3->clear();
}

void MainWindow::on_pushButton_file_output_ok_3_clicked()
{
    string name = ui->lineEdit_file_output_name_3->text().toStdString();
    if(name.size())
        this->collection.output_to_file(name);
    QMessageBox::information(this, "Output data", "Successful output data to file");
    ui->lineEdit_file_output_name_3->clear();
}

void MainWindow::on_pushButton_delete_3_clicked()
{
    char *curr = ui->listWidget_3->currentItem()->text().toLocal8Bit().data();
    string word(strchr(curr,' '),1, strchr(curr,'\n') - strchr(curr,' ') - 1);
    this->collection.delete_word(word);
    ui->listWidget_3->takeItem(ui->listWidget_3->currentRow());
}

void MainWindow::on_pushButton_find_ok_3_clicked()
{
    string codon = ui->lineEdit_find_3->text().toStdString();
    if (!check_word(codon))
        QMessageBox::critical(this, "Find word", "Please, write correct data"
                                                "without digits");
    else
        if (this->collection.in_dictionary(codon))
            QMessageBox::information(this, "Find codon",
                                     "This codon (" +
                                     QString::fromStdString(codon) +
                                     ") is in dictionary with aa " +
                                     QString::fromStdString(this->collection[codon]));
        else
            QMessageBox::information(this, "Find codon",
                                     "Sorry, but there is"
                                     " no such codon in dictionary");
    ui->lineEdit_find_3->clear();
}
/*
void MainWindow::on_pushButton_clicked()
{
    if(!check_word(ui->lineEdit_add_3->text().toStdString()))
        QMessageBox::critical(this, "Add word", "Please, write correct data"
                                                "without digits");
    else
    {
        if(!this->collection.in_dictionary(ui->lineEdit_add_3->text().toStdString()))
        {
            this->collection << ui->lineEdit_add_3->text().toStdString();
            ui->listWidget_3->addItem("Word: " + ui->lineEdit_add_3->text() +
                                "\nCount: " +
                                QString::number(this->collection[ui->
                                    lineEdit_add_3->text().toStdString()]));
        }
        else
        {
            this->collection << ui->lineEdit_add_3->text().toStdString();
            int row = -1;
            for(int i = 0; i < ui->listWidget_3->count(); i++)
            {
                QListWidgetItem* item = ui->listWidget_3->item(i);
                char *str = (char *)item->text().toStdString().c_str();
                if(strstr(str, ui->lineEdit_add_3->text().toLocal8Bit().data()))
                {
                    row = i;
                    break;
                }
            }
            if(row != -1)
            {
               ui->listWidget_3->item(row)->setText("Word: " + ui->lineEdit_add_3->text() +
                                         "\nCount: " +
                                         QString::number(this->
                                         collection.get_count(ui->
                                         lineEdit_add_3->text().toStdString())));
                ui->listWidget_3->update();
            }
        }
    }
    ui->lineEdit_add_3->setText("");
}
*/

void MainWindow::on_pushButton_2_clicked()
{
    dictionary_hash<string> tableCollection;
    tableCollection.input_from_file("C:\\Qt\\Projects\\Lab2\\genetic_code_table.txt");
    QTextEdit *txt = new QTextEdit();
    txt->resize(300, 400);
    txt->setWindowTitle("Genetic code table");
    txt->setFontPointSize(10);
    txt->setText("CODON    AA");
    for(auto &i: tableCollection.get())
        for(auto &j: i)
            txt->append(QString("  %1    %2").arg(j.first.c_str(), j.second.c_str()));
    txt->show();
}
