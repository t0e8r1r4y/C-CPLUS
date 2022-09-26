#include "incl.h"

/**
 * @brief ThreadStart::ThreadStart
 * @param type
 * @param cmdCnt
 * @param localPath
 * @param remotePath
 * @param cmd
 */
ThreadStart::ThreadStart(int type, int cmdCnt, QString localPath, QString remotePath, QString cmd)
{
    this->type              = type;
    this->cmdCnt            = cmdCnt;
    this->localPath         = localPath;
    this->remotePath        = remotePath;
    this->cmd               = cmd;
}


/**
 * @brief ThreadStart::~ThreadStart
 */
ThreadStart::~ThreadStart()
{
    qDebug() << Q_FUNC_INFO;
}

/**
 * @brief ThreadStart::run
 * override QThread ( like runnable in JAVA )
 */
void
ThreadStart::run()
{

    int cnt = 0;
    int result = 0;
    int readSize = 0;

    filectl *testFile = new filectl( localPath );
    SshProc *fileTransfer = new SshProc();

    QString localTime;
    QString testCmd;

    qDebug() << testFile->fileNamePtr << " + " << testFile->fileSize;

    char *writeBuffer = (char *)malloc(testFile->fileSize + 1);
    char *readBuffer  = (char *)malloc(testFile->fileSize + 1);
    memset(writeBuffer, 0x00, _msize(writeBuffer));
    memset(readBuffer, 0x00, _msize(readBuffer));


    qDebug() << "write size : " << _msize(writeBuffer) << "|" << testFile->fileSize;
    qDebug() << "read size : " << _msize(readBuffer) << "|" << testFile->fileSize;

    FILE *fp = fopen(testFile->fileNamePtr, "rb");

    if(fp == NULL)
    {
        qDebug() << testFile->fileNamePtr;
        free(testFile->fileNamePtr);
        delete testFile;
        return;
    }

    readSize = fread(writeBuffer, testFile->fileSize, 1, fp);

    qDebug() << readSize;
    fclose(fp);


    fileTransfer->setConnectionInfo("123.123.123.123", "22", "user", "1234");
    fileTransfer->setLocalPath( localPath );
    fileTransfer->setRemotePath( remotePath );

    qDebug() << localPath << " + " << remotePath;


    forever{ // This is macro same as "while(1)". ->

        if ( QThread::currentThread()->isInterruptionRequested() || (cnt >= cmdCnt)) { // interrupt signal or limit
            qDebug() << Q_FUNC_INFO << " terminated";
            free(testFile->fileNamePtr);

            // delete object.
            delete testFile;
            delete fileTransfer;
            return;
        }

        // thread operating
        qDebug() << "test Cnt : " << cnt;
        cnt++;

        localTime   = QDateTime::currentDateTime().toString("hh:mm:ss.z");
        testCmd     = "mv /app/test /app/test_" + localTime;

        fileTransfer->SendCmd(testCmd.toLocal8Bit().data());
        fileTransfer->TcpDisConnection();

        sleep(1);

        fileTransfer->SftpWriteExec(writeBuffer, testFile->fileSize, (remotePath+"/test").toLocal8Bit().data());
        fileTransfer->SftpReadExec(readBuffer, testFile->fileSize, (remotePath+"/test").toLocal8Bit().data() );

        result = fileTransfer->VerifyTransfer(writeBuffer, readBuffer, testFile->fileSize);

        qDebug() << "verift : " << result;

        memset(readBuffer, 0x00, _msize(readBuffer));

        sleep(10);

    }

    free(writeBuffer);
    free(readBuffer);
}
