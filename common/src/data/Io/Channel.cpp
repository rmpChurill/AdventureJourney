#include <cstdarg>

#include "data/Io.hpp"
#include "Exception.hpp"

util::Channel::Channel() : file(stdout),
                           active(true)
{
}

util::Channel::Channel(const Channel &copyFrom) : file(copyFrom.file),
                                                  active(copyFrom.active)
{
}

util::Channel::Channel(std::FILE *target) : file(target),
                                            active(true)
{
}

util::Channel &util::Channel::operator=(const Channel &copyFrom)
{
    if (this == &copyFrom)
        return *this;

    this->file = copyFrom.file;
    this->active = copyFrom.active;

    return *this;
}

bool util::Channel::IsActive() const
{
    return this->active;
}

bool util::Channel::IsOpen() const
{
    return this->file != nullptr;
}

void util::Channel::Close()
{
    this->file = nullptr;
}

void util::Channel::SetActive(bool active)
{
    this->active = active;
}

const std::FILE *util::Channel::GetTarget() const
{
    return this->file;
}

std::FILE *util::Channel::GetTarget()
{
    return this->file;
}

void util::Channel::SetTarget(std::FILE *target)
{
    this->file = target;
}

int util::Channel::Print(const char *format, ...)
{
    if (!this->active || this->file == nullptr)
        return -1;

    va_list args;
    va_start(args, format);

    std::vfprintf(this->file, format, args);

    va_end(args);
}

int util::Channel::Print(const char *format, va_list args)
{
    if (!this->active || this->file == nullptr)
        return -1;

    return std::vfprintf(this->file, format, args);
}

int util::Channel::PrintLine(const char *format, ...)
{
    if (!this->active || this->file == nullptr)
        return -1;

    va_list args;
    va_start(args, format);

    int res = std::vfprintf(this->file, format, args);
    std::fprintf(this->file, "\n");

    va_end(args);

    return res + 1;
}

int util::Channel::PrintLine(const char *format, va_list args)
{
    if (!this->active || this->file == nullptr)
        return -1;

    int res = std::vfprintf(this->file, format, args);
    std::fprintf(this->file, "\n");

    return res + 1;
}

int util::Channel::Print(const std::string &format, ...)
{
    if (!this->active || this->file == nullptr)
        return -1;

    va_list args;
    va_start(args, format);

    std::vfprintf(this->file, format.c_str(), args);

    va_end(args);
}

int util::Channel::Print(const std::string &format, va_list args)
{
    if (!this->active || this->file == nullptr)
        return -1;

    return std::vfprintf(this->file, format.c_str(), args);
}

int util::Channel::PrintLine(const std::string &format, ...)
{
    if (!this->active || this->file == nullptr)
        return -1;

    va_list args;
    va_start(args, format);

    int res = std::vfprintf(this->file, format.c_str(), args);
    std::fprintf(this->file, "\n");

    va_end(args);

    return res + 1;
}

int util::Channel::PrintLine(const std::string &format, va_list args)
{
    if (!this->active || this->file == nullptr)
        return -1;

    int res = std::vfprintf(this->file, format.c_str(), args);
    std::fprintf(this->file, "\n");

    return res + 1;
}