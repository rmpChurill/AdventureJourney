#include "DummyData.hpp"

gui::models::Vertex_XYZ_RGB gui::models::cubeData[8] = {
    {{-1, -1, -1}, {0, 0, 0}},    //0
    {{1, -1, -1}, {255, 0, 0}},   //1
    {{1, 1, -1}, {255, 255, 0}},  //2
    {{-1, 1, -1}, {0, 255, 0}},   //3
    {{-1, -1, 1}, {0, 0, 255}},   //4
    {{1, -1, 1}, {255, 0, 255}},  //5
    {{1, 1, 1}, {255, 255, 255}}, //6
    {{-1, 1, 1}, {0, 255, 255}},  //7
};

GLubyte gui::models::cubeIndices[36] = {
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
