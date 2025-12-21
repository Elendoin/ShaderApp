#include "filehelper.h"
#include <qdir.h>


QString FileHelper::read(const std::filesystem::path& path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "File couldn't open!" << path.generic_string() << Qt::endl;
        throw std::runtime_error("File couldn't open: "  + path.generic_string());
    }

    QTextStream in(&file);
    return in.readAll();
}

std::vector<QString> FileHelper::readLines(const std::filesystem::path& path)
{
    std::vector<QString> lines;
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "File couldn't open!" << path.generic_string() << Qt::endl;
        throw std::runtime_error("File couldn't open: " + path.generic_string());
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        lines.push_back(line);
    }

    return lines;
}

void FileHelper::saveString(const QString& contents, const std::filesystem::path& path)
{
    QFile file(QString::fromStdString(path.string()));
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "File couldn't open!" << path.generic_string() << Qt::endl;
        throw std::runtime_error("File couldn't open: " + path.generic_string());
    }

    QTextStream out(&file);
    out << contents;
    out.flush();
}

void FileHelper::saveLines(const std::vector<QString> lines, const std::filesystem::path& path)
{
    QString contents;
    for(const auto& line : lines)
    {
        contents += line;
        contents += "\n";
    }
    saveString(contents, path);
}

QString FileHelper::getFileNameFromPath(const std::filesystem::path& path)
{
    auto pathStr = path.string();
    std::string base_filename = pathStr.substr(pathStr.find_last_of("/\\") + 1);
    auto p(base_filename.find_last_of('.'));
    std::string file_without_extension = base_filename.substr(0, p);

    return QString::fromStdString(file_without_extension);
}

QString FileHelper::findIncrementFileName(const std::filesystem::path& path)
{
    auto name = getFileNameFromPath(path);
}
