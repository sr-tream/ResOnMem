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

    if (cgen != nullptr)
        delete cgen;
    cgen = new CodeGenerator(file.filePath());
}

void UserInterface::on_resName_textChanged(const QString &arg1)
{
    if (arg1.isEmpty())
        generate->setEnabled(false);
    else generate->setEnabled(true);
    QString name = arg1;
    removePunct(name);
    if (name != arg1)
        resName->setText(name);
}

void UserInterface::on_g_resName_textChanged(const QString &arg1)
{
    QString name = arg1;
    removePunct(name);
    if (name != arg1)
        g_resName->setText(name);
    if (!name.isEmpty())
        settings->setValue("GlobalNamespace", name);
}

void UserInterface::removePunct(QString &str)
{
    for(QChar &ch: str){
        if (ch.isPunct() && ch != '_')
            ch = '_';
    }
}

void UserInterface::on_generate_clicked()
{
    if (cgen == nullptr)
        return;

    QStringList code;
    if (inc_gNS->isChecked())
        code = cgen->generateCode(resName->text(), g_resName->text());
    else code = cgen->generateCode(resName->text(), "");
    QString fileName = QFileDialog::getSaveFileName(this, "Generate resource file", QDir::currentPath(), "*.hpp");

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    if (!file.isOpen())
        throw "Can not open file";
    if (!file.isWritable())
        throw "Can not write file";
    for (QString str: code)
        file.write(str.toStdString().c_str());
    file.close();
}
