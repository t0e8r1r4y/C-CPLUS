#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include "incl.h"

#define LOGIN_ID        "admin"
#define LOGIN_PW        "smarttolling1!"

#define ROOT_PATH       "/app"
#define MCU_SD_PATH     "/mnt/mmc"
#define MCU_MSD_PATH    "/mnt/save"

#define VDU_SD_PATH     "/media/sd"
#define VDU_MSD_PATH    "/media/msd"

#define CON_ID          "root"
#define CON_PW          1111
#define CON_PORT        22

#define MCU_A_IP        "211.221.247.101"
#define MCU_B_IP        "211.221.247.102"
#define VDU_A_IP        "211.221.247.121"
#define VDU_B_IP        "211.221.247.122"
#define IEU_IP          "211.221.247.151"


#define SEL_MCU_A       1
#define SEL_MCU_B       2
#define SEL_VDU_A       3
#define SEL_VDU_B       4
#define SEL_IEU         5

namespace Ui {
class TestDialog;
}

class TestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestDialog(QWidget *parent = nullptr);
    ~TestDialog();

private slots:
    void pushButtonLogin( void );
    void pushButtonSelect( void );
    void pushButtonStartTransfer( void );
    void pushButtonStartCmd( void );
    void selectComboBoard(int index );
    void selectComboCmd(int index );
    void selectComboSendPath(int index );
    void selectLocalPath( void );

private:
    Ui::TestDialog *ui;

private:
    QString     selectIP;
    QString     selectID;
    int         selectPort;
    int         selectPW;
    int         boardType;
    QString     transerPath;
    QString     fileSelection;
    QString     cmdSelection;

    bool        loginFlag;

    QStringList boardlist    {"MCU A", "MCU B", "VDU A", "VDU B", "IEU"};
    QStringList sendloc      {"root", "sd", "msd"};
    QStringList cmdlist      {"killall smcud", "killall hostd", "killall jsond", "uptime", "df"};

};

#endif // TESTDIALOG_H
