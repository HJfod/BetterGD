#pragma once

#include <BGDUtils.hpp>

USE_BGD_NAMESPACE();

struct Point {
    int x = 0;
    int y = 0;

    Point& operator+(Point const& pos) {
        this->x += pos.x;
        this->y += pos.y;
        return *this;
    }
};

struct Size {
    int width = 0;
    int height = 0;
};

class NativeUIElementBase {
    protected:
        inline static unsigned int s_uniqueID = 0x4000;

        Point m_pos;
        Size  m_size;
        std::vector<NativeUIElementBase*> m_children;
        NativeUIElementBase* m_parent = nullptr;
        bool m_visible = true;
        unsigned int m_uniqueID = s_uniqueID;

        virtual void update_pos(int x, int y)  {}
        virtual void update_size(int w, int h) {}
        virtual void update_visibility(bool v) {}

    public:
        void attach(NativeUIElementBase* element);
        void detach(NativeUIElementBase* element, bool free = true);

        std::vector<NativeUIElementBase*> children() const;
        NativeUIElementBase* parent() const;

        void  pos(Point pos);
        void  pos(int x, int y);
        Point pos() const;
        Point real_pos() const;

        void size(Size size);
        void size(int w, int h);
        Size size() const;

        bool visible() const;
        void show();
        void hide();
        virtual void render();

        NativeUIElementBase* element_by_unique_id(unsigned int id);

        NativeUIElementBase(NativeUIElementBase*);
        virtual ~NativeUIElementBase();
};

class INativeUIColorableBase {
    protected:
        ccColor3B m_color;
    
        virtual void update_color(ccColor3B const& color) {}
    
    public:
        void color(ccColor3B const& color);
        ccColor3B color() const;
};

class INativeUITextableBase {
    protected:
        std::string m_text;

        virtual void update_text(std::string const& text) {}
    
    public:
        void text(std::string const& text);
        std::string text() const;
};

#include "NativeUIElementWin32.hpp"
