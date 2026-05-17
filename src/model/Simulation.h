#pragma once

#include "Body.h"
#include <vector>

/// N-Body gravitational simulation (G = 1 unit system).
/// distances: visual AU (1 AU = 10 world units)
/// time:      real seconds (~31.6 s = 1 simulated Earth year)
/// mass:      scaled so G * M_sun = 39.535
class Simulation
{
public:
    static constexpr float kG = 1.0f;
    static constexpr float kSoftening = 0.3f;  ///< Prevents singularity at r=0

    bool paused = false;
    float timeScale = 1.0f;  ///< 1x ≈ 1 Earth year per ~31.6 s real time

    void addBody(Body body);
    void update(float deltaTime);
    void reset();

    [[nodiscard]] const std::vector<Body>& getBodies() const { return m_bodies; }
    [[nodiscard]] std::vector<Body>& getBodies() { return m_bodies; }

private:
    std::vector<Body> m_bodies;

    void computeAccelerations();
    void integrate(float deltaTime);
};
