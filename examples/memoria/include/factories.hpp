#ifndef FACTORIES_HPP
#define FACTORIES_HPP

#include "assets_manager.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "object.hpp"

// Factory Functions
std::shared_ptr<MyMesh> createMeshFromModel(AssetsManager &assetsManager, const std::string &modelName);
std::shared_ptr<MyObject> createObject(const std::shared_ptr<MyMesh> &mesh, const Material &material,
                                       const std::shared_ptr<MyShader> &shader, 
                                       const std::shared_ptr<MyShader> &shaderBox,
                                       bool isTransparent = false,
                                       GLuint textureId = 0, int drawType = GL_TRIANGLES                                       );
void loadAndRegisterShader(AssetsManager &assetsManager, const std::string &name);

#endif // FACTORIES_HPP