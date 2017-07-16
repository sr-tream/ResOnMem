#include "userinterface.h"

UserInterface::UserInterface(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
}

void UserInterface::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
