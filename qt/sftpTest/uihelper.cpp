#include "incl.h"

Uihelper::Uihelper()
{

}


void
Uihelper::setComboBox( QComboBox *combo, QStringList text )
{
    combo->addItem("select");

    for(int i = 0; i < text.length(); i++)
    {
        combo->addItem(text.at(i));
    }

    return;
}


void
Uihelper::clearComboBox( QComboBox *combo )
{
    combo->clear();
    return;
}

void
Uihelper::lockButton( QPushButton *b, bool flag )
{
    if(flag == false)
    {
        b->setEnabled(true);
    }
    else if(flag == true)
    {
        b->setEnabled(false);
    }

    return;
}


QString
Uihelper::getLineEditText( QLineEdit *e )
{
    return e->text();
}


void
Uihelper::setLineEditText( QLineEdit *e, QString s )
{
    e->setText(s);
    return;
}




QString
Uihelper::getButtonText( QPushButton *b )
{
    return b->text();
}

void
Uihelper::setButtonText( QPushButton *b, QString s )
{
    b->setText(s);
    return;
}
