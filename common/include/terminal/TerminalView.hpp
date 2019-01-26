#pragma once

#include <curses.h>
#include <string>
#include <tuple>

#include "Util.hpp"
#include "Values.hpp"
#include "Geometry.hpp"
#include "graphics/Color.hpp"
#include "data/collection/Array.hpp"
#include "data/Json.hpp"

namespace terminal
{
typedef int32_t colorId_t;
typedef int32_t colorPairId_t;
typedef std::tuple<colorId_t, colorId_t> ColorPair;

class ColorPallette : public json::IJsonSerializable
{
  private:
    util::Array<util::Color> colors;
    util::Array<ColorPair> colorPairs;

  public:
    ColorPallette();
    ColorPallette(size_t colors, size_t colorPairs);
    ColorPallette(const util::Array<util::Color> &, const util::Array<ColorPair> &);

    virtual json::Node *Serialize();
    virtual void Deserialize(const json::Node *);

    const util::Array<util::Color> &GetColors() const;
    const util::Array<ColorPair> &GetColorPairs() const;

    util::Array<util::Color> &GetColors();
    util::Array<ColorPair> &GetColorPairs();

    size_t GetColorCount() const;
    size_t GetColorPairCount() const;

    bool IsCompatible(size_t colors, size_t colorPairs) const;
    bool Matches(size_t colors, size_t colorPairs) const;

    bool operator==(const ColorPallette &) const;
    bool operator!=(const ColorPallette &) const;
};

namespace controlStyles
{
class ControlStyleColorPallette : public ColorPallette
{
  private:
    colorPairId_t controlText;
    colorPairId_t disabledControlText;
    colorPairId_t controlBorder;
    colorPairId_t inactiveControlBorder;
    colorPairId_t disabledControlBorder;

  public:
    ControlStyleColorPallette(const util::Array<util::Color> &, const util::Array<ColorPair> &);

    colorPairId_t GetControlText() const;
    colorPairId_t GetDisabledControlText() const;
    colorPairId_t GetControlBorder() const;
    colorPairId_t GetInactiveControlBorder() const;
    colorPairId_t GetDisabledControlBorder() const;

    void SetControlText(colorPairId_t);
    void SetDisabledControlText(colorPairId_t);
    void SetControlBorder(colorPairId_t);
    void SetInactiveControlBorder(colorPairId_t);
    void SetDisabledControlBorder(colorPairId_t);

    virtual json::Node *Serialize();
    virtual void Deserialize(const json::Node *);
};

std::tuple<size_t, size_t> LoadStyles();

extern ControlStyleColorPallette defaultMinimal;
extern ControlStyleColorPallette defaultReduced;
extern ControlStyleColorPallette defaultFull;

extern colorPairId_t controlText;
extern colorPairId_t disabledControlText;
extern colorPairId_t controlBorder;
extern colorPairId_t inactiveControlBorder;
extern colorPairId_t disabledControlBorder;
} // namespace controlStyles

class TerminalView
{
  public:
    ~TerminalView();

    static TerminalView *GetInstance();
    static void DeleteInstance();

    void SetInputMode(terminal::InputMode mode);
    void SetEcho(bool echo);

    util::Dimension GetSize() const;
    bool SetSize(const util::Dimension &);

    int ReadKey();
    std::string ReadLine();

    bool ReadKey(long timeOut, int &result);
    bool ReadLine(long timeOut, std::string &result);

    void Print(const std::string &text);
    void Print(const std::string &text, int x, int y);
    void Print(char c);
    void Print(char c, int x, int y);
    void Clear(const util::Rectangle &area);
    void Flush();

    void Maximise();
    void SetLiveMode(bool);

    void SetAttribute(terminal::OutputAttribute, bool);
    void ResetAttribtues();
    void AttributeOn(terminal::OutputAttribute);
    void AttributeOff(terminal::OutputAttribute);
    void SetActiveAttributes(terminal::OutputAttribute);
    OutputAttribute GetActiveAttributes() const;

    void SetActiveColorPair(colorPairId_t);
    void SetBackgroundColorPair(colorPairId_t);
    colorPairId_t GetActiveColorPair() const;
    colorPairId_t GetActiveBackground() const;
    const ColorPair &GetContent(colorPairId_t id) const;
    const util::Color &GetColor(colorId_t id) const;

    size_t GetMaxColors() const;
    size_t GetMaxColorPairs() const;
    bool CanChangeColors() const;

    bool BufferColor(colorId_t, const util::Color &);
    bool BufferColorPair(colorPairId_t, colorId_t, colorId_t);

    int CreateStyle(terminal::OutputAttribute attributes, int colorPair);

    util::Point GetCursorPosition() const;
    void SetCursorPosition(const util::Point &point);
    terminal::CursorMode GetCursorMode() const;
    void SetCursorMode(terminal::CursorMode);

    bool CursorVisibilitySupported() const;
    bool ColorsSupported() const;

    void ApplyColorPallette(const ColorPallette &);
    ColorPallette ExportCurrentColorPallette() const;
    void LoadColorPalletteFromJson(const std::string &path);
    void SaveCurrentColorPallette(const std::string &path) const;
    void RestoreDefaultColors();

  protected:
    void OnTerminalPropertyChanged();

  private:
    static TerminalView *instance;

    TerminalView();
    TerminalView(int width, int height);

    WINDOW *window;
    int width;
    int height;
    char inputBuf[4096];

    InputMode inputMode;
    bool echoOn;
    bool liveMode;
    bool colorsSupported;
    bool cursorModeSupported;
    CursorMode cursorMode;
    util::Point cursorPos;

    util::Array<util::Color> colors;
    util::Array<ColorPair> colorPairs;
    int activeColorPair;
    int activeBackgroundColorPair;

    OutputAttribute activeAttributes;
};
} // namespace terminal