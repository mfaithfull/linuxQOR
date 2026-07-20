// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"

#include "terminal.h"

namespace qor{ namespace ui{

    Terminal::Terminal(){ }

    Terminal::~Terminal() noexcept = default;

    void Terminal::Setup(){ }

    void Terminal::Shutdown(){ }

    Dimensions Terminal::Size()
    {
        return Dimensions{80,24};
    }

    Dimensions Terminal::Fixed(int v) 
    {
        return {v, v};
    }

    Dimensions Terminal::Full() 
    {
        return Size();
    }

    Terminal::Colour TerminalColourSupport::ColourSupport() 
    {
        if (!m_cached) {
            m_cached = true;
            m_cached_supported_colour = ComputeColourSupport();
        }
        return m_cached_supported_colour;
    }

    const char* Safe(const char* c) 
    {
        return (c != nullptr) ? c : "";
    }

    bool Contains(const std::string& s, const char* key) 
    {
        return s.find(key) != std::string::npos;
    }

    Terminal::Colour TerminalColourSupport::ComputeColourSupport()
    {
        std::string COLORTERM = Safe(std::getenv("COLORTERM"));
        if (Contains(COLORTERM, "24bit") || Contains(COLORTERM, "truecolor")) 
        {
            return Terminal::Colour::TrueColour;
        }

        std::string TERM = Safe(std::getenv("TERM"));
        if (Contains(COLORTERM, "256") || Contains(TERM, "256")) 
        {
            return Terminal::Colour::Palette256;
        }

        // Microsoft terminals do not properly declare themselves supporting true
        // colors: https://github.com/microsoft/terminal/issues/1040
        // As a fallback, assume microsoft terminal are the ones not setting those
        // variables, and enable true colors.
        if (TERM.empty() && COLORTERM.empty()) 
        {
            return Terminal::Colour::TrueColour;
        }

        return Terminal::Colour::Palette16;
    }

}}//qor::ui
