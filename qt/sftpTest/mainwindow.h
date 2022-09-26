#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "incl.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionFunction_Test_triggered();

private:
    Ui::MainWindow *ui;
    TestDialog *fileTransfer;
};
#endif // MAINWINDOW_H
