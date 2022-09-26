#ifndef FILETRANSFER_H
#define FILETRANSFER_H

#include "incl.h"

/**
 * @brief The SshProc class
 *
 */
class SshProc
{
 private:
    LIBSSH2_SESSION *Session;
    LIBSSH2_SFTP *Sftp;
    LIBSSH2_SFTP_HANDLE *SftpHandle;
    LIBSSH2_CHANNEL *channel;
    QTcpSocket TcpSocket;

    QString IP;
    QString Port;
    QString ID;
    QString PW;

    QString LocalPath;  // including file name
    QString RemotePath; // including file name

public:
    ~SshProc();
    int     TcpConnection( void );
    void    TcpDisConnection( void );
    int     SshInit( void );
    int     SftpWriteExec( char* pArr, int sendFileSize, char* remoteServerPath );
    int     SftpReadExec( char* pArr, int sendFileSize, char* remoteServerPath );
    void    ExitSftp();
    int     VerifyTransfer( char* writeMem, char* readMem, int writeSize );
    void    SendCmd(const char *commandLine);

public:
    void    setConnectionInfo( QString inputIP, QString inputPort, QString inputID, QString inputPW );
    void    setLocalPath( QString inputlocalPath );
    void    setRemotePath( QString inputremotePath );

    void    clearConnectionInfo( void );
    void    clearLocalPath( void );
    void    clearRemotePath( void );
};

#endif // FILETRANSFER_H
