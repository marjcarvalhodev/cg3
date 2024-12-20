#ifndef UTILS_HPP
#define UTILS_HPP

#include <glm/glm.hpp>
#include <iostream>
#include <string>

inline void printErr(const std::exception &e) {
  std::cerr << "Error: " << e.what() << std::endl;
}

// Variadic template function for printing multiple arguments
template <typename T, typename... Args>
inline void print(const T &first, const Args &...rest) {
  std::cout << first;
  ((std::cout << " " << rest), ...); // Fold expression to print all arguments
  std::cout << std::endl;
}

inline glm::vec3 interpolate(const glm::vec3 &start, const glm::vec3 &end,
                             float alpha) {
  alpha = glm::clamp(alpha, 0.0f, 1.0f); // Ensure alpha is clamped to [0, 1]
  return start + alpha * (end - start);
}

inline bool rayIntersectsAABB(const glm::vec3 &rayOrigin,
                              const glm::vec3 &rayDir, const glm::vec3 &min,
                              const glm::vec3 &max) {
  float tmin = (min.x - rayOrigin.x) / rayDir.x;
  float tmax = (max.x - rayOrigin.x) / rayDir.x;
  if (tmin > tmax)
    std::swap(tmin, tmax);

  float tymin = (min.y - rayOrigin.y) / rayDir.y;
  float tymax = (max.y - rayOrigin.y) / rayDir.y;
  if (tymin > tymax)
    std::swap(tymin, tymax);

  if ((tmin > tymax) || (tymin > tmax))
    return false;
  if (tymin > tmin)
    tmin = tymin;
  if (tymax < tmax)
    tmax = tymax;

  float tzmin = (min.z - rayOrigin.z) / rayDir.z;
  float tzmax = (max.z - rayOrigin.z) / rayDir.z;
  if (tzmin > tzmax)
    std::swap(tzmin, tzmax);

  return (tmin <= tzmax) && (tzmin <= tmax);
}

inline glm::vec3 calculateRayDirection(int mouseX, int mouseY, int windowWidth, int windowHeight,
 const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) {
    // Convert screen coordinates to normalized device coordinates (NDC)
    float ndcX = (2.0f * mouseX) / windowWidth - 1.0f;
    float ndcY = 1.0f - (2.0f * mouseY) / windowHeight; // Invert Y for NDC
    glm::vec4 clipCoords(ndcX, ndcY, -1.0f, 1.0f);

    // Convert to camera/view space
    glm::vec4 eyeCoords = glm::inverse(projectionMatrix) * clipCoords;
    eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

    // Convert to world space
    glm::vec3 rayWorld = glm::vec3(glm::inverse(viewMatrix) * eyeCoords);
    rayWorld = glm::normalize(rayWorld);

    return rayWorld;
}

// inline glm::vec3 calculateRayDirection(int mouseX, int mouseY,
//                                        const glm::mat4 &viewMatrix,
//                                        const glm::mat4 &projectionMatrix,
//                                        int windowWidth, int windowHeight) {
//   // Convert screen space to normalized device coordinates (NDC)
//   float x = (2.0f * mouseX) / windowWidth - 1.0f;
//   float y = 1.0f - (2.0f * mouseY) / windowHeight; // Invert Y-axis
//   float z = 1.0f;                                  // Forward direction in NDC

//   glm::vec4 rayNDC(x, y, z, 1.0f);

//   // Convert NDC to clip space (view-projection inverse)
//   glm::mat4 invVP = glm::inverse(projectionMatrix * viewMatrix);
//   glm::vec4 rayClip = invVP * rayNDC;

//   // Convert to world space
//   glm::vec3 rayWorld = glm::normalize(glm::vec3(rayClip));
//   return rayWorld;
// }

#endif