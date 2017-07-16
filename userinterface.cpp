#include "userinterface.h"

UserInterface::UserInterface(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    settings = new QSettings("Prime-Hack", "ResOnMem", this);
    inc_gNS->setChecked(settings->value("UseGlobalNamespace").toBool());
    g_resName->setText(settings->value("GlobalNamespace").toString());
    if (g_resName->text().isEmpty())
        g_resName->setText("ResOnMem");
}

void UserInterface::closeEvent(QCloseEvent *e)
{
    //settings->sync();
}

void UserInterface::on_g_resName_textEdited(const QString &arg1)
{
    if (!arg1.isEmpty())
        settings->setValue("GlobalNamespace", arg1);
}

void UserInterface::on_inc_gNS_toggled(bool checked)
{
    settings->setValue("UseGlobalNamespace", checked);
}
