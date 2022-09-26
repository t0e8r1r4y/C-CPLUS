#ifndef INCL_H
#define INCL_H

// Qt header
#include <QWidget>
#include <QTcpSocket>
#include <QFile>
#include <QString>
#include <QProgressBar>
#include <QCoreApplication>
#include <QPixmap>
#include <QThread>
#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

// libssh2 header -> path is ./lib/...
#include "lib/libssh2.h"
#include "lib/libssh2_config.h"
#include "lib/libssh2_publickey.h"
#include "lib/libssh2_sftp.h"

// c/c++ header ->
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>

// user define header about function
#include "sshproc.h"
#include "threadStart.h"
#include "uihelper.h"
#include "filectl.h"
#include "defremote.h"

// user define header about ui dialog
#include "testdialog.h"


#endif // INCL_H
