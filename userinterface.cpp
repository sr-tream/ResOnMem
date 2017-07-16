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

void UserInterface::on_selectFile_clicked()
{
    QFileInfo file = QFileDialog::getOpenFileName(this, "Select file for resource", QDir::currentPath(), "*.*");

    if (!file.isFile())
        return;

    filePath->setText(file.filePath());
    if (resName->text().isEmpty())
        resName->setText(file.fileName());
}

void UserInterface::on_resName_textChanged(const QString &arg1)
{
    QString name = arg1;
    for(QChar &ch: name){
        if (ch.isPunct() && ch != '_')
            ch = '_';
    }
    resName->setText(name);
}
