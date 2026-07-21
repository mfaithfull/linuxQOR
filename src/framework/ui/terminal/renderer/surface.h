// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_UI_TERMINAL_RENDERER_SURFACE
#define QOR_PP_H_FRAMEWORK_UI_TERMINAL_RENDERER_SURFACE

#include <memory>
#include <vector>
#include <string>
#include <functional>

#include "image.h"

namespace qor{ namespace ui{ namespace tty {

    class qor_pp_module_interface(QOR_TERMINAL) Surface : public Image 
    {
    public:

        Surface(int dimx, int dimy);        
        static Surface Create(int width, int height);

        virtual ~Surface() override = default;

        std::string ToString() const;

        // Print the Screen on to the terminal.
        void Print() const;

        // Fill the screen with space and reset any screen state, like hyperlinks, and cursor
        void Clear();

        // Move the terminal cursor n-lines up with n = dimy().
        std::string ResetPosition(bool clear = false) const;

        void ApplyShader();

        struct Cursor 
        {
            int x = 0;
            int y = 0;

            enum Shape 
            {
                Hidden = 0,
                BlockBlinking = 1,
                Block = 2,
                UnderlineBlinking = 3,
                Underline = 4,
                BarBlinking = 5,
                Bar = 6,
            };
            Shape shape = Hidden;
        };

        Cursor cursor() const { return cursor_; }
        void SetCursor(Cursor cursor) { cursor_ = cursor; }

        // Store an hyperlink in the screen. Return the id of the hyperlink. The id is
        // used to identify the hyperlink when the user click on it.
        uint8_t RegisterHyperlink(std::string_view link);
        const std::string& Hyperlink(uint8_t id) const;

        using SelectionStyle = std::function<void(Pixel&)>;
        const SelectionStyle& GetSelectionStyle() const;
        void SetSelectionStyle(SelectionStyle decorator);

    protected:
    
        Cursor cursor_;
        std::vector<std::string> hyperlinks_ = {""};

        // The current selection style. This is overridden by various dom elements.
        SelectionStyle selection_style_ = [](Pixel& pixel) 
        {
            pixel.inverted ^= true;
        };
    };

}}}//qor::ui::tty

#endif//QOR_PP_H_FRAMEWORK_UI_TERMINAL_RENDERER_SURFACE

