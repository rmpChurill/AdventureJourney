#include "Objects.hpp"

#include <cstdio>

#include "data/Io.hpp"
#include "data/IteratorUtils.hpp"
#include "stb/stb_image.h"

namespace
{
constexpr uint32_t WRAP_R = 0;
constexpr uint32_t WRAP_S = 1;
constexpr uint32_t WRAP_T = 2;
constexpr uint32_t FILTER_MIN = 0;
constexpr uint32_t FILTER_MAG = 1;
} // namespace

glutil::Texture::Texture() : format(GL_RGBA),
                             internalFormat(GL_RGBA),
                             target(GL_TEXTURE_2D),
                             width(0),
                             height(0),
                             wrapModes{GL_REPEAT, GL_REPEAT, GL_REPEAT},
                             filterModes{GL_LINEAR, GL_LINEAR},
                             createMipmaps(false),
                             maxAnisotropy(0),
                             tex(0)
{
}

glutil::Texture::~Texture()
{
    this->DestroyGlObjects();
}

void glutil::Texture::DestroyGlObjects()
{
    if (this->tex)
    {
        glDeleteTextures(1, &this->tex);
        this->tex = 0;
    }
}

void glutil::Texture::PrepareLoad()
{
    this->DestroyGlObjects();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &this->tex);
    glBindTexture(this->target, this->tex);
}

void glutil::Texture::SetTextureParameters()
{
    glTexParameteri(this->target, GL_TEXTURE_WRAP_R, this->wrapModes[WRAP_R]);
    glTexParameteri(this->target, GL_TEXTURE_WRAP_S, this->wrapModes[WRAP_S]);
    glTexParameteri(this->target, GL_TEXTURE_WRAP_T, this->wrapModes[WRAP_T]);
    glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, this->filterModes[FILTER_MIN]);
    glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, this->filterModes[FILTER_MAG]);

    if (this->maxAnisotropy > 0)
        glTexParameterf(this->target, GL_TEXTURE_MAX_ANISOTROPY, this->maxAnisotropy);

    if (this->createMipmaps)
        glGenerateMipmap(this->target);
}

int glutil::Texture::GetChannelCountFromFormat()
{
    switch (this->format)
    {
    case GL_RED:
        return 1;
    case GL_RG:
        return 2;
    case GL_RGB:
        return 3;
    case GL_RGBA:
    default:
        return 4;
    }
};

GLenum glutil::Texture::GetFormatFromChannelCount(int n)
{
    switch (n)
    {
    case 1:
        return GL_RED;
    case 2:
        return GL_RG;
    case 3:
        return GL_RGB;
    case 4:
    default:
        return GL_RGBA;
    }
}

bool glutil::Texture::LoadDataFromMemory(void *data)
{
    this->PrepareLoad();

    glTexImage2D(this->target, 0, this->internalFormat,
                 this->width, this->height, 0,
                 this->format, GL_UNSIGNED_BYTE, data);

    this->SetTextureParameters();

    util::dbg.WriteLine("Copying data to buffer %. Target=%, internalFormat=%, width=%, height=%, format=%\nDone!\n",
                        this->tex,
                        this->target,
                        this->internalFormat,
                        this->width,
                        this->height,
                        this->format);

    return true;
}

bool glutil::Texture::LoadData(const std::string &path)
{
    if (!this->format || !this->internalFormat || !this->target)
        return false;

    util::dbg.WriteLine("Loading texture from %...", path);

    auto file = std::fopen(path.c_str(), "r");

    if (!file)
    {
        util::dbg.WriteLine("Error: Could not open file!");

        return false;
    }

    int nChannels;

    stbi_set_flip_vertically_on_load(true);
    auto pixels = stbi_load(path.c_str(), &this->width, &this->height, &nChannels, 0);

    this->format = this->GetFormatFromChannelCount(nChannels);

    if (!pixels)
    {
        util::dbg.WriteLine("Error: Invalid or corrupted data! Message: %", stbi_failure_reason());

        return false;
    }

    auto res = this->LoadDataFromMemory(pixels);

    stbi_image_free(pixels);

    return res;
}

bool glutil::Texture::LoadCubeMapCore(const std::vector<std::string> &files)
{
    util::dbg.WriteLine("Loading cubemap from files...");

    if (files.size() != 6)
    {
        util::err.WriteLine("Error: excatly 6 images required, % provided!", files.size());
        return false;
    }

    util::dbg.WriteLine("Sources are: \n\tnx=%\n\tpx=%\n\tny=%\n\tpy=%\n\tnz=%\n\tpz=%",
                        files[0], files[1], files[2], files[3], files[4], files[5]);

    void *maps[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    auto res = true;

    for (int i = 0; i < 6; i++)
    {
        int nChannels;

        stbi_set_flip_vertically_on_load(true);
        maps[i] = stbi_load(files[i].c_str(), &this->width, &this->height, &nChannels, 0);

        //TODO: hier könnte man noch prüfen, ob das Format stimmt!

        this->format = this->GetFormatFromChannelCount(nChannels);

        if (!maps[i])
        {
            util::dbg.WriteLine("Error: Invalid or corrupt data in %! Message: %", files[i], stbi_failure_reason());

            res = false;

            break;
        }
    }

    if (res)
        res = this->LoadCubeMapCore(&maps[0]);

    for (int i = 0; i < 6; i++)
        if (maps[i])
            stbi_image_free(maps[i]);

    return res;
}

bool glutil::Texture::LoadCubeMapCore(const void *const *bufs)
{
    this->target = GL_TEXTURE_CUBE_MAP;
    this->SetWrapMode(GL_CLAMP_TO_EDGE);

    this->PrepareLoad();

    GLenum faces[] = {
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    };

    for (int i = 0; i < 6; i++)
    {
        glTexImage2D(faces[i], 0,
                     this->internalFormat, this->width, this->height,
                     0, this->format, GL_UNSIGNED_BYTE, bufs[i]);
    }

    this->SetTextureParameters();

    util::dbg.WriteLine("Copying data to buffer %. Target=%, internalFormat=%, width=%, height=%, format=%\nDone!\n",
                        this->tex,
                        this->target,
                        this->internalFormat,
                        this->width,
                        this->height,
                        this->format);

    return true;
}

bool glutil::Texture::LoadCubeMap(const std::string &directory, const std::vector<std::string> &files)
{
    std::vector<std::string> paths;

    for (const auto &file : files)
        paths.push_back(directory + file);

    return this->LoadCubeMapCore(paths);
}

bool glutil::Texture::LoadCubeMap(const std::string &directory, const std::string &extension)
{
    return this->LoadCubeMapCore({
        directory + "nx" + extension,
        directory + "px" + extension,
        directory + "ny" + extension,
        directory + "py" + extension,
        directory + "nz" + extension,
        directory + "pz" + extension,
    });
}

bool glutil::Texture::LoadCubeMap(const std::vector<std::string> &paths)
{
    return this->LoadCubeMapCore(paths);
}

bool glutil::Texture::LoadCubeMapFromMemory(const void *const *bufs)
{
    util::dbg.WriteLine("Loading cubemap from buffer...");

    return this->LoadCubeMapCore(bufs);
}

void glutil::Texture::Bind(GLuint textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(this->target, this->tex);
}

void glutil::Texture::SetFormat(int fromat)
{
    this->format = format;
}

void glutil::Texture::SetInternalFormat(int internalFormat)
{
    this->internalFormat = internalFormat;
}

void glutil::Texture::SetDataType(int type)
{
    this->dataType = type;
}

void glutil::Texture::SetTarget(int target)
{
    this->target = target;
}

void glutil::Texture::SetSize(int w, int h)
{
    this->width = w;
    this->height = h;
}

void glutil::Texture::SetWidth(int w)
{
    this->width = w;
}

void glutil::Texture::SetHeight(int h)
{
    this->height = h;
}

void glutil::Texture::SetMipmapsEnabled(bool enabled)
{
    this->createMipmaps = enabled;
}

void glutil::Texture::SetFilterMode(GLenum filterMode)
{
    this->filterModes[FILTER_MIN] = filterMode;
    this->filterModes[FILTER_MAG] = filterMode;
}

void glutil::Texture::SetMinFilterMode(GLenum filterMode)
{
    this->filterModes[FILTER_MIN] = filterMode;
}

void glutil::Texture::SetMagFilterMode(GLenum filterMode)
{
    this->filterModes[FILTER_MAG] = filterMode;
}

void glutil::Texture::SetWrapMode(GLenum wrapMode)
{
    this->wrapModes[WRAP_R] = wrapMode;
    this->wrapModes[WRAP_S] = wrapMode;
    this->wrapModes[WRAP_T] = wrapMode;
}

void glutil::Texture::SetWrapModeR(GLenum wrapMode)
{
    this->wrapModes[WRAP_R] = wrapMode;
}

void glutil::Texture::SetWrapModeS(GLenum wrapMode)
{
    this->wrapModes[WRAP_S] = wrapMode;
}

void glutil::Texture::SetWrapModeT(GLenum wrapMode)
{
    this->wrapModes[WRAP_T] = wrapMode;
}

void glutil::Texture::SetMaxAnisotropy(double maxAnisotropy)
{
    this->maxAnisotropy = maxAnisotropy;
}

void glutil::Texture::SetAnisotropicFilterDisabled()
{
    this->maxAnisotropy = 0;
}

GLuint glutil::Texture::GetId() const
{
    return this->tex;
}

GLuint glutil::Texture::DisownId()
{
    auto res = this->tex;

    this->tex = 0;

    return res;
}
