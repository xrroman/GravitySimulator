#pragma once

#include <glm/glm.hpp>
#include <string>

/// Represents a single gravitational body (physics state + rendering hints).
struct Body
{
    std::string name;
    float mass; ///< Scaled mass (G = 1 unit system)
    float radius; ///< Visual radius in world units
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 color;
    bool isStatic; ///< If true, acts as an immovable anchor (e.g. Sun)

    Body(std::string name,
         float mass,
         float radius,
         glm::vec3 position,
         glm::vec3 velocity,
         glm::vec3 color,
         bool isStatic = false)
        : name(std::move(name))
        , mass(mass)
        , radius(radius)
        , position(position)
        , velocity(velocity)
        , acceleration(0.0f)
        , color(color)
        , isStatic(isStatic)
    {}
};
