#ifndef UIHELPER_H
#define UIHELPER_H

#include "incl.h"

class Uihelper
{
public:
    Uihelper();
    void setComboBox( QComboBox *combo, QStringList text);
    void clearComboBox( QComboBox *combo );
    void lockButton( QPushButton *b, bool flag );
    QString getLineEditText( QLineEdit *e );
    void setLineEditText( QLineEdit *e, QString s );
    QString getButtonText( QPushButton *b );
    void setButtonText( QPushButton *b, QString s );

private:


};

#endif // UIHELPER_H
