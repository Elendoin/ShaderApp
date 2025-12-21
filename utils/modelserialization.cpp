#include "modelserialization.h"
#include "filehelper.h"
#include <QDebug>
namespace fs = std::filesystem;

void ModelSerialization::serializeShaderModel(ShaderModel model)
{
    if(!fs::exists(model.getPath()))
    {
        throw std::runtime_error("No path found in model: " + model.getName().toStdString());
    }

    auto sourcePath = fs::exists(model.getSourcePath()) ? model.getSourcePath().filename() : model.getPath() / "source.txt";
    FileHelper::saveString(model.getFragmentShaderSource(), sourcePath);

    if(!model.getDocumentation().isEmpty())
    {
        auto documentationPath = fs::exists(model.getDocumentationPath()) ? model.getDocumentationPath().filename() : model.getPath() / "documentation.md";
        FileHelper::saveString(model.getDocumentation(), documentationPath);
    }
    if(!model.getIcon().isNull())
    {
        //TODO
    }
}

ShaderModel ModelSerialization::deserializeShaderModel(const fs::path& path, const QImage& defaultIcon)
{
    ShaderModel model;
    model.setName(FileHelper::getFileNameFromPath(path));
    model.setPath(path);
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
            model.setDocumentationPath(path);
            model.setDocumentation(FileHelper::read(path));
        }
        else if(!(path.string().find("source") == std::string::npos))
        {
            model.setSourcePath(path);
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
