#include "Objects.hpp"
#include "data/Io.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"

glutil::SceneObject::SceneObject(GeometryBuffer *data, int bufferOffset, int indexCount, int offset, int drawMode, int indexType, bool manageGeometryBuffer) : bufferOffset(bufferOffset),
                                                                                                                                                               indexCount(indexCount),
                                                                                                                                                               offset(offset),
                                                                                                                                                               drawMode(drawMode),
                                                                                                                                                               indexType(indexType),
                                                                                                                                                               geometry(geometry),
                                                                                                                                                               geometryManaged(manageGeometryBuffer)
{
    util::dbg.WriteLine("Created Scene Object [%] from data: buffer=%, bufferOffset=%, indexCount=%, offset=%, drawMode=%, indexType=%, manageBuffer=%",
                        this,
                        data,
                        bufferOffset,
                        indexCount,
                        offset,
                        drawMode,
                        indexType,
                        manageGeometryBuffer);
}

glutil::SceneObject::SceneObject(const Mesh &mesh) : bufferOffset(0),
                                                     indexCount(mesh.GetIndexCount()),
                                                     offset(0),
                                                     drawMode(mesh.GetDrawMode()),
                                                     indexType(mesh.GetIndexType()),
                                                     geometry(new GeometryBuffer(mesh)),
                                                     geometryManaged(true)
{
    util::dbg.WriteLine("Created Scene Object [%] from mesh [%]: drawMode=%, indexType=%",
                        this,
                        &mesh,
                        drawMode,
                        indexType);
}

glutil::SceneObject::~SceneObject()
{
    util::dbg.WriteLine("Destroying Scene Object [%]", this);

    if (this->geometryManaged)
    {
        util::dbg.WriteLine("Deleting geometry buffer of scene object [%]", this);
        delete this->geometry;
    }
}

const glm::mat4 &glutil::SceneObject::GetModelMatrix() const
{
    return this->data.modelMatrix;
}

void glutil::SceneObject::SetModelMatrix(const glm::mat4 &mat)
{
    this->data.modelMatrix = mat;
    this->data.normalMatrix = glm::inverseTranspose(glm::mat3(mat));
    this->SetDirty();
}

void glutil::SceneObject::Render()
{
    this->geometry->Bind();
    glDrawElements(this->drawMode, this->indexCount, this->indexType, (void *)this->offset);
}