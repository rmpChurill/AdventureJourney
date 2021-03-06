#include "DummyData.hpp"

namespace
{
gui::Vertex_XYZ_RGB vertices[8] = {
    {{-1, -1, -1}, {0, 0, 0}},    //0
    {{1, -1, -1}, {255, 0, 0}},   //1
    {{1, 1, -1}, {255, 255, 0}},  //2
    {{-1, 1, -1}, {0, 255, 0}},   //3
    {{-1, -1, 1}, {0, 0, 255}},   //4
    {{1, -1, 1}, {255, 0, 255}},  //5
    {{1, 1, 1}, {255, 255, 255}}, //6
    {{-1, 1, 1}, {0, 255, 255}},  //7
};

GLubyte indices[36] = {
    0, 2, 1,
    3, 2, 0,
    4, 5, 6,
    7, 4, 6,
    5, 1, 2,
    2, 6, 5,
    0, 4, 7,
    7, 3, 0,
    5, 4, 1,
    0, 1, 4,
    7, 6, 2,
    7, 2, 3};

glutil::MeshBuffer mesh;
bool meshReady = false;

void BuildMesh()
{
    mesh.LoadFromData(8, sizeof(gui::Vertex_XYZ_RGB), vertices,
                      36, GL_UNSIGNED_BYTE, indices,
                      {glutil::MeshAttribute(0, 3, GL_FLOAT, false, offsetof(gui::Vertex_XYZ_RGB, position)),
                       glutil::MeshAttribute(2, 3, GL_UNSIGNED_BYTE, true, offsetof(gui::Vertex_XYZ_RGB, color))},
                      GL_TRIANGLES);

    meshReady = true;
}
} // namespace

const glutil::MeshBuffer &gui::models::CubeMesh()
{
    if (!meshReady)
        BuildMesh();

    return mesh;
}