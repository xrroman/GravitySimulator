#include "SphereGeometry.h"

std::vector<Vertex> SphereGeometry::getVertices(int stacks, int slices)
{
    std::vector<Vertex> vertices;
    vertices.reserve(static_cast<std::size_t>((stacks + 1) * (slices + 1)));

    for (int i = 0; i <= stacks; ++i)
    {
        const float phi = kPi * static_cast<float>(i) / static_cast<float>(stacks);

        for (int j = 0; j <= slices; ++j)
        {
            const float theta = 2.0f * kPi * static_cast<float>(j) / static_cast<float>(slices);

            Vertex v;
            v.position = {
                std::sin(phi) * std::cos(theta),
                std::cos(phi),
                std::sin(phi) * std::sin(theta)
            };
            v.texCoords = {
                static_cast<float>(j) / static_cast<float>(slices),
                static_cast<float>(i) / static_cast<float>(stacks)
            };
            vertices.push_back(v);
        }
    }
    return vertices;
}

std::vector<unsigned int> SphereGeometry::getIndices(int stacks, int slices)
{
    std::vector<unsigned int> indices;
    indices.reserve(static_cast<std::size_t>(stacks * slices * 6));

    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < slices; ++j)
        {
            const unsigned int row = static_cast<unsigned int>(i * (slices + 1));
            const unsigned int next = row + static_cast<unsigned int>(slices + 1);

            indices.push_back(row + static_cast<unsigned int>(j));
            indices.push_back(next + static_cast<unsigned int>(j));
            indices.push_back(row + static_cast<unsigned int>(j) + 1u);

            indices.push_back(row + static_cast<unsigned int>(j) + 1u);
            indices.push_back(next + static_cast<unsigned int>(j));
            indices.push_back(next + static_cast<unsigned int>(j) + 1u);
        }
    }
    return indices;
}
