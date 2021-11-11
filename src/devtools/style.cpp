#pragma once

#include "DevTools.hpp"
#include <imgui-hook.hpp>

constexpr GLubyte limitc(int v) {
    return static_cast<GLubyte>(v > 255 ? 255 : v < 0 ? 0 : v);
}

template<GLubyte R, GLubyte G, GLubyte B, GLubyte A>
struct ConstRGBA {
    GLubyte r = R;
    GLubyte g = G;
    GLubyte b = B;
    GLubyte a = A;
    constexpr ConstRGBA() = default;
    constexpr ImVec4 gen() const {
        return ImVec4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
    }
};

template<int Hex>
struct ConstRGBAHex {
    GLubyte r = (Hex >> 24) & 0xff;
    GLubyte g = (Hex >> 16) & 0xff;
    GLubyte b = (Hex >> 8)  & 0xff;
    GLubyte a = (Hex >> 0)  & 0xff;
    constexpr ConstRGBAHex() = default;
    constexpr ImVec4 gen() const {
        return ImVec4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
    }
};

template<int Hex>
struct ConstRGBAHexShort {
    GLubyte r = ((Hex >> 12) & 0xf) * 0x11;
    GLubyte g = ((Hex >> 8)  & 0xf) * 0x11;
    GLubyte b = ((Hex >> 4)  & 0xf) * 0x11;
    GLubyte a = ((Hex >> 0)  & 0xf) * 0x11;
    constexpr ConstRGBAHexShort() = default;
    constexpr ImVec4 gen() const {
        return ImVec4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
    }
};

#define CN(_var_) _var_##_color

#define COMPOSE_COLOR_ONLY(_var_, _r_, _g_, _b_, _a_)                               \
    static constexpr const ConstRGBA CN(_var_) = ConstRGBA<_r_, _g_, _b_, _a_>();   \

#define COMPOSE_COLOR(_var_, _r_, _g_, _b_, _a_)                                        \
    static constexpr const ConstRGBA CN(_var_) = ConstRGBA<_r_, _g_, _b_, _a_>();   \
    colors[ImGuiCol_##_var_] = CN(_var_).gen();

#define COMPOSE_COLOR_HEX(_var_, _hex_)                                     \
    static constexpr const ConstRGBAHex CN(_var_) = ConstRGBAHex<_hex_>();  \
    colors[ImGuiCol_##_var_] = CN(_var_).gen();

#define COMPOSE_COLOR_HEXS(_var_, _hex_)                                            \
    static constexpr const ConstRGBAHexShort CN(_var_) = ConstRGBAHexShort<_hex_>();\
    colors[ImGuiCol_##_var_] = CN(_var_).gen();

#define LIGHTEN_COLOR(_var_, _other_, _add_)        \
    static constexpr const ConstRGBA CN(_var_) =    \
    ConstRGBA<                                      \
        limitc(CN(_other_).r + _add_),              \
        limitc(CN(_other_).g + _add_),              \
        limitc(CN(_other_).b + _add_),              \
        limitc(CN(_other_).a)>();                   \
    colors[ImGuiCol_##_var_] = CN(_var_).gen();

#define LIGHTEN_COLOR_A(_var_, _other_, _add_, _a_) \
    static constexpr const ConstRGBA CN(_var_) =    \
    ConstRGBA<                                      \
        limitc(CN(_other_).r + _add_),              \
        limitc(CN(_other_).g + _add_),              \
        limitc(CN(_other_).b + _add_),              \
        limitc(CN(_other_).a + _a_)>();             \
    colors[ImGuiCol_##_var_] = CN(_var_).gen();

#define COPY_COLOR(_var_, _other_)                              \
    static constexpr const decltype(CN(_other_)) CN(_var_) = CN(_other_);   \
    colors[ImGuiCol_##_var_] = CN(_var_).gen();

#define COPY_COLOR_A(_var_, _other_, _a_)                              \
    static constexpr const ConstRGBA CN(_var_) =          \
    ConstRGBA<CN(_other_).r, CN(_other_).g, CN(_other_).b, _a_>();   \
    colors[ImGuiCol_##_var_] = CN(_var_).gen();

void DevTools::loadTheme(DevToolsTheme theme) {
    auto& colors = ImGui::GetStyle().Colors;
    switch (theme) {
        case kDevToolsThemeLight: {
            COMPOSE_COLOR_ONLY( _MainBg,         240, 240, 240, 255 );
            COMPOSE_COLOR_ONLY( _AltBg,          0, 0, 0, 255 );
            COMPOSE_COLOR_ONLY( _Text,           0, 0, 0, 255 );
            COMPOSE_COLOR_ONLY( _MainColor,      192, 255, 216, 255 );
            COMPOSE_COLOR_ONLY( _Highlight,      192, 216, 255, 255 );

            COPY_COLOR(         Text,           _Text);
            LIGHTEN_COLOR(      TextDisabled,   _Text, -102);
            COPY_COLOR(         WindowBg,       _MainBg);
            COPY_COLOR(         ChildBg,        _AltBg);
            COPY_COLOR(         PopupBg,        WindowBg);
            if (this->m_eMode == kDevToolsModeIntegrated) {
                COPY_COLOR(         Border,         WindowBg);
                COPY_COLOR(         BorderShadow,   WindowBg);
            } else {
                COPY_COLOR_A(       Border,         _MainBg, 23);
                COPY_COLOR_A(       BorderShadow,   _AltBg,  50);
            }
            LIGHTEN_COLOR(      FrameBg,        _MainBg, -20);
            LIGHTEN_COLOR_A(    FrameBgHovered, _MainBg, -35, -80);
            COPY_COLOR_A(       FrameBgActive,  _MainColor, 230);
            COPY_COLOR_A(       TitleBg,        _MainColor, 255);
            LIGHTEN_COLOR_A(   TitleBgCollapsed,_MainColor, -10, -150);
            LIGHTEN_COLOR(      TitleBgActive,  _MainColor, -20);
            LIGHTEN_COLOR(      MenuBarBg,      _MainColor, -20);
            LIGHTEN_COLOR(      ScrollbarBg,    _MainBg,    -150);
            LIGHTEN_COLOR(      ScrollbarGrab,  _MainColor, 0);
            LIGHTEN_COLOR(      ScrollbarGrabHovered,  _MainColor, -50);
            LIGHTEN_COLOR(      ScrollbarGrabActive,   _Highlight, 0);
            LIGHTEN_COLOR_A(    CheckMark,      _Highlight, 0, 255);
            LIGHTEN_COLOR(      SliderGrab,     _MainColor, -50);
            LIGHTEN_COLOR(      SliderGrabActive,_Highlight, 0);
            LIGHTEN_COLOR_A(    Button,         _MainColor, 0, -180);
            LIGHTEN_COLOR(      ButtonHovered,  _MainColor, 10);
            LIGHTEN_COLOR(      ButtonActive,   _Highlight, 10);
            LIGHTEN_COLOR_A(    Header,         _MainColor, -30, -140);
            LIGHTEN_COLOR_A(    HeaderHovered,  _MainColor, -30, -40);
            LIGHTEN_COLOR(      HeaderActive,   _Highlight, 0);
            LIGHTEN_COLOR_A(    Separator,          _MainColor, -160, -140);
            LIGHTEN_COLOR_A(    SeparatorHovered,   _MainColor, -160, -40);
            LIGHTEN_COLOR(      SeparatorActive,    _Highlight, 0);
            LIGHTEN_COLOR_A(    ResizeGrip,         _MainColor, 0, -140);
            LIGHTEN_COLOR_A(    ResizeGripHovered,  _MainColor, 0, -40);
            LIGHTEN_COLOR(      ResizeGripActive,   _Highlight, 0);
            LIGHTEN_COLOR_A(    PlotLines,          _MainColor, 0, -140);
            LIGHTEN_COLOR_A(    PlotLinesHovered,   _MainColor, 0, -40);
            LIGHTEN_COLOR_A(    PlotHistogram,      _MainColor, 0, -140);
            LIGHTEN_COLOR_A(    PlotHistogramHovered,_MainColor, 0, -40);
            LIGHTEN_COLOR_A(    TextSelectedBg,     _MainColor, -60, -180);
            LIGHTEN_COLOR_A(    DragDropTarget,     _MainColor, -60, -20);
            COPY_COLOR(         NavHighlight,       HeaderHovered);
            LIGHTEN_COLOR_A(    NavWindowingHighlight,_MainColor, -40, -40);
            LIGHTEN_COLOR_A(    Tab,                _MainColor, -30, -140);
            LIGHTEN_COLOR_A(    TabHovered,         _MainColor, -30, -40);
            LIGHTEN_COLOR(      TabActive,          _Highlight, 0);
        } break;
        
        case kDevToolsThemeDark: {
            COMPOSE_COLOR_ONLY( _MainBg,         20, 30, 40, 255 );
            COMPOSE_COLOR_ONLY( _AltBg,          20, 30, 40, 255 );
            COMPOSE_COLOR_ONLY( _Text,           255, 255, 255, 255 );
            COMPOSE_COLOR_ONLY( _MainColor,      192, 255, 216, 120 );
            COMPOSE_COLOR_ONLY( _Highlight,      192, 216, 255, 120 );

            COPY_COLOR(         Text,           _Text);
            LIGHTEN_COLOR(      TextDisabled,   _Text, -102);
            COPY_COLOR(         WindowBg,       _MainBg);
            COPY_COLOR(         ChildBg,        _AltBg);
            COPY_COLOR(         PopupBg,        WindowBg);
            if (this->m_eMode == kDevToolsModeIntegrated) {
                COPY_COLOR(         Border,         WindowBg);
                COPY_COLOR(         BorderShadow,   WindowBg);
            } else {
                COPY_COLOR_A(       Border,         _MainBg, 23);
                COPY_COLOR_A(       BorderShadow,   _AltBg,  50);
            }
            LIGHTEN_COLOR(      FrameBg,        _MainBg, 40);
            LIGHTEN_COLOR_A(    FrameBgHovered, _MainBg, 80, 20);
            COPY_COLOR_A(       FrameBgActive,  _MainBg, 230);
            COPY_COLOR_A(       TitleBg,        _MainBg, 255);
            LIGHTEN_COLOR_A(   TitleBgCollapsed,_MainBg, 10, -150);
            LIGHTEN_COLOR(      TitleBgActive,  _MainBg, 20);
            LIGHTEN_COLOR(      MenuBarBg,      _MainColor, 20);
            LIGHTEN_COLOR(      ScrollbarBg,    _MainBg,    150);
            LIGHTEN_COLOR(      ScrollbarGrab,  _MainColor, 0);
            LIGHTEN_COLOR(      ScrollbarGrabHovered,  _MainColor, 50);
            LIGHTEN_COLOR(      ScrollbarGrabActive,   _Highlight, 0);
            LIGHTEN_COLOR_A(    CheckMark,      _Highlight, 0, 255);
            LIGHTEN_COLOR(      SliderGrab,     _MainColor, 50);
            LIGHTEN_COLOR(      SliderGrabActive,_Highlight, 0);
            LIGHTEN_COLOR_A(    Button,         _MainBg, 40, 0);
            LIGHTEN_COLOR_A(    ButtonHovered,  _MainBg, 80, 20);
            LIGHTEN_COLOR(      ButtonActive,   _Highlight, 10);
            COPY_COLOR(         Header,         Button);
            COPY_COLOR(         HeaderHovered,  ButtonHovered);
            COPY_COLOR(         HeaderActive,   ButtonActive);
            LIGHTEN_COLOR(      Separator,          _MainBg, -160);
            LIGHTEN_COLOR_A(    SeparatorHovered,   _MainColor, 160, -40);
            LIGHTEN_COLOR(      SeparatorActive,    _Highlight, 0);
            COPY_COLOR(         ResizeGrip,         Button);
            COPY_COLOR(         ResizeGripHovered,  ButtonHovered);
            COPY_COLOR(         ResizeGripActive,   ButtonActive);
            LIGHTEN_COLOR_A(    PlotLines,          _MainColor, 0, -140);
            LIGHTEN_COLOR_A(    PlotLinesHovered,   _MainColor, 0, -40);
            LIGHTEN_COLOR_A(    PlotHistogram,      _MainColor, 0, -140);
            LIGHTEN_COLOR_A(    PlotHistogramHovered,_MainColor, 0, -40);
            LIGHTEN_COLOR_A(    TextSelectedBg,     _MainColor, 60, -180);
            LIGHTEN_COLOR_A(    DragDropTarget,     _MainColor, 60, -20);
            COPY_COLOR(         NavHighlight,       HeaderHovered);
            LIGHTEN_COLOR_A(    NavWindowingHighlight,_MainColor, 40, -40);
            LIGHTEN_COLOR(      Tab,                Button, -20);
            COPY_COLOR(         TabHovered,         ButtonHovered);
            COPY_COLOR(         TabActive,          ButtonActive);
        } break;
    }
}

void DevTools::loadStyle() {
    if (this->m_bLoadedStyle) {
        return;
    }
    
    this->m_bLoadedStyle = true;

    auto style = &ImGui::GetStyle();
    auto colors = style->Colors;
    
    if (this->m_eMode == kDevToolsModeIntegrated) {
        style->WindowRounding    = 0.0f;
    } else {
        style->WindowRounding    = 2.0f;
    }
    style->ScrollbarRounding = 3.0f;
    style->GrabRounding      = 2.0f;
    style->AntiAliasedLines  = true;
    style->AntiAliasedFill   = true;
    style->WindowRounding    = 2;
    style->ChildRounding     = 2;
    style->ScrollbarSize     = 16;
    style->ScrollbarRounding = 3;
    style->GrabRounding      = 2;
    style->ItemSpacing.x     = 10;
    style->ItemSpacing.y     = 6;
    style->IndentSpacing     = 22;
    style->FramePadding.x    = 6;
    style->FramePadding.y    = 4;
    style->Alpha             = 1.0f;
    style->FrameRounding     = 3.0f;
    style->WindowPadding     = { 2.f, 2.f };

    this->loadTheme(this->m_eTheme);
}
