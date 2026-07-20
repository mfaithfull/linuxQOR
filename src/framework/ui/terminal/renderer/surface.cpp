// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <utility>

#include "surface.h"
#include "string.h"
#include "../terminal.h"

namespace qor{ namespace ui{ namespace tty {

    namespace 
    {
        void UpdatePixelStyle(const Surface* screen, std::stringstream& ss, const Pixel& prev, const Pixel& next) 
        {
            // See https://gist.github.com/egmontkob/eb114294efbcd5adb1944c9f3cb5feda
            if(next.hyperlink != prev.hyperlink)
            {
                ss << "\x1B]8;;" << screen->Hyperlink(next.hyperlink) << "\x1B\\";
            }

            // Bold
            if((next.bold ^ prev.bold) | (next.dim ^ prev.dim)) 
            {
                // BOLD_AND_DIM_RESET:
                ss << ((prev.bold && !next.bold) || (prev.dim && !next.dim) ? "\x1B[22m"
                                            : "");
                ss << (next.bold ? "\x1B[1m" : "");  // BOLD_SET
                ss << (next.dim ? "\x1B[2m" : "");   // DIM_SET
            }

            // Underline
            if(next.underlined != prev.underlined || next.underlined_double != prev.underlined_double)
            {
                ss << (next.underlined          ? "\x1B[4m"     // UNDERLINE
                : next.underlined_double ? "\x1B[21m"    // UNDERLINE_DOUBLE
                : "\x1B[24m");  // UNDERLINE_RESET
            }

            // Blink
            if(next.blink != prev.blink)
            {
                ss << (next.blink ? "\x1B[5m"     // BLINK_SET
                : "\x1B[25m");  // BLINK_RESET
            }

            // Inverted
            if(next.inverted != prev.inverted)
            {
                ss << (next.inverted ? "\x1B[7m"     // INVERTED_SET
                : "\x1B[27m");  // INVERTED_RESET
            }

            // Italics
            if(next.italic != prev.italic) 
            {
                ss << (next.italic ? "\x1B[3m"     // ITALIC_SET
                : "\x1B[23m");  // ITALIC_RESET
            }

            // StrikeThrough
            if(next.strikethrough != prev.strikethrough) 
            {
                ss << (next.strikethrough ? "\x1B[9m"     // CROSSED_OUT
                : "\x1B[29m");  // CROSSED_OUT_RESET
            }

            if(next.foreground_color != prev.foreground_color || next.background_color != prev.background_color) 
            {
                ss << "\x1B[" + next.foreground_color.Print(false) + "m";
                ss << "\x1B[" + next.background_color.Print(true) + "m";
            }
        }

        struct TileEncoding 
        {
            std::uint8_t left : 2;
            std::uint8_t top : 2;
            std::uint8_t right : 2;
            std::uint8_t down : 2;
            std::uint8_t round : 1;

            bool operator<(const TileEncoding& other) const 
            {
                if (left < other.left)   { return  true;  }
                if (left > other.left)   { return  false; }
                if (top < other.top)     { return  true;  }
                if (top > other.top)     { return  false; }
                if (right < other.right) { return  true;  }
                if (right > other.right) { return  false; }
                if (down < other.down)   { return  true;  }
                if (down > other.down)   { return  false; }
                if (round < other.round) { return  true;  }
                if (round > other.round) { return  false; }
                return false;
            }
        };


        const std::map<std::string, TileEncoding> tile_encoding = {
            {"─", {1, 0, 1, 0, 0}},
            {"━", {2, 0, 2, 0, 0}},
            {"╍", {2, 0, 2, 0, 0}},

            {"│", {0, 1, 0, 1, 0}},
            {"┃", {0, 2, 0, 2, 0}},
            {"╏", {0, 2, 0, 2, 0}},

            {"┌", {0, 0, 1, 1, 0}},
            {"┍", {0, 0, 2, 1, 0}},
            {"┎", {0, 0, 1, 2, 0}},
            {"┏", {0, 0, 2, 2, 0}},

            {"┐", {1, 0, 0, 1, 0}},
            {"┑", {2, 0, 0, 1, 0}},
            {"┒", {1, 0, 0, 2, 0}},
            {"┓", {2, 0, 0, 2, 0}},

            {"└", {0, 1, 1, 0, 0}},
            {"┕", {0, 1, 2, 0, 0}},
            {"┖", {0, 2, 1, 0, 0}},
            {"┗", {0, 2, 2, 0, 0}},

            {"┘", {1, 1, 0, 0, 0}},
            {"┙", {2, 1, 0, 0, 0}},
            {"┚", {1, 2, 0, 0, 0}},
            {"┛", {2, 2, 0, 0, 0}},

            {"├", {0, 1, 1, 1, 0}},
            {"┝", {0, 1, 2, 1, 0}},
            {"┞", {0, 2, 1, 1, 0}},
            {"┟", {0, 1, 1, 2, 0}},
            {"┠", {0, 2, 1, 2, 0}},
            {"┡", {0, 2, 2, 1, 0}},
            {"┢", {0, 1, 2, 2, 0}},
            {"┣", {0, 2, 2, 2, 0}},

            {"┤", {1, 1, 0, 1, 0}},
            {"┥", {2, 1, 0, 1, 0}},
            {"┦", {1, 2, 0, 1, 0}},
            {"┧", {1, 1, 0, 2, 0}},
            {"┨", {1, 2, 0, 2, 0}},
            {"┩", {2, 2, 0, 1, 0}},
            {"┪", {2, 1, 0, 2, 0}},
            {"┫", {2, 2, 0, 2, 0}},

            {"┬", {1, 0, 1, 1, 0}},
            {"┭", {2, 0, 1, 1, 0}},
            {"┮", {1, 0, 2, 1, 0}},
            {"┯", {2, 0, 2, 1, 0}},
            {"┰", {1, 0, 1, 2, 0}},
            {"┱", {2, 0, 1, 2, 0}},
            {"┲", {1, 0, 2, 2, 0}},
            {"┳", {2, 0, 2, 2, 0}},

            {"┴", {1, 1, 1, 0, 0}},
            {"┵", {2, 1, 1, 0, 0}},
            {"┶", {1, 1, 2, 0, 0}},
            {"┷", {2, 1, 2, 0, 0}},
            {"┸", {1, 2, 1, 0, 0}},
            {"┹", {2, 2, 1, 0, 0}},
            {"┺", {1, 2, 2, 0, 0}},
            {"┻", {2, 2, 2, 0, 0}},

            {"┼", {1, 1, 1, 1, 0}},
            {"┽", {2, 1, 1, 1, 0}},
            {"┾", {1, 1, 2, 1, 0}},
            {"┿", {2, 1, 2, 1, 0}},
            {"╀", {1, 2, 1, 1, 0}},
            {"╁", {1, 1, 1, 2, 0}},
            {"╂", {1, 2, 1, 2, 0}},
            {"╃", {2, 2, 1, 1, 0}},
            {"╄", {1, 2, 2, 1, 0}},
            {"╅", {2, 1, 1, 2, 0}},
            {"╆", {1, 1, 2, 2, 0}},
            {"╇", {2, 2, 2, 1, 0}},
            {"╈", {2, 1, 2, 2, 0}},
            {"╉", {2, 2, 1, 2, 0}},
            {"╊", {1, 2, 2, 2, 0}},
            {"╋", {2, 2, 2, 2, 0}},

            {"═", {3, 0, 3, 0, 0}},
            {"║", {0, 3, 0, 3, 0}},

            {"╒", {0, 0, 3, 1, 0}},
            {"╓", {0, 0, 1, 3, 0}},
            {"╔", {0, 0, 3, 3, 0}},

            {"╕", {3, 0, 0, 1, 0}},
            {"╖", {1, 0, 0, 3, 0}},
            {"╗", {3, 0, 0, 3, 0}},

            {"╘", {0, 1, 3, 0, 0}},
            {"╙", {0, 3, 1, 0, 0}},
            {"╚", {0, 3, 3, 0, 0}},

            {"╛", {3, 1, 0, 0, 0}},
            {"╜", {1, 3, 0, 0, 0}},
            {"╝", {3, 3, 0, 0, 0}},

            {"╞", {0, 1, 3, 1, 0}},
            {"╟", {0, 3, 1, 3, 0}},
            {"╠", {0, 3, 3, 3, 0}},

            {"╡", {3, 1, 0, 1, 0}},
            {"╢", {1, 3, 0, 3, 0}},
            {"╣", {3, 3, 0, 3, 0}},

            {"╤", {3, 0, 3, 1, 0}},
            {"╥", {1, 0, 1, 3, 0}},
            {"╦", {3, 0, 3, 3, 0}},

            {"╧", {3, 1, 3, 0, 0}},
            {"╨", {1, 3, 1, 0, 0}},
            {"╩", {3, 3, 3, 0, 0}},

            {"╪", {3, 1, 3, 1, 0}},
            {"╫", {1, 3, 1, 3, 0}},
            {"╬", {3, 3, 3, 3, 0}},

            {"╭", {0, 0, 1, 1, 1}},
            {"╮", {1, 0, 0, 1, 1}},
            {"╯", {1, 1, 0, 0, 1}},
            {"╰", {0, 1, 1, 0, 1}},

            {"╴", {1, 0, 0, 0, 0}},
            {"╵", {0, 1, 0, 0, 0}},
            {"╶", {0, 0, 1, 0, 0}},
            {"╷", {0, 0, 0, 1, 0}},

            {"╸", {2, 0, 0, 0, 0}},
            {"╹", {0, 2, 0, 0, 0}},
            {"╺", {0, 0, 2, 0, 0}},
            {"╻", {0, 0, 0, 2, 0}},

            {"╼", {1, 0, 2, 0, 0}},
            {"╽", {0, 1, 0, 2, 0}},
            {"╾", {2, 0, 1, 0, 0}},
            {"╿", {0, 2, 0, 1, 0}},
        };

        
        template <class A, class B>
        std::map<B, A> InvertMap(const std::map<A, B> input) 
        {
            std::map<B, A> output;
            for (const auto& it : input) 
            {
                output[it.second] = it.first;
            }
            return output;
        }

        const std::map<TileEncoding, std::string> tile_encoding_inverse = InvertMap(tile_encoding);

        void UpgradeLeftRight(std::string& left, std::string& right) 
        {
            const auto it_left = tile_encoding.find(left);
            if (it_left == tile_encoding.end()) 
            {
                return;
            }
            const auto it_right = tile_encoding.find(right);
            if (it_right == tile_encoding.end()) 
            {
                return;
            }

            if (it_left->second.right == 0 && it_right->second.left != 0) 
            {
                TileEncoding encoding_left = it_left->second;
                encoding_left.right = it_right->second.left;
                const auto it_left_upgrade = tile_encoding_inverse.find(encoding_left);
                if (it_left_upgrade != tile_encoding_inverse.end()) 
                {
                    left = it_left_upgrade->second;
                }
            }

            if (it_right->second.left == 0 && it_left->second.right != 0) 
            {
                TileEncoding encoding_right = it_right->second;
                encoding_right.left = it_left->second.right;
                const auto it_right_upgrade = tile_encoding_inverse.find(encoding_right);
                if (it_right_upgrade != tile_encoding_inverse.end()) 
                {
                    right = it_right_upgrade->second;
                }
            }
        }

        void UpgradeTopDown(std::string& top, std::string& down) 
        {
            const auto it_top = tile_encoding.find(top);
            if (it_top == tile_encoding.end()) 
            {
                return;
            }
            const auto it_down = tile_encoding.find(down);
            if (it_down == tile_encoding.end()) 
            {
                return;
            }

            if (it_top->second.down == 0 && it_down->second.top != 0) 
            {
                TileEncoding encoding_top = it_top->second;
                encoding_top.down = it_down->second.top;
                const auto it_top_down = tile_encoding_inverse.find(encoding_top);
                if (it_top_down != tile_encoding_inverse.end()) 
                {
                    top = it_top_down->second;
                }
            }

            if (it_down->second.top == 0 && it_top->second.down != 0) 
            {
                TileEncoding encoding_down = it_down->second;
                encoding_down.top = it_top->second.down;
                const auto it_down_top = tile_encoding_inverse.find(encoding_down);
                if (it_down_top != tile_encoding_inverse.end()) 
                {
                    down = it_down_top->second;
                }
            }
        }

        bool ShouldAttemptAutoMerge(Pixel& pixel) 
        {
            return pixel.automerge && pixel.character.size() == 3;
        }

    }//

    // Create a screen with the given dimension along the x-axis and y-axis.
    Surface Surface::Create(Dimensions width, Dimensions height) 
    {
        return {width.dimx, height.dimy};
    }

    Surface Surface::Create(Dimensions dimension) 
    {
        return {dimension.dimx, dimension.dimy};
    }

    Surface::Surface(int dimx, int dimy) : Image{dimx, dimy} { }

    // Produce a std::string that can be used to print the Screen on the terminal.
    //Don't forget to flush stdout. Alternatively, you can use Screen::Print();
    std::string Surface::ToString() const 
    {
        std::stringstream ss;

        const Pixel default_pixel;
        const Pixel* previous_pixel_ref = &default_pixel;

        for (int y = 0; y < dimy_; ++y) 
        {
            // New line in between two lines.
            if (y != 0) 
            {
                UpdatePixelStyle(this, ss, *previous_pixel_ref, default_pixel);
                previous_pixel_ref = &default_pixel;
                ss << "\r\n";
            }

            // After printing a fullwith character, we need to skip the next cell.
            bool previous_fullwidth = false;
            for (const auto& pixel : pixels_[y]) 
            {
                if (!previous_fullwidth) 
                {
                    UpdatePixelStyle(this, ss, *previous_pixel_ref, pixel);
                    previous_pixel_ref = &pixel;
                    if (pixel.character.empty()) 
                    {
                        ss << " ";
                    } 
                    else 
                    {
                        ss << pixel.character;
                    }
                }
                previous_fullwidth = (string_width(pixel.character) == 2);
            }
        }

        // Reset the style to default:
        UpdatePixelStyle(this, ss, *previous_pixel_ref, default_pixel);

        return ss.str();
    }

    // Print the Screen to the terminal.
    void Surface::Print() const 
    {
        std::cout << ToString() << '\0' << std::flush;
    }

/// Return a string to be printed in order to reset the cursor position
///        to the beginning of the screen.
///
/// ```cpp
/// std::string reset_position;
/// while(true) {
///   auto document = render();
///   auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
///   Render(screen, document);
///   std::cout << reset_position << screen.ToString() << std::flush;
///   reset_position = screen.ResetPosition();
///
///   using namespace std::chrono_literals;
///   std::this_thread::sleep_for(0.01s);
/// }
/// ```
///
/// @return The string to print in order to reset the cursor position to the
///         beginning.
    std::string Surface::ResetPosition(bool clear) const 
    {
        std::stringstream ss;
        if (clear) 
        {
            ss << "\r";       // MOVE_LEFT;
            ss << "\x1b[2K";  // CLEAR_SCREEN;
            for (int y = 1; y < dimy_; ++y) 
            {
                ss << "\x1B[1A";  // MOVE_UP;
                ss << "\x1B[2K";  // CLEAR_LINE;
            }
        } 
        else 
        {
            ss << "\r";  // MOVE_LEFT;
            for (int y = 1; y < dimy_; ++y) 
            {
                ss << "\x1B[1A";  // MOVE_UP;
            }
        }
        return ss.str();
    }

    void Surface::Clear() 
    {
        Image::Clear();

        cursor_.x = dimx_ - 1;
        cursor_.y = dimy_ - 1;

        hyperlinks_ = 
        {
            "",
        };
    }

    void Surface::ApplyShader() 
    {
        // Merge box characters togethers.
        for (int y = 0; y < dimy_; ++y) 
        {
            for (int x = 0; x < dimx_; ++x) 
            {
                // Box drawing character uses exactly 3 byte.
                Pixel& cur = pixels_[y][x];
                if (!ShouldAttemptAutoMerge(cur)) {
                    continue;
                }

                if (x > 0) 
                {
                    Pixel& left = pixels_[y][x-1];
                    if (ShouldAttemptAutoMerge(left)) 
                    {
                        UpgradeLeftRight(left.character, cur.character);
                    }
                }
                if (y > 0) 
                {
                    Pixel& top = pixels_[y-1][x];
                    if (ShouldAttemptAutoMerge(top)) 
                    {
                        UpgradeTopDown(top.character, cur.character);
                    }
                }
            }
        }
    }


    std::uint8_t Surface::RegisterHyperlink(std::string_view link) 
    {
        for (std::size_t i = 0; i < hyperlinks_.size(); ++i) 
        {
            if (hyperlinks_[i] == link) 
            {
                return static_cast<uint8_t>(i);
            }
        }
        if (hyperlinks_.size() == std::numeric_limits<std::uint8_t>::max()) 
        {
            return 0;
        }
        hyperlinks_.push_back(std::string(link));
        return static_cast<uint8_t>(hyperlinks_.size() - 1);
    }

    const std::string& Surface::Hyperlink(std::uint8_t id) const 
    {
        if (id >= hyperlinks_.size()) 
        {
            return hyperlinks_[0];
        }
        return hyperlinks_[id];
    }

    //Return the current selection style.
    const Surface::SelectionStyle& Surface::GetSelectionStyle() const 
    {
        return selection_style_;
    }

    //Set the current selection style.
    void Surface::SetSelectionStyle(SelectionStyle decorator) 
    {
        selection_style_ = std::move(decorator);
    }

}}}//qor::ui::tty
