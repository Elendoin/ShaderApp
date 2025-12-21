#ifndef MODELDESERIALIZATIONH_H
#define MODELDESERIALIZATIONH_H
#include <filesystem>
#include "../models/shadermodel.h"
namespace fs = std::filesystem;

#define ICON_SIZE 64

class ModelSerialization
{
public:
    static void serializeShaderModel(ShaderModel model);
    static ShaderModel deserializeShaderModel(const fs::path& path, const QImage& defaultIcon);
};

#endif // MODELDESERIALIZATIONH_H
