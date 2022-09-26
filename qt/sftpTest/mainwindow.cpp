#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "incl.h"

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


void MainWindow::on_actionFunction_Test_triggered()
{
    fileTransfer = new TestDialog(this);
    fileTransfer->setModal(true);
    fileTransfer->exec();

    delete fileTransfer;

    return;
}

