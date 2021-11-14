#pragma once

#define BGD_INCLUDE_IMGUI
#include "../../submodules/GL/glew.h"
#include "DevTools.hpp"
#include <BGDInternal.hpp>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static bool g_bRenderInSwapBuffers = false;

void CCEGLView_swapBuffers(CCEGLView* self) {
    static bool g_init = [self]() -> bool {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        auto& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigDockingWithShift = true;
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        DevTools::get()->initFonts();
        auto hwnd = WindowFromDC(*as<HDC*>(as<uintptr_t>(self->getWindow()) + 0x244));
        ImGui_ImplWin32_Init(hwnd);
        ImGui_ImplOpenGL3_Init();

        return true;
    }();

    if (g_bRenderInSwapBuffers) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        DevTools::get()->draw();

        ImGui::EndFrame();
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glFlush();
    }

    hook::orig<&CCEGLView_swapBuffers>(self);
}
static InternalCreateHook<&CCEGLView_swapBuffers>$ccevsb(
    "libcocos2d.dll",
    "?swapBuffers@CCEGLView@cocos2d@@UAEXXZ"
);

ImVec2 operator-(ImVec2 const& v1, ImVec2 const& v2) {
    return { v1.x - v2.x, v1.y - v2.y };
}

ImVec2 operator*(ImVec2 const& v1, float multi) {
    return { v1.x * multi, v1.y * multi };
}

bool operator!=(ImVec2 const& v1, ImVec2 const& v2) {
    return v1.x == v2.x && v1.y == v2.y;
}

void CCDirector_drawScene(CCDirector* self) {
    if (!DevTools::get()->shouldPopGame()) {
        g_bRenderInSwapBuffers = true;
        return hook::orig<&CCDirector_drawScene>(self);
    }
    g_bRenderInSwapBuffers = false;

    auto winSize = self->getOpenGLView()->getViewPortRect();

    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    GLuint framebuffer = 0;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // The texture we're going to render to
    GLuint renderedTexture;
    glGenTextures(1, &renderedTexture);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, renderedTexture);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(
        GL_TEXTURE_2D, 0,GL_RGB,
        static_cast<GLsizei>(winSize.size.width),
        static_cast<GLsizei>(winSize.size.height),
        0,GL_RGB, GL_UNSIGNED_BYTE, 0
    );

    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // The depth buffer
    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(
        GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
        static_cast<GLsizei>(winSize.size.width),
        static_cast<GLsizei>(winSize.size.height)
    );
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

    // Always check that our framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "oh no\n";
        hook::orig<&CCDirector_drawScene>(self);
        glDeleteRenderbuffers(1, &depthrenderbuffer);
        glDeleteTextures(1, &renderedTexture);
        glDeleteFramebuffers(1, &framebuffer);
        return;
    }

    // Render to our framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    hook::orig<&CCDirector_drawScene>(self);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    glClear(0x4100);

    DevTools::get()->draw();

    // if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    //     auto backup_current_context = self->getOpenGLView()->getWindow();
    //     ImGui::UpdatePlatformWindows();
    //     ImGui::RenderPlatformWindowsDefault();
    //     glfwMakeContextCurrent(backup_current_context);
    // }
    
    glFlush();

    if (ImGui::Begin("Geometry Dash")) {
        auto ratio = winSize.size.width / winSize.size.height;
        auto win = ImGui::GetMainViewport()->Size;
        ImVec2 imgSize = {
            (ImGui::GetWindowHeight() - 35) * ratio,
            (ImGui::GetWindowHeight() - 35)
        };
        if (ImGui::GetWindowWidth() - 20 < imgSize.x) {
            imgSize = {
                (ImGui::GetWindowWidth() - 20),
                (ImGui::GetWindowWidth() - 20) / ratio
            };
        }
        auto pos = (ImGui::GetWindowSize() - imgSize) * .5f;
        pos.y += 10.f;
        ImGui::SetCursorPos(pos);
        ImGui::Image(as<ImTextureID>(renderedTexture),
            imgSize, { 0, 1 }, { 1, 0 }
        );
        static bool   s_clicked = false;
        static ImVec2 s_lastPos = { -1.f, -1.f };
        if (ImGui::IsItemHovered()) {
            if (ImGui::GetMousePos() != s_lastPos) {
                s_lastPos = ImGui::GetMousePos();
                self->getOpenGLView()->onGLFWMouseMoveCallBack(
                    self->getOpenGLView()->getWindow(),
                    (ImGui::GetMousePos().x / win.x) * imgSize.x + pos.x,
                    (ImGui::GetMousePos().y / win.y) * imgSize.y + pos.y
                );
            }
            if (s_clicked != ImGui::IsMouseDown(0)) {
                self->getOpenGLView()->onGLFWMouseCallBack(
                    self->getOpenGLView()->getWindow(), 0, s_clicked, 0
                );
            }
        }
    }
    ImGui::End();

    ImGui::EndFrame();
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteRenderbuffers(1, &depthrenderbuffer);
    glDeleteTextures(1, &renderedTexture);
    glDeleteFramebuffers(1, &framebuffer);
}
static InternalCreateHook<&CCDirector_drawScene>$ccdds(
    "libcocos2d.dll",
    "?drawScene@CCDirector@cocos2d@@QAEXXZ"
);

void CCEGLView_pollEvents(CCEGLView* self) {
    auto& io = ImGui::GetIO();

    bool blockInput = false;
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);

        if (io.WantCaptureMouse) {
            switch(msg.message) {
                case WM_LBUTTONDBLCLK:
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                case WM_MBUTTONDBLCLK:
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:
                case WM_MOUSEACTIVATE:
                case WM_MOUSEHOVER:
                case WM_MOUSEHWHEEL:
                case WM_MOUSELEAVE:
                case WM_MOUSEMOVE:
                case WM_MOUSEWHEEL:
                case WM_NCLBUTTONDBLCLK:
                case WM_NCLBUTTONDOWN:
                case WM_NCLBUTTONUP:
                case WM_NCMBUTTONDBLCLK:
                case WM_NCMBUTTONDOWN:
                case WM_NCMBUTTONUP:
                case WM_NCMOUSEHOVER:
                case WM_NCMOUSELEAVE:
                case WM_NCMOUSEMOVE:
                case WM_NCRBUTTONDBLCLK:
                case WM_NCRBUTTONDOWN:
                case WM_NCRBUTTONUP:
                case WM_NCXBUTTONDBLCLK:
                case WM_NCXBUTTONDOWN:
                case WM_NCXBUTTONUP:
                case WM_RBUTTONDBLCLK:
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                case WM_XBUTTONDBLCLK:
                case WM_XBUTTONDOWN:
                case WM_XBUTTONUP:
                    blockInput = true;
            }
        }

        if (io.WantCaptureKeyboard) {
            switch(msg.message) {
                case WM_HOTKEY:
                case WM_KEYDOWN:
                case WM_KEYUP:
                case WM_KILLFOCUS:
                case WM_SETFOCUS:
                case WM_SYSKEYDOWN:
                case WM_SYSKEYUP:
                    blockInput = true;
            }
        }

        if (!blockInput) {
            DispatchMessage(&msg);
        }

        ImGui_ImplWin32_WndProcHandler(msg.hwnd, msg.message, msg.wParam, msg.lParam);
    }

    hook::orig<CCEGLView_pollEvents>(self);
}
static InternalCreateHook<&CCEGLView_pollEvents>$ccevpe(
    "libcocos2d.dll",
    "?pollEvents@CCEGLView@cocos2d@@QAEXXZ"
);
