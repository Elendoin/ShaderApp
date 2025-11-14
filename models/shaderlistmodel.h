#ifndef SHADERLISTMODEL_H
#define SHADERLISTMODEL_H

#include <models/shadermodel.h>

class ShaderListModel
{
public:
    std::vector<ShaderModel>& getList() {return m_list;}
private:
    std::vector<ShaderModel> m_list;
};

#endif // SHADERLISTMODEL_H
