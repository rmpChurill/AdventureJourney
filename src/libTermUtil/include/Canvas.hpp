#pragma once

#include "View.hpp"

namespace terminal
{
class Canvas
{
  private:
    View *view;
    util::Dimension size;
    util::Point origin;
    util::Rectangle clippedArea;

    int X(int);
    int Y(int);

  public:
    Canvas();
    Canvas(const Canvas &);

    terminal::Canvas GetSubCanvas(int x, int y, int w, int h);
    terminal::Canvas GetSubCanvas(const util::Rectangle &);

    terminal::Canvas &DrawVerticalLine(int x, int y, int length, char c);
    terminal::Canvas &DrawVerticalLine(const util::Point &, int length, char c);
    terminal::Canvas &DrawHorizontalLine(int x, int y, int length, char c);
    terminal::Canvas &DrawHorizontalLine(const util::Point &, int length, char c);
    terminal::Canvas &DrawBox(int x, int y, int width, int height, char horizontal, char vertical, char cornor);
    terminal::Canvas &DrawBox(const util::Rectangle &, char horizontal, char vertical, char cornor);
    terminal::Canvas &DrawBox(int x, int y, int width, int height, char c);
    terminal::Canvas &DrawBox(const util::Rectangle &, char c);
    terminal::Canvas &DrawString(int x, int y, const std::string &);
    terminal::Canvas &DrawString(const util::Point &, const std::string &);
    terminal::Canvas &DrawChar(int x, int y, int value);
    terminal::Canvas &DrawChar(const util::Point &, int value);
    terminal::Canvas &Fill(int x, int y, int width, int height, char c);
    terminal::Canvas &Fill(const util::Rectangle &, char c);
    terminal::Canvas &Clear(char c);
    terminal::Canvas &Clear();
    terminal::Canvas &Reset();

    const util::Dimension &GetSize() const;
    const util::Point &GetOrigin() const;
    const util::Rectangle &GetClippedArea() const;

    terminal::Canvas &SetOrigin(int x, int y);
    terminal::Canvas &SetOrigin(const util::Point &);
    terminal::Canvas &ClipArea(int x, int y, int width, int height);
    terminal::Canvas &ClipArea(const util::Rectangle &);
    terminal::Canvas &DisableClip();
    terminal::Canvas &EnableAttribute(terminal::OutputAttribute);
    terminal::Canvas &DisableAttribute(terminal::OutputAttribute);
    terminal::Canvas &SetBackgroundColorPair(colorPairId_t);
    terminal::Canvas &SetActiveColorPair(colorPairId_t);

    void Flush();

    View *GetView() const;
};
} // namespace terminal