#include "codegenerator.h"
#include <QDebug>

CodeGenerator::CodeGenerator(QString filePath, bool useSpaces)
{
	this->filePath = filePath;
	this->useSpaces = useSpaces;

	file = new QFile(filePath);
	file->open(QIODevice::ReadOnly);
	if (!file->isOpen())
		throw "Can not open file";
	if (!file->isReadable())
		throw "Can not read file";
	fileArray.clear();
	code.clear();
	fileArray = file->readAll();
}

QStringList CodeGenerator::generateCode(QString NS)
{
	code.clear();
	auto NSList = NS.split(':');
	NS.replace(':', '_');
	writeString("#ifndef " + NS + "_H");
	writeString("#define " + NS + "_H");
	writeString("#include <fstream>");
	for (int i = 0; i < NSList.count(); ++i){
		writeString("namespace " + NSList[i] + "{");
		setOffset(i + 1);
	}

	writeString("const unsigned char data[] = {");
	setOffset(offset() + 1);
	QString dataLine;
	int i = 0, c = 0;
	for(auto ch: fileArray){
		QString tmp;
		if (i < octets){
			if (c == fileArray.size() - 1)
				tmp.sprintf("0x%02X", (unsigned char)ch);
			else tmp.sprintf("0x%02X, ", (unsigned char)ch);
			dataLine += tmp;
		} else {
			writeString(dataLine);
			dataLine.clear();
			tmp.sprintf("0x%02X, ", (unsigned char)ch);
			dataLine += tmp;
			i = 0;
		}
		++i;
		++c;
	}
	if (!dataLine.isEmpty())
		writeString(dataLine);
	setOffset(offset() - 1);
	writeString("}; // data");
	writeString("const unsigned long long size = " + QString::number(fileArray.size()) + "; // bytes");

	writeString("bool save(const char* fileName){");
	setOffset(offset() + 1);
	writeString("std::ofstream f(fileName, std::ios::binary);");
	writeString("if (!f.is_open())");
	setOffset(offset() + 1);
	writeString("return false;");
	setOffset(offset() - 1);
	writeString("for (unsigned long long i = 0; i < size; ++i)");
	setOffset(offset() + 1);
	writeString("f << data[i];");
	setOffset(offset() - 1);
	writeString("f.close();");
	writeString("return true;");
	setOffset(offset() - 1);
	writeString("}");

	for (int i = NSList.count(); i > 0; --i){
		setOffset(i - 1);
		writeString("} // " + NSList[i - 1]);
	}
	writeString("#endif // " + NS + "_H");

	return code;
}

void CodeGenerator::setUseSpaces(bool state)
{
	useSpaces = state;
}

bool CodeGenerator::isUseSpaces()
{
	return useSpaces;
}

void CodeGenerator::setSpacesInTab(int count)
{
	spacesInOneTab = count;
}

int CodeGenerator::spacesInTab()
{
	return spacesInOneTab;
}

void CodeGenerator::setCols(int count)
{
	octets = count;
}

int CodeGenerator::cols()
{
	return octets;
}

void CodeGenerator::setOffset(uint tabs)
{
	codeOffset = tabs;
}

uint CodeGenerator::offset()
{
	return codeOffset;
}

void CodeGenerator::writeString(QString str)
{
	for (uint i = 0; i < codeOffset; ++i)
		if (useSpaces){
			for (int j = 0; j < spacesInOneTab; ++j)
				str.insert(0, " ");
		} else
			str.insert(0, "\t");
	str.push_back('\n');
	code << str;
}
