#include "Objects.hpp"

#include <algorithm>

#include "glad/glad.h"
#include "data/IteratorUtils.hpp"
#include "GlUtils.hpp"
#include "data/Io.hpp"

glutil::Program::Program() : shaders(),
                             id(0)
{
}

glutil::Program::Program(const Program &program)
{
    this->CopyFrom(program);
}

glutil::Program::Program(Program &&program)
{
    this->TransferFrom(program);
}

glutil::Program::~Program()
{
    this->DestroyGlObjects();
}

void glutil::Program::DestroyGlObjects()
{
    if (this->id)
    {
        glDeleteProgram(this->id);
        this->id = 0;
    }
}

void glutil::Program::CopyFrom(const Program &program)
{
    this->shaders = program.shaders;
    this->Link();
}

void glutil::Program::TransferFrom(Program &program)
{
    this->shaders = program.shaders;
    this->id = program.id;

    program.shaders.clear();
    program.id = 0;
}

void glutil::Program::AttachAll(const std::initializer_list<Shader *> &shaderList)
{
    this->shaders.insert(this->shaders.end(), shaderList);
}

void glutil::Program::Attach(Shader *shader)
{
    this->shaders.push_back(shader);
}

void glutil::Program::Detach(Shader *shader)
{
    auto pos = std::find(this->shaders.begin(), this->shaders.end(), shader);

    if (pos != this->shaders.end())
        this->shaders.erase(pos);
}

void glutil::Program::Clear()
{
    this->shaders.clear();
}

bool glutil::Program::Link()
{
    this->DestroyGlObjects();
    this->id = glCreateProgram();

    for (auto shader : this->shaders)
    {
        glAttachShader(this->id, shader->GetId());
    }

    glLinkProgram(this->id);

    if (!glutil::CheckProgram(this->id))
        return false;
    else
        util::dbg.WriteLine("Linked program % from: {%}.", this->id, util::WrapPointerIterable(this->shaders.begin(), this->shaders.end(), "}, {"));

    return true;
}

bool glutil::Program::LinkAll(const std::initializer_list<Shader *> &shaders)
{
    this->Clear();
    this->AttachAll(shaders);
    return this->Link();
}

bool glutil::Program::ReloadAll()
{
    for (auto shader : this->shaders)
        shader->Reload();

    return this->Link();
}

void glutil::Program::Use()
{
    glUseProgram(this->id);
}

glutil::Program &glutil::Program::operator=(const Program &program)
{
    this->CopyFrom(program);

    return *this;
}

glutil::Program &glutil::Program::operator=(Program &&program)
{
    this->TransferFrom(program);

    return *this;
}