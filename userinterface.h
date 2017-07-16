#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "ui_userinterface.h"
#include <QSettings>

class UserInterface : public QWidget, private Ui::UserInterface
{
    Q_OBJECT

public:
    explicit UserInterface(QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *e) override;

private slots:
    void on_g_resName_textEdited(const QString &arg1);

    void on_inc_gNS_toggled(bool checked);

private:
    QSettings *settings;
};

#endif // USERINTERFACE_H
