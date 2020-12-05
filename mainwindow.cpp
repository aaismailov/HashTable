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
    if (!this->collection.size()) {
         ui->pushButton_nor_ok_3->setDisabled(true);
         ui->pushButton_size_3->setDisabled(true);
         ui->pushButton_delete_3->setDisabled(true);
         ui->pushButton_find_ok_3->setDisabled(true);
         ui->pushButton_file_output_ok_3->setDisabled(true);
         ui->pushButton_delete_all_3->setDisabled(true);
         ui->lineEdit_nor_3->setEnabled(false);
         ui->lineEdit_find_3->setEnabled(false);
         ui->lineEdit_file_output_name_3->setEnabled(false);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_size_3_clicked()
{
    QMessageBox::information(this, "Size", "The size of the queue is " + QString::number(this->collection.size()));
}

string check_codon(string codon)
{
    int flag = 0;
    dictionary_hash<string> tableCollection;
    tableCollection.input_from_file("C:\\Qt\\Projects\\Lab2\\genetic_code_table.txt");
    if(!codon.size()) {
        return "The field \"Codon\" is empty";
    }
    for (int i = 0; i < codon.size(); i++)
    {
        if (isdigit(codon[i]))
            return "Enter data without digits";
        if (codon[i] != ' ')
            flag = 1;
    }
    if (!tableCollection.in_dictionary(codon))
        return "This codon does not exist in nature,\nsee the table of the genetic code";
    if(!flag)
        return "The field \"Codon\" is empty";
    return "OK";
}

string check_aa(string codon, string aa)
{
    int flag = 0;
    dictionary_hash<string> tableCollection;
    tableCollection.input_from_file("C:\\Qt\\Projects\\Lab2\\genetic_code_table.txt");
    if(!aa.size()) {
        return "The field \"Aminoacid\" is empty";
    }
    for (int i = 0; i < aa.size(); i++)
    {
        if (isdigit(aa[i]))
            return "Enter data without digits";
        if (aa[i] != ' ')
            flag = 1;
    }
    if (tableCollection[codon] != aa)
        return "This amino acid does not match the given codon,\nsee the table of the genetic code";
    if(!flag)
        return "The field \"Aminoacid\" is empty";
    return "OK";
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
    if (vqueue.size()==0)
        QMessageBox::information(this, "K->V", "Codons: "+QString(kstring.c_str())+" don't exist in nature,\nsee the table of the genetic code");
    else {
        QListWidget *txt = new QListWidget();
        txt->resize(300, 400);
        txt->setWindowTitle("K->V");
        for(string k: vqueue)
            txt->addItem(k.c_str());

        txt->show();
    }
    ui->lineEdit_nor_3->clear();
}

void MainWindow::on_pushButton_delete_all_3_clicked()
{
    collection.clear();
    ui->listWidget_3->clear();
    ui->pushButton_nor_ok_3->setDisabled(true);
    ui->pushButton_size_3->setDisabled(true);
    ui->pushButton_delete_3->setDisabled(true);
    ui->pushButton_find_ok_3->setDisabled(true);
    ui->pushButton_file_output_ok_3->setDisabled(true);
    ui->pushButton_delete_all_3->setDisabled(true);
    ui->lineEdit_nor_3->setEnabled(false);
    ui->lineEdit_find_3->setEnabled(false);
    ui->lineEdit_file_output_name_3->setEnabled(false);
}

void MainWindow::on_pushButton_file_input_ok_3_clicked()
{
    ui->listWidget_3->clear();
    string name = ui->lineEdit_file_input_name_3->text().toStdString();
    if(name.size())
    {
        this->collection.clear();
        this->collection.input_from_file(name);
        for(auto &i: this->collection.get())
            for(auto &j: i)
                if (!j.first.empty()&&!j.second.empty())
                    ui->listWidget_3->addItem("Word: " +
                                        QString::fromStdString(j.first) +
                                        "\nCount: " +
                                        QString::fromStdString(j.second));
    }
    QMessageBox::information(this, "Input data", "Successful input data from file");
    ui->lineEdit_file_input_name_3->clear();
    ui->pushButton_nor_ok_3->setDisabled(false);
    ui->pushButton_size_3->setDisabled(false);
    ui->pushButton_delete_3->setDisabled(false);
    ui->pushButton_find_ok_3->setDisabled(false);
    ui->pushButton_file_output_ok_3->setDisabled(false);
    ui->pushButton_delete_all_3->setDisabled(false);
    ui->lineEdit_nor_3->setEnabled(true);
    ui->lineEdit_find_3->setEnabled(true);
    ui->lineEdit_file_output_name_3->setEnabled(true);
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
    if (ui->listWidget_3->currentItem()) {
        char *curr = ui->listWidget_3->currentItem()->text().toLocal8Bit().data();
        string word(strchr(curr,' '),1, strchr(curr,'\n') - strchr(curr,' ') - 1);
        this->collection.delete_word(word);
        ui->listWidget_3->takeItem(ui->listWidget_3->currentRow());
        if (!this->collection.size()) {
            ui->pushButton_nor_ok_3->setDisabled(true);
            ui->pushButton_size_3->setDisabled(true);
            ui->pushButton_delete_3->setDisabled(true);
            ui->pushButton_find_ok_3->setDisabled(true);
            ui->pushButton_file_output_ok_3->setDisabled(true);
            ui->pushButton_delete_all_3->setDisabled(true);
            ui->lineEdit_nor_3->setEnabled(false);
            ui->lineEdit_find_3->setEnabled(false);
            ui->lineEdit_file_output_name_3->setEnabled(false);
        }
    } else {
        QMessageBox::information(this, "Delete codon", "Choose item");
    }
}

void MainWindow::on_pushButton_find_ok_3_clicked()
{
    string codon = ui->lineEdit_find_3->text().toStdString();
    string s = check_codon(codon);
    if (s != "OK")
        QMessageBox::critical(this, "Find word", s.c_str());
    else
        if (this->collection.in_dictionary(codon))
            QMessageBox::information(this, "Find codon",
                                     "Codon " +
                                     QString::fromStdString(codon) +
                                     " is in dictionary\nwith aminoacid " +
                                     QString::fromStdString(this->collection[codon]));
        else
            QMessageBox::information(this, "Find codon",
                                     "Sorry, but there is"
                                     " no such codon in dictionary");
    ui->lineEdit_find_3->clear();
}

void MainWindow::on_pushButton_clicked()
{
    string s1 = check_codon(ui->lineEdit_add_3->text().toStdString());
    string s2 = check_aa(ui->lineEdit_add_3->text().toStdString(), ui->lineEdit_add_4->text().toStdString());

    if(s1 != "OK")
        QMessageBox::critical(this, "Add codon", s1.c_str());
    else if (s2 != "OK")
        QMessageBox::critical(this, "Add codon", s2.c_str());
    else
    {
        if(!this->collection.in_dictionary(ui->lineEdit_add_3->text().toStdString()))
        {
            this->collection.insert(ui->lineEdit_add_3->text().toStdString(), ui->lineEdit_add_4->text().toStdString());
            ui->listWidget_3->addItem("Codon: " + ui->lineEdit_add_3->text() +
                                "\nAminoacid: " + ui->lineEdit_add_4->text());
            ui->pushButton_nor_ok_3->setDisabled(false);
            ui->pushButton_size_3->setDisabled(false);
            ui->pushButton_delete_3->setDisabled(false);
            ui->pushButton_find_ok_3->setDisabled(false);
            ui->pushButton_file_output_ok_3->setDisabled(false);
            ui->pushButton_delete_all_3->setDisabled(false);
            ui->lineEdit_nor_3->setEnabled(true);
            ui->lineEdit_find_3->setEnabled(true);
            ui->lineEdit_file_output_name_3->setEnabled(true);
        }
        else
        {
            QMessageBox::information(this, "Add codon", "This codon also is in dictionary");
        }
    }
    ui->lineEdit_add_3->clear();
    ui->lineEdit_add_4->clear();

}


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
