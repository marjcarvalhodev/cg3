#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <glm/glm.hpp>

inline void printErr(const std::exception &e)
{
    std::cerr << "Error: " << e.what() << std::endl;
}

// Variadic template function for printing multiple arguments
template <typename T, typename... Args>
inline void print(const T &first, const Args &...rest)
{
    std::cout << first;
    ((std::cout << " " << rest), ...); // Fold expression to print all arguments
    std::cout << std::endl;
}

inline glm::vec3 interpolate(const glm::vec3 &start, const glm::vec3 &end, float alpha)
{
    alpha = glm::clamp(alpha, 0.0f, 1.0f); // Ensure alpha is clamped to [0, 1]
    return start + alpha * (end - start);
}

#endif