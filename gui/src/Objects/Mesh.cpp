#include "Objects.hpp"
#include "data/Io.hpp"

glutil::Mesh::Mesh() : vertexCount(0),
                       indexCount(0),
                       vertexSize(0),
                       indexSize(0),
                       indexType(0),
                       attributes(0),
                       vertices(nullptr),
                       indices(nullptr),
                       drawMode(0),
                       dataManaged(false)
{
}

glutil::Mesh::~Mesh()
{
    util::dbg.WriteLine("Destroying Mesh [%]", this);

    this->vertexCount = 0;
    this->vertexSize = 0;
    this->vertices = nullptr;
    this->indexCount = 0;
    this->indexSize = 0;
    this->indexType = 0;
    this->indices = nullptr;

    this->attributes.clear();

    if (this->dataManaged)
    {
        util::dbg.WriteLine("Deleting data of Mesh [%]", this);

        delete[] this->vertices;
        delete[] this->indices;
    }

    this->drawMode = 0;
}

int glutil::Mesh::CalculateIndexSize(int type)
{
    switch (type)
    {
    case GL_UNSIGNED_BYTE:
        return sizeof(GLubyte);
    case GL_UNSIGNED_SHORT:
        return sizeof(GLushort);
    case GL_UNSIGNED_INT:
        return sizeof(GLuint);
    default:
        return 0;
    }
}

bool glutil::Mesh::LoadFromData(int vertexCount, int vertexSize, void *vertices, int indexCount, int indexType, void *indices, std::vector<GeometryBufferAttribute> attributes, int drawMode, bool managaData)
{
    this->vertexCount = vertexCount;
    this->vertexSize = vertexSize;
    this->vertices = vertices;
    this->indexCount = indexCount;
    this->indexType = indexType;
    this->indexSize = this->CalculateIndexSize(indexType);
    this->indices = indices;
    this->attributes = attributes;
    this->drawMode = drawMode;
    this->dataManaged = managaData;

    util::dbg.WriteLine("Loaded Mesh [%] from data: vertexCount=%, vertexSize=%, indexCount=%, indexType=%, indexSize=%, drawMode=%, attributes=%",
                        this,
                        this->vertexCount,
                        this->vertexSize,
                        this->indexCount,
                        this->indexType,
                        this->indexSize,
                        this->drawMode,
                        attributes.size());

    for (const auto &attrib : this->attributes)
    {
        util::dbg.WriteLine("Loaded Attribute % for Mesh [%]: count=%, type=%, normalized=%, offset=%",
                            attrib.GetIndex(),
                            this,
                            attrib.GetCount(),
                            attrib.GetType(),
                            attrib.IsNormalized(),
                            attrib.GetOffset());
    }

    return true;
}

int glutil::Mesh::GetVertexCount() const
{
    return this->vertexCount;
}

int glutil::Mesh::GetVertexSize() const
{
    return this->vertexSize;
}

int glutil::Mesh::GetVertexDataSize() const
{
    return this->vertexCount * this->vertexSize;
}

const void *glutil::Mesh::GetVertexData() const
{
    return this->vertices;
}

int glutil::Mesh::GetIndexCount() const
{
    return this->indexCount;
}

int glutil::Mesh::GetIndexSize() const
{
    return this->indexSize;
}

int glutil::Mesh::GetIndexType() const
{
    return this->indexType;
}

int glutil::Mesh::GetIndexDataSize() const
{
    return this->indexCount * this->indexSize;
}

const void *glutil::Mesh::GetIndexData() const
{
    return this->indices;
}

int glutil::Mesh::GetDrawMode() const
{
    return this->drawMode;
}

const std::vector<glutil::GeometryBufferAttribute> &glutil::Mesh::GetAttributes() const
{
    return this->attributes;
}