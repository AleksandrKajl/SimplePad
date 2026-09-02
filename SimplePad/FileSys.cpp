#include "FileSys.h"

#include <QFile>
#include <QTextStream>

bool FileSys::readTextFile(const QString &filePath, QString &text, QString &errorMessage)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        errorMessage = file.errorString();
        return false;
    }

    QTextStream stream(&file);
    text = stream.readAll();
    if (stream.status() != QTextStream::Ok)
    {
        errorMessage = file.errorString();
        return false;
    }

    errorMessage.clear();
    return true;
}

bool FileSys::writeTextFile(const QString &filePath, const QString &text, QString &errorMessage)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        errorMessage = file.errorString();
        return false;
    }

    QTextStream stream(&file);
    stream << text;
    stream.flush();
    if (stream.status() != QTextStream::Ok)
    {
        errorMessage = file.errorString();
        return false;
    }

    errorMessage.clear();
    return true;
}
