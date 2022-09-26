#include "sshproc.h"

/**
 * @brief SshProc::~SshProc
 */
SshProc::~SshProc()
{
    clearConnectionInfo();
    clearLocalPath();
    clearRemotePath();
}


// public function start =========================================================================

/**
 * @brief SshProc::setConnectionInfo
 * @param inputIP
 * @param inputPort
 * @param inputID
 * @param inputPW
 *
 * Input values from UI( MainWidonw ) are set in private value;
 */
void
SshProc::setConnectionInfo( QString inputIP, QString inputPort, QString inputID, QString inputPW )
{
    IP = inputIP;
    Port = inputPort;
    ID = inputID;
    PW = inputPW;

    return;
}

/**
 * @brief SshProc::setLocalPath
 * @param inputlocalPath
 */
void
SshProc::setLocalPath( QString inputlocalPath )
{
    LocalPath = inputlocalPath;
    return;
}

/**
 * @brief SshProc::setRemotePath
 * @param inputremotePath
 */
void
SshProc::setRemotePath( QString inputremotePath )
{
    RemotePath = inputremotePath;
    return;
}

/**
 * @brief SshProc::clearConnectionInfo
 */
void
SshProc::clearConnectionInfo( void )
{
    IP      = "";
    Port    = "";
    ID      = "";
    PW      = "";
    return;
}

/**
 * @brief SshProc::clearLocalPath
 */
void
SshProc::clearLocalPath( void )
{
    LocalPath = "";
    return;
}

/**
 * @brief SshProc::clearRemotePath
 */
void
SshProc::clearRemotePath( void )
{
    RemotePath = "";
    return;
}

// public function end =========================================================================


/**
 * @brief SshProc::TcpConnection
 * @return On Success, this function return 0. but 1 is returned when Tcp Connection Fail
 */
int
SshProc::TcpConnection( void )
{
    int rc = 0;
    TcpSocket.connectToHost(IP, Port.toShort());
    if( !TcpSocket.waitForConnected())
    {
        TcpDisConnection();
        rc = 1;
    }
    return rc;
}

/**
 * @brief SshProc::TcpDisConnection
 */
void
SshProc::TcpDisConnection( void )
{
    TcpSocket.disconnectFromHost();
    return;
}


/**
 * @brief SshProc::SshInit
 * @return On Success, this function returns 0. but failed, return values are below
 *
 * On Failed,
 * "1" means that libssh2 init function is failed. "libssh2_init" function is return negative value.
 * "2" means that that Session init function is failed.
 * "3" means that tpc connection fail.
 * "4" means that Session handshaking function is failed.
 * "5" meanse that Authentication by password failed.
 * "6" is not used.
 * "7" means that unable to init SFTP session
 */
int
SshProc::SshInit( void )
{
    int rc = 0;

    /* step1. libssh initialize
     * (Tip) "libssh2_init" functino is not thread safety.
     */
    rc = libssh2_init(0);
    if(rc != 0)
    {
        qDebug() << QString("libssh2 initialization failed (%1)\r\n").arg(rc);
        rc = 1;
        return rc;
    }

    /* step2. Session initialize
     * (Tip) libssh2_session_init function returns that pointer to a newly allocated LIBSSH2_SESSION.
     *       if failed, return NULL or errors.
     */
    Session = libssh2_session_init();
    if(!Session)
    {
        ExitSftp();
        rc = 2;
        return rc;
    }

    /* step3. Tcp Connection
     * (Opinion)
     */
    if( TcpConnection() )
    {
        ExitSftp();
        rc = 3;
        qDebug() << QString("Failure Tcp Connection: %1").arg(rc);
        return rc;
    }

    /* step4. Session handshake
     * (Tip) libssh2_session_set_blocking function's second arg is blocking / non-blocking flag
     *       args "1" is blocking
     *       args "0" is non-blocking
     *
     *       Set or clear blocking mode on the selected on the session.
     *       This will instantly affect any channels associated with this session.
     *       If a read is performed on a session with no data currently available,
     *       a blocking session will wait for data to arrive and return what it receives.
     *
     *       A non-blocking session will return immediately with an empty buffer.
     *       If a write is performed on a session with no room for more data,
     *       a blocking session will wait for room.
     *       A non-blocking session will return immediately without writing anything.
     */
    libssh2_session_set_blocking(Session,1);

    /** Begin transport layer protocol negotiation with the connected host.
     *  On Success, this function is returnd 0, negative on failure.
     */
    rc = libssh2_session_handshake(Session, TcpSocket.socketDescriptor());
    if( rc )
    {
        ExitSftp();
        qDebug() << QString("Failure establishing SSH session: %1").arg(rc);
        rc = 4;
        return rc;
    }

    /* step5. Key Value save in QString Hash
     * (Tip) libssh2_hostkey_hash function return hostkey hash
     *       The length of the returned string is hash_type specific (e.g. 16 bytes for MD5, 20 bytes for SHA1, 32 bytes for SHA256).
     */
    QString Hash=libssh2_hostkey_hash(Session,LIBSSH2_HOSTKEY_HASH_SHA1);

    /**
     * (opinion) local PC's rsa value is not used. because, I don't want to add operator to make rsa key and find.
     */
    if(!PW.isEmpty())
    {
        if(libssh2_userauth_password(Session, ID.toLocal8Bit().data(), PW.toLocal8Bit().data()))
        {
            qDebug() << "Authentication by password failed.\r\n";
            ExitSftp();
            rc = 5;
            return rc;
        }
    }
    else
    {
        if(libssh2_userauth_publickey_fromfile(Session,ID.toLocal8Bit().data(),"/home/username/.ssh/id_rsa.pub", "/home/username/.ssh/id_rsa", PW.toLocal8Bit().data()))
        {
            qDebug() << "Authentication by public key failed.\r\n";
            ExitSftp();
            rc = 6;
            return rc;
        }
    }


    /* step6. sftp init
     * (Tip) Open a channel and initialize the SFTP subsystem. This means that open a channer for FTP in SSH Layer.
     *       (SSH Layer is part of TCP Layer)
     *       Although the SFTP subsystem operates over the same type of channel as those exported by the Channel API,
     *       the protocol itself implements its own unique binary packet protocol which must be managed with the libssh2_sftp_*() family of functions.
     *       When an SFTP session is complete, it must be destroyed using the libssh2_sftp_shutdown function.
     */
    Sftp = libssh2_sftp_init(Session);

    /**
     *  libssh2_sftp_init function error.
     *  LIBSSH2_ERROR_ALLOC - An internal memory allocation call failed.
     *  LIBSSH2_ERROR_SOCKET_SEND - Unable to send data on socket.
     *  LIBSSH2_ERROR_SOCKET_TIMEOUT -
     *  LIBSSH2_ERROR_SFTP_PROTOCOL - An invalid SFTP protocol response was received on the socket, or an SFTP operation caused an errorcode to be returned by the server.
     *  LIBSSH2_ERROR_EAGAIN - Marked for non-blocking I/O but the call would block.
     */

    if(!Sftp)
    {
        qDebug() << "Unable to init SFTP session.\r\n";
        libssh2_sftp_shutdown(Sftp);
        ExitSftp();
        rc = 7;
        return rc;
    }
    rc = 0;

    return rc;
}



/**
 * @brief SshProc::SftpWriteExec
 * @return On success return 0, Not Zero is failed.
 */
int
SshProc::SftpWriteExec( char* pArr, int sendFileSize, char* remoteServerPath )
{

    /**
     * @brief (int)     rc is return value
     * @brief (char)    RemoteFile is file Name in local dir ( for upload remote server )
     * @brief (char*)   mem is buffer of file to send remote server. allocate memory.
     * @brief (int)     nread is return of fread function.
     * @brief (int)     fileSize is return of fseek function.
     * @brief (File*)   File Pointer ( using kernel buffer stream )
     */
    int rc = 0;
    int nread = sendFileSize;
    char *RemoteFile = remoteServerPath;
    char *mem = pArr;

    rc = SshInit();

    if( rc )
    {
        qDebug() << "SftpWriteExec fail";
        return rc;
    }

    /**
     * sftp_session_init function is operated on SshInit() functino.
     *
     */
    SftpHandle = libssh2_sftp_open(Sftp,RemoteFile,
                                 LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC,
                                 LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR|
                                 LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH);

    if(!SftpHandle)
    {
        libssh2_sftp_close(SftpHandle);
        TcpDisConnection();
        libssh2_sftp_close_handle(SftpHandle);
        libssh2_sftp_shutdown(Sftp);
        ExitSftp();
        libssh2_exit();

        qDebug() << "SftpHandle fail";

        return 1;
    }

    do {

        if (nread <= 0) {

            break;
        }

        do {
            rc = libssh2_sftp_write(SftpHandle, mem, nread);

            if(rc < 0)
                break;
            mem += rc;
            nread -= rc;

        } while (nread);

    } while (rc > 0);

    TcpDisConnection();
    libssh2_sftp_close_handle(SftpHandle);
    libssh2_sftp_shutdown(Sftp);
    ExitSftp();
    libssh2_exit();

    rc = 0;

    return rc;
}



/**
 * @brief SshProc::SftpReadExec
 * @return
 */
int
SshProc::SftpReadExec( char* pArr , int sendFileSize, char* remoteServerPath )
{

    char *RemoteFile = remoteServerPath;
    char *mem = pArr;
    int  rc = 0;

    rc = SshInit();
    if( rc ) return rc;

    SftpHandle = libssh2_sftp_open(Sftp, RemoteFile,LIBSSH2_FXF_READ,0);

    if(!SftpHandle)
    {
        libssh2_sftp_close(SftpHandle);
        ExitSftp();
        return 1;
    }

    do
    {
        rc = libssh2_sftp_read(SftpHandle, mem, sendFileSize);

        if(rc > 0)
        {
//            _write(1, mem, rc);
        }
        else
        {
            break;
        }

    } while(1);

    TcpDisConnection();
    libssh2_sftp_close_handle(SftpHandle);
    libssh2_sftp_shutdown(Sftp);
    ExitSftp();
    libssh2_exit();

    return 0;
}

/**
 * @brief SshProc::ExitSftp
 */
void
SshProc::ExitSftp( void )
{
    libssh2_session_disconnect(Session,"Exit");
    libssh2_session_free(Session);
}


/**
 * @brief SshProc::VerifyTransfer
 * @param writeMem
 * @param readMem
 * @return On Success, 0 is return. negative is failed.
 */
int
SshProc::VerifyTransfer( char* writeMem, char* readMem, int writeSize )
{
    int rc  = 0;
    int i   = 0;

    for( i = 0; i < writeSize; i++)
    {
        if(writeMem[i] != readMem[i])
        {
            rc = i;
            break;
        }
    }

//    if( i != writeSize) rc = 1;

    return rc;
}



void
SshProc::SendCmd( const char *commandLine )
{
    qDebug() << commandLine;

    int rc = 0;
    LIBSSH2_KNOWNHOSTS *nh;
    const char *fingerprint;
    size_t len;
    int type;

    int exitcode;
    char *exitsignal = (char *)"none";

    if( TcpConnection() )
    {
        qDebug()  << "Socket Connection Fail";
        return;
    }

    Session = libssh2_session_init();
    // channel

    if(!Session)
    {
        qDebug()  << "SendCmd function Session init fail";
        return;
    }

    libssh2_session_set_blocking(Session, 0); // tell libssh2 we want it all done non-blocking

    while((rc = libssh2_session_handshake(Session, TcpSocket.socketDescriptor())) == LIBSSH2_ERROR_EAGAIN);
    if(rc)
    {
        qDebug()  << "SendCmd function Session set non blocking fail";
        return;
    }

    nh = libssh2_knownhost_init(Session);
    if(!nh)
    {
        qDebug()  << "SendCmd function libssh2_knownhost_init fail";
        return;
    }

    libssh2_knownhost_readfile(nh, "known_hosts",

                               LIBSSH2_KNOWNHOST_FILE_OPENSSH);

    // store all known hosts to here
    libssh2_knownhost_writefile(nh, "dumpfile",

                                LIBSSH2_KNOWNHOST_FILE_OPENSSH);

    fingerprint = libssh2_session_hostkey(Session, &len, &type);

    if(fingerprint)
    {
        struct libssh2_knownhost *host;
        int check = libssh2_knownhost_checkp(nh, IP.toLocal8Bit().data(), 22,

                                             fingerprint, len,
                                             LIBSSH2_KNOWNHOST_TYPE_PLAIN|
                                             LIBSSH2_KNOWNHOST_KEYENC_RAW,
                                             &host);

    }
    else
    {
        return;
    }
    libssh2_knownhost_free(nh);

    if(strlen(PW.toLocal8Bit().data()) != 0) {
        // We could authenticate via password
        while((rc = libssh2_userauth_password(Session, ID.toLocal8Bit().data(), PW.toLocal8Bit().data())) == LIBSSH2_ERROR_EAGAIN);
        if(rc) {
//            fprintf(stderr, "Authentication by password failed.\n");
            qDebug()  << "Authentication by password failed." ;
            goto shutdown;
        }
    }
    else {
        // Or by public key
        while((rc = libssh2_userauth_publickey_fromfile(Session, ID.toLocal8Bit().data(),
                                                         "/home/user/"
                                                         ".ssh/id_rsa.pub",
                                                         "/home/user/"
                                                         ".ssh/id_rsa",
                                                         PW.toLocal8Bit().data())) == LIBSSH2_ERROR_EAGAIN);
        if(rc) {
//            fprintf(stderr, "\tAuthentication by public key failed\n");
            qDebug()  << "Authentication by password failed." ;
            goto shutdown;
        }
    }

    // chanel open
    while((channel = libssh2_channel_open_session(Session)) == NULL && libssh2_session_last_error(Session, NULL, NULL, 0) == LIBSSH2_ERROR_EAGAIN) {
//        waitsocket(sock, session);
//        qDebug()  << "waitsocket." ;

    }

    if(channel == NULL) {
        fprintf(stderr, "Error\n");
        exit(1);
    }

    qDebug() << "check Point2";

//    const char* commandline = "killall smcud";
    while((rc = libssh2_channel_exec(channel, commandLine)) == LIBSSH2_ERROR_EAGAIN) {
//        waitsocket(sock, session);
    }

    if(rc != 0) {
        fprintf(stderr, "Error\n");
        exit(1);
    }

    exitcode = 127;
    while((rc = libssh2_channel_close(channel)) == LIBSSH2_ERROR_EAGAIN);

//        waitsocket(TcpSocket.socketDescriptor(), Session);

    if(rc == 0) {
        exitcode = libssh2_channel_get_exit_status(channel);

        libssh2_channel_get_exit_signal(channel, &exitsignal, NULL, NULL, NULL, NULL, NULL);
    }

    if(exitsignal) qDebug() << "Got signal: " <<  exitsignal;
    else qDebug() << "Exit : " << exitcode;

    libssh2_channel_free(channel);

    channel = NULL;



 shutdown:
    libssh2_session_disconnect(Session, "Normal Shutdown, Thank you for playing");
    libssh2_session_free(Session);
    libssh2_exit();
    return;
}
