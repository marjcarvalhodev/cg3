#include "factories.hpp"

void loadAndRegisterShader(AssetsManager &assetsManager,
                           const std::string &name) {

  std::string shaderName = name + "Shader";
  assetsManager.loadShader(shaderName, name);
}

std::shared_ptr<MyMesh> createMeshFromModel(AssetsManager &assetsManager,
                                            const std::string &modelName) {
  assetsManager.loadModel(modelName + "Model", modelName);
  return assetsManager.getModel(modelName + "Model");
}

std::shared_ptr<MyObject>
createObject(const std::shared_ptr<MyMesh> &mesh, const Material &material,
             const std::shared_ptr<MyShader> &shader,
             const std::shared_ptr<MyShader> &shaderBox, bool isTransparent,
             GLuint textureId, int drawType) {
  return std::make_shared<MyObject>(mesh, material, shader, shaderBox, isTransparent,
                                    textureId, drawType);
}