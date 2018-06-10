#include "userinterface.h"

UserInterface::UserInterface(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);
	settings = new QSettings("Prime-Hack", "ResOnMem", this);
	useSpaces->setChecked(settings->value("UseSoftSpaces").toBool());
	spacesCount->setValue(settings->value("SpacesCount").toInt());
	columnsCount->setValue(settings->value("ColumnsCount").toInt());
}

void UserInterface::on_selectFile_clicked()
{
	QFileInfo file = QFileDialog::getOpenFileName(this, "Select file for resource", QDir::currentPath(), "*.*");

	if (!file.isFile())
		return;

	filePath->setText(file.filePath());
	if (resName->text().isEmpty())
		resName->setText("resources:" + file.fileName());

	if (cgen != nullptr)
		delete cgen;
	cgen = new CodeGenerator(file.filePath());
	cgen->setUseSpaces(useSpaces->isChecked());
	cgen->setSpacesInTab(spacesCount->value());
	cgen->setCols(columnsCount->value());
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

void UserInterface::removePunct(QString &str)
{
	for(QChar &ch: str)
		if (ch.isPunct() && ch != '_' && ch != ':')
			ch = '_';
}

void UserInterface::on_generate_clicked()
{
	if (cgen == nullptr)
		return;

	QStringList code;
	code = cgen->generateCode(resName->text());
	QFileInfo fileName = QFileDialog::getSaveFileName(this, "Generate resource file", QDir::currentPath(), "*.hpp");

	if (fileName.suffix() != "hpp")
		fileName = fileName.filePath() + ".hpp";

	QFile file(fileName.filePath());
	if (file.exists())
		file.remove();
	file.open(QIODevice::WriteOnly);
	if (!file.isOpen())
		throw "Can not open file";
	if (!file.isWritable())
		throw "Can not write file";
	for (QString str: code)
		file.write(str.toStdString().c_str());
	file.close();
}

void UserInterface::on_useSpaces_toggled(bool checked)
{
	settings->setValue("UseSoftSpaces", checked);
	if (!cgen)
		return;
	cgen->setUseSpaces(checked);
}

void UserInterface::on_spacesCount_valueChanged(int arg1)
{
	settings->setValue("SpacesCount", arg1);
	if (!cgen)
		return;
	cgen->setSpacesInTab(arg1);
}

void UserInterface::on_columnsCount_valueChanged(int arg1)
{
	settings->setValue("ColumnsCount", arg1);
	if (!cgen)
		return;
	cgen->setCols(arg1);
}
