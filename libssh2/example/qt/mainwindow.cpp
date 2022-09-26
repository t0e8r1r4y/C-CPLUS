#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->toolButton_LocalFile, SIGNAL(pressed()), this, SLOT(local_file_select()));
    connect(ui->pushButton_Transfer, SIGNAL(pressed()), this, SLOT(sftp_write()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Init()
{
}

void MainWindow::ExitApp()
{
    libssh2_session_disconnect(Session,"Exit");
    libssh2_session_free(Session);

    TcpSocket.disconnectFromHost();
}

void MainWindow::SftpWriteExec()
{
    if(ui->lineEdit_Username->text().isEmpty() || ui->lineEdit_Server->text().isEmpty() ||ui->lineEdit_RemoteFile->text().isEmpty()
            ||ui->lineEdit_Password->text().isEmpty() ||ui->lineEdit_LocalFile->text().isEmpty())
    {
        ui->textBrowser->append("LineEdit is empty.");
        return;
    }

    int rc=0;

    rc=libssh2_init(0);
    if(rc!=0)
    {
        ui->textBrowser->append(QString("libssh2 initialization failed (%1)").arg(rc));
        return;
    }

    FILE *local;
    local = fopen(ui->lineEdit_LocalFile->text().toLocal8Bit().data(), "rb");
    if (!local) {
        ui->textBrowser->append("can not open file");
       return;
    }

    TcpSocket.connectToHost(ui->lineEdit_Server->text(),22);

    if(!TcpSocket.waitForConnected())
    {
        ui->textBrowser->append(QString("Socket Error: %1").arg(TcpSocket.errorString()));
        ExitApp();
        return;
    }

    Session=libssh2_session_init();
    if(!Session)
    {
        ui->textBrowser->append(QString("Session Error"));
        ExitApp();
        return;
    }

    libssh2_session_set_blocking(Session,1);
    rc=libssh2_session_handshake(Session,TcpSocket.socketDescriptor());

    if(rc)
    {
        ui->textBrowser->append(QString("Failure establishing SSH session: %1").arg(rc));
        ExitApp();
        return;
    }

    QString Hash=libssh2_hostkey_hash(Session,LIBSSH2_HOSTKEY_HASH_SHA1);
   // ui->textBrowser->append(Hash);

   /* char Username[100]={0,};
    qsnprintf(Username,sizeof(Username),"%s",ui->lineEdit_Username->text().toUtf8().data());
    char Password[100]={0,};
    qsnprintf(Password,sizeof(Password),"%s", ui->lineEdit_Password->text().toUtf8().data());*/

    if(!ui->lineEdit_Password->text().isEmpty())
    {

        if(libssh2_userauth_password(Session,ui->lineEdit_Username->text().toLocal8Bit().data(),ui->lineEdit_Password->text().toLocal8Bit().data()))
        {
            ui->textBrowser->append("Authentication by password failed.");
            ExitApp();
        }
    }
    else
    {
        if(libssh2_userauth_publickey_fromfile(Session,ui->lineEdit_Username->text().toLocal8Bit().data(),"/home/username/.ssh/id_rsa.pub",
                                               "/home/username/.ssh/id_rsa",
                                               ui->lineEdit_Password->text().toLocal8Bit().data()))
        {
            ui->textBrowser->append("Authentication by public key failed.");
            ExitApp();
        }
    }

    ui->textBrowser->append("libssh2_sftp_init()!\n");
    Sftp = libssh2_sftp_init(Session);

    if (!Sftp)
    {
        ui->textBrowser->append("Unable to init SFTP session");
        ExitApp();
    }

    ui->textBrowser->append("libssh2_sftp_open()!");

    char RemoteFile[1024]={0,};
    qsnprintf(RemoteFile,ui->lineEdit_RemoteFile->text().toUtf8().size()+1,"%s",ui->lineEdit_RemoteFile->text().toUtf8().data());
    qDebug()<<RemoteFile;

    SftpHandle=libssh2_sftp_open(Sftp,RemoteFile,
                                 LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC,
                                 LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR|
                                 LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH);

    if (!SftpHandle) {
        ui->textBrowser->append("Unable to open file with SFTP");
        ExitApp();
    }

    ui->textBrowser->append("libssh2_sftp_open() is done, now send data!\n");

    char mem[1024*100];
    size_t nread;
    char *ptr;

    do {
        nread = fread(mem, 1, sizeof(mem), local);
        if (nread <= 0) {

            break;
        }
        ptr = mem;
        do {
            rc = libssh2_sftp_write(SftpHandle, ptr, nread);

            if(rc < 0)
                break;
            ptr += rc;
            nread -= rc;

        } while (nread);
    } while (rc > 0);


    libssh2_sftp_close(SftpHandle);
    libssh2_sftp_shutdown(Sftp);

    ExitApp();

    ui->textBrowser->append("All done");
    libssh2_exit();
}

void MainWindow::sftp_write()
{
    SftpWriteExec();
}

void MainWindow::local_file_select()
{
    ui->lineEdit_LocalFile->setText(QFileDialog::getOpenFileName(this,"SelectFile",QApplication::applicationDirPath(),"All Files(*.*)"));
}
