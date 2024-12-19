#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <glm/glm.hpp>

constexpr int DEFAULT_WIN_WIDTH = {800};
constexpr int DEFAULT_WIN_HEIGHT = {600};

constexpr glm::vec4 DEFAULT_CLEAR_COLOR = {0.3f, 0.5f, 0.4f, 1.0f};
constexpr glm::vec4 NIGHT_CLEAR_COLOR = {0.0f, 0.0f, 0.1f, 1.0f};
constexpr glm::vec4 SKY_CLEAR_COLOR = {0.6f, 0.8f, 1.0f, 1.0f};

const int TICKS_PER_SECOND = 30;
const float MS_PER_TICK = 1000.0f / TICKS_PER_SECOND;

#endif