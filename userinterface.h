#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "ui_userinterface.h"

class UserInterface : public QWidget, private Ui::UserInterface
{
    Q_OBJECT

public:
    explicit UserInterface(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);
};

#endif // USERINTERFACE_H
