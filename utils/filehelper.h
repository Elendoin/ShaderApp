#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <QString>
#include <filesystem>

class FileHelper
{
public:
    static QString readFile(const std::filesystem::path& path);
    static void saveStringToFile(const QString& contents, const std::filesystem::path& path);
    static QString getFileNameFromPath(const std::filesystem::path& path);
};


#endif // FILEHELPER_H
