
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Semantico;

class MainWindow : public QMainWindow

{
    Q_OBJECT

    Semantico * sem;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_open_clicked();

    void on_save_clicked();

    void on_btnTabela_clicked();

    void on_BipsMipsBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
