#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include <QFile>
#include <QStringList>

class CodeGenerator
{
public:
    CodeGenerator(QString filePath, bool useSpaces = 1);

    QStringList generateCode(QString NS, QString gNS);

    void setUseSpaces(bool state);
    bool isUseSpaces();
    void setSpacesInTab(int count);
    int spacesInTab();
    void setCols(int count);
    int cols();

protected:
    void setOffset(uint tabs);
    uint offset();

    void writeString(QString);

private:
    QString filePath;
    QFile *file = nullptr;
    QByteArray fileArray;
    bool useSpaces;
    uint codeOffset = 0;
    int spacesInOneTab = 4;
    int octets = 12;
    QStringList code;
};

#endif // CODEGENERATOR_H
