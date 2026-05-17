#pragma once

#include "../view/Mesh.h"
#include <vector>
#include <cmath>

/// Generates a UV-sphere mesh at unit radius, centered at the origin.
class SphereGeometry
{
public:
    static constexpr int kDefaultStacks = 24;
    static constexpr int kDefaultSlices = 24;

    [[nodiscard]] static std::vector<Vertex> getVertices(int stacks = kDefaultStacks, int slices = kDefaultSlices);
    [[nodiscard]] static std::vector<unsigned int> getIndices(int stacks = kDefaultStacks, int slices = kDefaultSlices);

private:
    static constexpr float kPi = 3.14159265358979f;
};
