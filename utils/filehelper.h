#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <QString>
#include <filesystem>

class FileHelper
{
public:
    static QString read(const std::filesystem::path& path);
    static std::vector<QString> readLines(const std::filesystem::path& path);
    static void saveString(const QString& contents, const std::filesystem::path& path);
    static void saveLines(const std::vector<QString> lines, const std::filesystem::path& path);
    static QString getFileNameFromPath(const std::filesystem::path& path);
    static QString findIncrementFileName(const std::filesystem::path& path);
};


#endif // FILEHELPER_H
