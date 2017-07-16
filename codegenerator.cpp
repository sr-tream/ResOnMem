#include "codegenerator.h"

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
    fileArray = file->readAll();
}

QStringList CodeGenerator::generateCode(QString NS, QString gNS)
{
    if (!gNS.isEmpty()){
        writeString("namespace " + gNS + "{");
        setOffset(1);
    }
    writeString("namespace " + NS + "{");
    setOffset(offset() + 1);

    writeString("const char data[] = {");
    setOffset(offset() + 1);
    QString dataLine;
    int i = 0, c = 0;
    for(auto ch: fileArray){
        if (i < octets){
            QString tmp;
            if (c == fileArray.size() - 1)
                tmp.sprintf("0x%02X", ch);
            else tmp.sprintf("0x%02X, ", ch);
            dataLine += tmp;
        } else {
            writeString(dataLine);
            dataLine.clear();
            i = -1;
        }
        ++i;
        ++c;
    }
    if (!dataLine.isEmpty())
        writeString(dataLine);
    setOffset(offset() - 1);
    writeString("}; // data");
    writeString("const unsigned long long size = " + QString::number(fileArray.size()) + "; // bytes");

    // TODO: generate save code

    setOffset(offset() - 1);
    writeString("} // " + NS);
    if (!gNS.isEmpty()){
        setOffset(0);
        writeString("} // " + gNS);
    }

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
