#ifndef __SPY_AM_BMP_HPP__
#define __SPY_AM_BMP_HPP__

#include <iosfwd>
#include <vector>
#include <string>

namespace spy
{
    namespace am
    {
        bool load_bmp (std::istream& stream, std::vector <unsigned char>& out, unsigned& w, unsigned& h);
        bool load_bmp (const std::string& fname, std::vector <unsigned char>& out, unsigned& w, unsigned& h);
        bool load_bmp (const std::wstring& fname, std::vector <unsigned char>& out, unsigned& w, unsigned& h);

        bool convert_bmp (std::istream& stream, std::vector <unsigned char>& png);
        bool convert_bmp (const std::string& fname, std::vector <unsigned char>& png);
        bool convert_bmp (const std::wstring& fname, std::vector <unsigned char>& png);
    } // ns am
} // ns spy

#endif
