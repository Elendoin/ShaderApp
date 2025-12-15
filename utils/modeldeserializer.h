#ifndef MODELDESERIALIZER_H
#define MODELDESERIALIZER_H
#include <filesystem>
#include "../models/shadermodel.h"
namespace fs = std::filesystem;

#define ICON_SIZE 64

class ModelDeserializer
{
public:
    static ShaderModel deserializeShaderModel(const fs::path& path, const QImage& defaultIcon);
};

#endif // MODELDESERIALIZER_H
