#ifndef THREADSTART_H
#define THREADSTART_H

#include "incl.h"

#define  CMD_FILE_TRANSFER  1
#define  CMD_CMD_TRANSFER   2

class ThreadStart : public QThread
{
    Q_OBJECT
public:
    ThreadStart(int type, int cmdCnt, QString localPath, QString remotePath, QString cmd);
    ~ThreadStart() override;

protected:
    void run() override;

private:
    int type;
    int cmdCnt;
    QString localPath;
    QString remotePath;
    QString cmd;

};

#endif // THREADSTART_H
