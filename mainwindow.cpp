
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


//extern list<Simbolo> tabelaSimbolo;
//extern list<Simbolo> tabelaSimboloFuncoes;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->sem = NULL;
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

    if(sem) {
        delete sem;
    }
    this->sem = new Semantico();



    try {
        ui->setText->setText("");
        sint->parse(lex, sem);
        ui->setText->append("compilou");
        string warning;

        ui->setText->append("------------------------Warning--------------------");
        for(Warning war : this->sem->listaWar){
            warning.append(war.id);
            warning.append("| escopo: ");
            warning.append(to_string(war.escopo));
            warning.append("| motivo: ");
            warning.append(war.aviso);
            ui->setText->append(warning.c_str());
            ui->setText->append("----------------------------------------------------");
        }

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

#include <QDebug>
void MainWindow::on_btnTabela_clicked()
{
    string tabelaSimbolosMsg;
        ui->tabelaSimbolo->setText("");
    if(!sem) {
        ui->tabelaSimbolo->setText("A compilação não foi realizada");
        return;
    }else{
        ui->tabelaSimbolo->append("----------------------Tokens das Funcoes----------------------------");
        for(Simbolo sim : this->sem->tabelaSimboloFuncoes){
            cout<<sim.tipo<<" "<<sim.id<<" escopo:"<<sim.escopo<<" init:"<<sim.inicializado<<" usada:"<<sim.usado<<" funcao:"<<sim.funcao<<" vetor:"<<sim.vetor<<" posVetor:"<<sim.posVetor<<" paremtro:"<<sim.parametro<<endl;
            tabelaSimbolosMsg.append(sim.tipo);
            tabelaSimbolosMsg.append(" ");
            tabelaSimbolosMsg.append(sim.id);
            tabelaSimbolosMsg.append("| escopo: ");
            tabelaSimbolosMsg.append(to_string(sim.escopo));
            tabelaSimbolosMsg.append("| inciada: ");
            tabelaSimbolosMsg.append(to_string(sim.inicializado));
            tabelaSimbolosMsg.append("| usada: ");
            tabelaSimbolosMsg.append(to_string(sim.usado));
            tabelaSimbolosMsg.append("| funcao: ");
            tabelaSimbolosMsg.append(to_string(sim.funcao));
            tabelaSimbolosMsg.append("| parametro: ");
            tabelaSimbolosMsg.append(to_string(sim.parametro));
            tabelaSimbolosMsg.append("| vetor: ");
            tabelaSimbolosMsg.append(to_string(sim.vetor));
            tabelaSimbolosMsg.append("| posVetor: ");
            tabelaSimbolosMsg.append(to_string(sim.posVetor));

            ui->tabelaSimbolo->append(tabelaSimbolosMsg.c_str());
            ui->tabelaSimbolo->append("-----------------------------------------------------------------");

            tabelaSimbolosMsg.clear();
        }
        ui->tabelaSimbolo->append("----------------------Tokens da Main----------------------------");


        for(Simbolo sim : this->sem->tabelaSimbolo){
            cout<<sim.tipo<<" "<<sim.id<<" escopo:"<<sim.escopo<<" init:"<<sim.inicializado<<" usada:"<<sim.usado<<" funcao:"<<sim.funcao<<" vetor:"<<sim.vetor<<" posVetor:"<<sim.posVetor<<" paremtro:"<<sim.parametro<<endl;
            tabelaSimbolosMsg.append(sim.tipo);
            tabelaSimbolosMsg.append(" ");
            tabelaSimbolosMsg.append(sim.id);
            tabelaSimbolosMsg.append("| escopo: ");
            tabelaSimbolosMsg.append(to_string(sim.escopo));
            tabelaSimbolosMsg.append("| inciada: ");
            tabelaSimbolosMsg.append(to_string(sim.inicializado));
            tabelaSimbolosMsg.append("| usada: ");
            tabelaSimbolosMsg.append(to_string(sim.usado));
            tabelaSimbolosMsg.append("| funcao: ");
            tabelaSimbolosMsg.append(to_string(sim.funcao));
            tabelaSimbolosMsg.append("| parametro: ");
            tabelaSimbolosMsg.append(to_string(sim.parametro));
            tabelaSimbolosMsg.append("| vetor: ");
            tabelaSimbolosMsg.append(to_string(sim.vetor));
            tabelaSimbolosMsg.append("| posVetor: ");
            tabelaSimbolosMsg.append(to_string(sim.posVetor));

            ui->tabelaSimbolo->append(tabelaSimbolosMsg.c_str());
            ui->tabelaSimbolo->append("-----------------------------------------------------------------");

            tabelaSimbolosMsg.clear();
        }

    }

    for(Simbolo sim : this->sem->tabelaSimbolo){
        cout<<sim.tipo<<" "<<sim.id<<" escopo:"<<sim.escopo<<" init:"<<sim.inicializado<<" usada:"<<sim.usado<<" funcao:"<<sim.funcao<<" vetor:"<<sim.vetor<<" posVetor:"<<sim.posVetor<<" paremtro:"<<sim.parametro<<endl;
    }
}

