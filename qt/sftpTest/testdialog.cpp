#include "incl.h"
#include "ui_testdialog.h"


/**
 * @brief fileTransferTest
 * [CHECK] I don't know the reason why "ThreadStart" class isn't impossible to declare in header file.
 */
ThreadStart *fileTransferTest;
Uihelper    *testUi;


/** ================================================================
 * @brief TestDialog::TestDialog
 * @param parent
 ================================================================ */
TestDialog::TestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestDialog)
{
    ui->setupUi(this);

    testUi    =  new Uihelper();
    loginFlag = false;

    testUi->lockButton(ui->select, true);
    testUi->lockButton(ui->startTransfer, true);
    testUi->lockButton(ui->startCmd, true);

    /**
     * @brief connect function declare
     */
    connect(ui->Login, SIGNAL(pressed()), this, SLOT(pushButtonLogin()));
    connect(ui->select, SIGNAL(pressed()), this, SLOT(pushButtonSelect()));
    connect(ui->startTransfer, SIGNAL(pressed()), this, SLOT(pushButtonStartTransfer()));
    connect(ui->startCmd, SIGNAL(pressed()), this, SLOT(pushButtonStartCmd()));

    connect(ui->toolButton, SIGNAL(pressed()), this, SLOT(selectLocalPath()));

    connect(ui->comboBox_3, SIGNAL(activated(int)), this, SLOT(selectComboBoard(int)));
    connect(ui->comboBox, SIGNAL(activated(int)), this, SLOT(selectComboSendPath(int)));
    connect(ui->comboBox_2, SIGNAL(activated(int)), this, SLOT(selectComboCmd(int)));


}



/** ================================================================
 * @brief TestDialog::~TestDialog
 ================================================================ */
TestDialog::~TestDialog()
{
    delete ui;
}


/** ================================================================
 * @brief [SLOTS] TestDialog::pushButtonLogin -> OK
 * When "User" wnats to test "Ssh function" in libssh2 library,
 ================================================================ */
void
TestDialog::pushButtonLogin( void )
{

    /**
     * @brief Local Values
     *
     * "buttonText" is Login Button Text. On default, this is "Login". when ID & PW is valid and pressed button, this will be changed "Logout"
     * "id" is string from QLineEdit ID. This edit is identifier for activating this dialog.
     * "pw" is smae explaination above.
     */
    QString buttonText  =   testUi->getButtonText(ui->Login);
    QString id          =   testUi->getLineEditText( ui->lineEdit_3 );
    QString pw          =   testUi->getLineEditText( ui->lineEdit_4 );

    if( !id.isEmpty() && !pw.isEmpty() )
    {
        if( id.contains(LOGIN_ID, Qt::CaseInsensitive) && pw.contains(LOGIN_PW, Qt::CaseInsensitive) )
        {
            loginFlag = true;
        }
    }


    // when loginFlag is ture, dialog activated.
    if( loginFlag == false )
    {
        goto CLEAREXIT;
    }
    else // if( loginFlag == true )
    {
        if( buttonText.contains("login", Qt::CaseInsensitive) )
        {
            testUi->setComboBox(ui->comboBox_3, boardlist);
            testUi->lockButton(ui->select, false);
            testUi->setButtonText(ui->Login, "logout");
            goto EXIT;
        }
        else if( buttonText.contains("logout", Qt::CaseInsensitive) )
        {
            testUi->setButtonText(ui->Login, "login");
            goto CLEAREXIT;
        }
    }

CLEAREXIT:
    // All buttons are locked.
    testUi->lockButton(ui->select,          true);
    testUi->lockButton(ui->startTransfer,   true);
    testUi->lockButton(ui->startCmd,        true);
    testUi->setButtonText(ui->select,       "select");
    testUi->setButtonText(ui->startTransfer,"start");
    testUi->setButtonText(ui->startCmd,     "start");

    // All comboBoxes are clear.
    testUi->clearComboBox(ui->comboBox_3);
    testUi->clearComboBox(ui->comboBox_2);
    testUi->clearComboBox(ui->comboBox);

EXIT:
    return;
}



/** ================================================================
 * @brief [SLOTS] TestDialog::pushButtonSelect  -> OK
 * selection board to connection
 ================================================================ */
void
TestDialog::pushButtonSelect( void )
{
    // only this function works on "loginFlag" true.
    if( loginFlag == false ) return;

    QString buttonText  =   testUi->getButtonText(ui->select);
    QString comboText   =   ui->comboBox_3->currentText();

    if( buttonText.contains("select",  Qt::CaseInsensitive) && !comboText.contains("select") )
    {
        testUi->setButtonText(ui->select,       "cancel");
        testUi->setComboBox(ui->comboBox,       sendloc);
        testUi->setComboBox(ui->comboBox_2,     cmdlist);
        testUi->lockButton(ui->startTransfer,   false);
        testUi->lockButton(ui->startCmd,        false);
        goto EXIT;
    }
    else if( buttonText.contains("cancel",  Qt::CaseInsensitive) || comboText.contains("select") )
    {
        testUi->setButtonText(ui->select, "select");
        goto CLEAREXIT;
    }

CLEAREXIT:
    testUi->clearComboBox(ui->comboBox);
    testUi->clearComboBox(ui->comboBox_2);
    testUi->setLineEditText(ui->lineEdit,   "0");
    testUi->setLineEditText(ui->lineEdit_2, "0");
    testUi->lockButton(ui->startTransfer,   true);
    testUi->lockButton(ui->startCmd,        true);

EXIT:
    return;
}


/** ================================================================
 * @brief [SLOTS] TestDialog::pushButtonStartTransfer
 ================================================================ */
void
TestDialog::pushButtonStartTransfer( void )
{
    // thread start
    QString buttonText = ui->startTransfer->text();

    if( buttonText.contains( "start") )
    {
        /**
         *  "ThreadStart args" means below
         *  first   : command type defined in threadstart.h
         *  second  : count to test. This value is form's input.
         *  third   : local path including file name.
         *  forth   : remote path including file name.
         *  fifth   : shell cmd
         */
        fileTransferTest = new ThreadStart(CMD_FILE_TRANSFER,
                                           testUi->getLineEditText(ui->lineEdit).toInt(),
                                           fileSelection,
                                           transerPath,
                                           "");
        fileTransferTest->start();
        testUi->setButtonText(ui->startTransfer, "end");

    }
    else if( buttonText.contains("end") )
    {
        fileTransferTest->requestInterruption();
        testUi->setButtonText(ui->startTransfer, "start");
        delete fileTransferTest;
    }

    return;
}


/** ================================================================
 * @brief [SLOTS] TestDialog::pushButtonStartCmd
 ================================================================ */
void
TestDialog::pushButtonStartCmd( void )
{
    // thread start
    QString buttonText = testUi->getButtonText(ui->startCmd);

    if( buttonText.contains( "start") )
    {
        /**
         *  "ThreadStart args" means below
         *  first   : command type defined in threadstart.h
         *  second  : count to test. This value is form's input.
         *  third   : local path including file name.
         *  forth   : remote path including file name.
         *  fifth   : shell cmd
         */
        fileTransferTest = new ThreadStart(CMD_CMD_TRANSFER,
                                           testUi->getLineEditText(ui->lineEdit_2).toInt(),
                                           "",
                                           "",
                                           cmdSelection);
        fileTransferTest->start();
        testUi->setButtonText(ui->startTransfer, "end");

    }
    else if( buttonText.contains("end") )
    {
        fileTransferTest->requestInterruption();
        testUi->setButtonText(ui->startTransfer, "start");
        delete fileTransferTest;
    }

    return;
}


/** ================================================================
 * @brief [SLOT] TestDialog::selectComboBoard -> OK
 * @param index
 ================================================================ */
void
TestDialog::selectComboBoard(int index )
{
    switch (index) {
        case 1:
        {
            /* test biz logic*/
            break;
        }
        default:
        {
            boardType = -1;
            break;
        }
    }
    return;
}

/** ================================================================
 * @brief TestDialog::selectComboCmd
 * @param index
 ================================================================ */
void
TestDialog::selectComboCmd(int index )
{
    if(index == -1) return;

    cmdSelection = "";
    cmdSelection = cmdlist[index-1]; // index start 1, so -1 needed

    return;
}


/** ================================================================
 * @brief TestDialog::selectComboSendPath -> OK
 * @param index
 ================================================================ */
void
TestDialog::selectComboSendPath(int index )
{
    transerPath = "";

    switch (index) {
        case 1: // root
        {
            transerPath = ROOT_PATH;
            break;
        }
        case 2: // sd
        {
            /* test biz logic*/

            break;
        }
        case 3: // msd
        {
             /* test biz logic*/

            break;
        }
        default:
        {
            transerPath = "";
            break;
        }
    }
    return;
}



/** ================================================================
 * @brief TestDialog::selectLocalPath
 ================================================================ */
void
TestDialog::selectLocalPath( void )
{
    fileSelection = "";
    fileSelection = QFileDialog::getOpenFileName(this,"SelectFile",QApplication::applicationDirPath(),"All Files(*.*)");
    return;
}
