#pragma once

#include <QString>

namespace FileSys
{
bool readTextFile(const QString &filePath, QString &text, QString &errorMessage);
bool writeTextFile(const QString &filePath, const QString &text, QString &errorMessage);
}
