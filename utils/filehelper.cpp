#include "filehelper.h"
#include <qdir.h>


QString FileHelper::readFile(const std::filesystem::path& path)
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

void FileHelper::saveStringToFile(const QString& contents, const std::filesystem::path& path)
{
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "File couldn't open!" << path.generic_string() << Qt::endl;
        throw std::runtime_error("File couldn't open: " + path.generic_string());
    }

    QTextStream out(&file);
    out << contents;
}

QString FileHelper::getFileNameFromPath(const std::filesystem::path& path)
{
    auto pathStr = path.string();
    std::string base_filename = pathStr.substr(pathStr.find_last_of("/\\") + 1);
    std::string::size_type const p(base_filename.find_last_of('.'));
    std::string file_without_extension = base_filename.substr(0, p);

    return QString::fromStdString(file_without_extension);
}
