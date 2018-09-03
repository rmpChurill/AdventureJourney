#pragma once

#include "defs.hpp"

namespace util
{
class color
{
  private:
    int data;
    static constexpr int A_MASK = 0xff000000;
    static constexpr int A_SHIFT = 24;
    static constexpr int R_MASK = 0x00ff0000;
    static constexpr int R_SHIFT = 16;
    static constexpr int G_MASK = 0x0000ff00;
    static constexpr int G_SHIFT = 8;
    static constexpr int B_MASK = 0x000000ff;
    static constexpr int B_SHIFT = 0;
    static constexpr int LOW_BYTE_MASK = 0x000000ff;

  public:
    color();
    color(const color &copy);
    color(int r, int g, int b, int a = 255);
    color(float r, float g, float b, float a = 1.0f);
    color(int color);
    int alpha() const;
    int red() const;
    int green() const;
    int blue() const;
    float alpha_percentage() const;
    float red_percentage() const;
    float green_percentage() const;
    float blue_percentage() const;
    int value() const;

    friend bool util::operator==(const util::color &, const util::color &);
    friend bool util::operator!=(const util::color &, const util::color &);
}; /*color*/

bool operator==(const util::color &, const util::color &);
bool operator!=(const util::color &, const util::color &);

const util::color color_black = color(0.0f, 0.0f, 0.0f);
const util::color color_red = color(1.0f, 0.0f, 0.0f);
const util::color color_green = color(0.0f, 1.0f, 0.0f);
const util::color color_blue = color(0.0f, 0.0f, 1.0f);
const util::color color_cyan = color(0.0f, 1.0f, 1.0f);
const util::color color_magenta = color(1.0f, 0.0f, 1.0f);
const util::color color_yellow = color(1.0f, 1.0f, 0.0f);
const util::color color_white = color(1.0f, 1.0f, 1.0f);
} // namespace util