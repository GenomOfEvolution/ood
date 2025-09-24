#pragma once
#include <string>
#include <regex>
#include <stdexcept>

namespace gfx
{
    struct Color
    {
        std::string m_data;

        Color(const std::string& color) : m_data(color)
        {
            if (!IsValidColor(color)) 
            {
                throw std::invalid_argument("Invalid color format");
            }
        }

        Color() { m_data = "#000000"; };

    private:
        bool IsValidColor(const std::string& color) const
        {
            std::regex colorRegex("^#[0-9A-Fa-f]{6}$");
            return std::regex_match(color, colorRegex);
        }
    };
}