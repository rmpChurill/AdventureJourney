#include "Objects.hpp"

template <class TBuilder, uint PChannels>
bool glutil::Texture::LoadDataFromBuilderCore(const TBuilder &builder)
{
    auto writer = glutil::Texture::FormatConverter<PChannels>();
    auto pixels = new uint8_t[static_cast<size_t>(this->width) * static_cast<size_t>(this->height) * PChannels];
    auto pos = pixels;
    auto stepX = 1.f / this->width;
    auto stepY = 1.f / this->height;

    for (int i = 0; i < this->width * this->height; i++)
        pos = writer(pos, builder((i % this->width) * stepX, static_cast<int>(i / this->height) * stepY));

    auto res = this->LoadDataFromMemory(pixels);

    delete[] pixels;

    return res;
}

template <class TBuilder>
bool glutil::Texture::LoadDataFromBuilder(const TBuilder &builder)
{
    switch (this->GetChannelCountFromFormat())
    {
    case 1:
        return this->LoadDataFromBuilderCore<TBuilder, 1>(builder);
    case 2:
        return this->LoadDataFromBuilderCore<TBuilder, 2>(builder);
    case 3:
        return this->LoadDataFromBuilderCore<TBuilder, 3>(builder);
    case 4:
        return this->LoadDataFromBuilderCore<TBuilder, 4>(builder);
    }
}

template <class TBuilder, uint PChannels>
bool glutil::Texture::LoadCubeMapFromBuilderCore(const TBuilder &builder)
{
    auto len = static_cast<size_t>(this->width) * static_cast<size_t>(this->height) * PChannels;
    auto writer = glutil::Texture::FormatConverter<PChannels>();
    auto pixels = new uint8_t[6 * len];
    auto pos = pixels;
    auto stepX = 1.f / this->width;
    auto stepY = 1.f / this->height;

    for (int s = 0; s < 6; s++)
        for (int i = 0; i < this->width * this->height; i++)
            pos = writer(pos, builder(s, (i % this->width) * stepX, static_cast<int>(i / this->height) * stepY));

    uint8_t *dataBlocks[6] = {
        &pixels[len * 0],
        &pixels[len * 1],
        &pixels[len * 2],
        &pixels[len * 3],
        &pixels[len * 4],
        &pixels[len * 5],
    };

    auto res = this->LoadCubeMapFromMemory(static_cast<const void**>(&dataBlocks[0]));

    delete[] pixels;

    return res;
}

template <class TBuilder>
bool glutil::Texture::LoadCubeMapFromBuilder(const TBuilder &builder)
{
    switch (this->GetChannelCountFromFormat())
    {
    case 1:
        return this->LoadCubeMapFromBuilderCore<TBuilder, 1>(builder);
    case 2:
        return this->LoadCubeMapFromBuilderCore<TBuilder, 2>(builder);
    case 3:
        return this->LoadCubeMapFromBuilderCore<TBuilder, 3>(builder);
    case 4:
        return this->LoadCubeMapFromBuilderCore<TBuilder, 4>(builder);
    }
}