#include "Simulation.h"

#include <glm/glm.hpp>
#include <cmath>
#include <utility>

void Simulation::addBody(Body body)
{
    m_bodies.push_back(std::move(body));
}

void Simulation::reset()
{
    m_bodies.clear();
}

void Simulation::update(float deltaTime)
{
    if (paused) return;

    const float scaledDt = deltaTime * timeScale;
    computeAccelerations();
    integrate(scaledDt);
}

// Brute-force O(n²) — suitable for solar-system scale (<20 bodies).
void Simulation::computeAccelerations()
{
    for (auto& body : m_bodies)
        body.acceleration = glm::vec3(0.0f);

    for (std::size_t i = 0; i < m_bodies.size(); ++i)
    {
        if (m_bodies[i].isStatic) continue;

        for (std::size_t j = 0; j < m_bodies.size(); ++j)
        {
            if (i == j) continue;

            const glm::vec3 delta = m_bodies[j].position - m_bodies[i].position;
            const float distSq = glm::dot(delta, delta) + kSoftening * kSoftening;
            const float dist = std::sqrt(distSq);

            const float forceMag = kG * m_bodies[j].mass / distSq;
            m_bodies[i].acceleration += forceMag * (delta / dist);
        }
    }
}

// Symplectic Euler: velocity updated before position for better energy conservation.
void Simulation::integrate(float deltaTime)
{
    for (auto& body : m_bodies)
    {
        if (body.isStatic) continue;

        body.velocity += body.acceleration * deltaTime;
        body.position += body.velocity * deltaTime;
    }
}
