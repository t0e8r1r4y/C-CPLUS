#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QFile>
#include <QFileDialog>

/* 라이브러리 추가*/
#include "lib/libssh2.h"
#include "lib/libssh2_config.h"
#include "lib/libssh2_publickey.h"
#include "lib/libssh2_sftp.h"

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
    void sftp_write();
    void local_file_select();

private:
    Ui::MainWindow *ui;
    LIBSSH2_SESSION *Session;
    LIBSSH2_SFTP *Sftp;
    LIBSSH2_SFTP_HANDLE *SftpHandle;
    LIBSSH2_CHANNEL *channel;
    QTcpSocket TcpSocket;

    void Init();
    void ExitApp();
    void SftpWriteExec();

};
#endif // MAINWINDOW_H
