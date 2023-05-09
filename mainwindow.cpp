
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Lexico.h"
#include "Sintatico.h"
#include "Semantico.h"
#include <iostream>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>

using namespace std;



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



void MainWindow::on_pushButton_clicked()
{
    QString Text = ui->textEdit->toPlainText();

    string text = Text.toStdString();

   cout << text;


    Lexico * lex = new Lexico( text.c_str());
    Sintatico * sint = new Sintatico();
    Semantico * sem = new Semantico();


    try {
        sint->parse(lex, sem);
        ui->setText->setText("compilou");
    }catch(LexicalError const msg){
        string erro = "Erro no Lexico : ";
        erro.append(msg.getMessage());
        ui->setText->setText(erro.c_str());
    } catch (SyntaticError const msg) {
        string erro = "Erro no Sintatico : ";
        erro.append(msg.getMessage());
        ui->setText->setText(erro.c_str());
    }catch(SemanticError const msg){
        string erro = "Erro no Semantico : ";
        erro.append(msg.getMessage());
        ui->setText->setText(erro.c_str());
    }


}






void MainWindow::on_open_clicked()
{
    QString fileContent;

    QString filename= QFileDialog::getOpenFileName(this, tr("Choose File"), "/home", tr("Notas(*.txt)"));


    if(filename.isEmpty())
       return;

    QFile file(filename);

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return;

    QTextStream in(&file);

    fileContent= in.readAll();

    file.close();

    ui->textEdit->clear();
    ui->textEdit->setText(fileContent);
}


void MainWindow::on_save_clicked()
{
    //Save File

    QString filename= QFileDialog::getSaveFileName(this, "Salvar arquivo", "*.txt");

    if (filename.isEmpty())
        return;

    QFile file(filename);


    //Open the file
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    out << ui->textEdit->toPlainText() << "\n";

    file.close();
}


void MainWindow::on_btnTabela_clicked()
{
    ui->tabelaSimbolo->setText("Tabela Simbolo");
}

