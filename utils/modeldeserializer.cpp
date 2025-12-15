#include "modeldeserializer.h"
#include "filehelper.h"
#include <QDebug>

ShaderModel ModelDeserializer::deserializeShaderModel(const fs::path& path, const QImage& defaultIcon)
{
    ShaderModel model;
    model.setName(FileHelper::getFileNameFromPath(path));
    bool sourceFound = false;
    bool iconFound = false;

    for(auto entry : fs::directory_iterator(path))
    {
        auto path = entry.path();
        if(entry.is_directory())
        {
            continue;
        }

        if(path.extension() == ".md")
        {
            model.setDocumentation(FileHelper::read(path));
        }
        else if(!(path.string().find("source") == std::string::npos))
        {
            model.setFragmentShaderSource(FileHelper::read(path));
            sourceFound = true;
        }
        else if(!(path.string().find("icon") == std::string::npos))
        {
            QImage image;
            if(image.load(QString::fromStdString(path.string())) && (image.width() == ICON_SIZE && image.height() == ICON_SIZE))
            {
                iconFound = true;
                model.setIcon(image);
            }
            else
            {
                qDebug() << "Couldn't load image from directory: " + path.string();
            }
        }
    }

    if(!iconFound)
    {
        model.setIcon(defaultIcon);
    }
    if(!sourceFound)
    {
        throw std::runtime_error("No source code in the shader directory: " + path.string());
    }

    return model;
}
