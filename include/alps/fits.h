/*
  ALPS, an open-source library for astrophotographic image processing
  Copyright (C) 2026 Stefano De Angelis

  ALPS is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  ALPS is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef ALPS_FITS_H
#define ALPS_FITS_H

#include <fitsio.h>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "alps/types.h"

namespace ALPS
{

class Card
{
public:
    Card(std::string keyword, std::string value, std::string comment) :
        _keyword(std::move(keyword)),
        _value(std::move(value)),
        _comment(std::move(comment))
    {}

    [[nodiscard]] const std::string& keyword() const noexcept { return _keyword; }
    [[nodiscard]] const std::string& value() const noexcept { return _value; }
    [[nodiscard]] const std::string& comment() const noexcept { return _comment; }

private:
    std::string _keyword;
    std::string _value;
    std::string _comment;
};

class Header
{
public:
    Header() = default;
    Header(std::vector<Card> cards) : _cards(std::move(cards)) {}

    [[nodiscard]] const std::vector<Card>& cards() const noexcept { return _cards; }

private:
    std::vector<Card> _cards;
};

class Data
{
public:
    [[nodiscard]] Pixels& data() noexcept { return _data; }
    [[nodiscard]] std::size_t& width() noexcept { return _width; }
    [[nodiscard]] std::size_t& height() noexcept { return _height; }
    [[nodiscard]] std::size_t& channels() noexcept { return _channels; }

private:
    Pixels _data;

    std::size_t _width;
    std::size_t _height;
    std::size_t _channels;
};

// CFITSIO wrapper

class FitsFile
{
public:
    explicit FitsFile(const Path& path);

    ~FitsFile();

    FitsFile(const FitsFile&) = delete;
    FitsFile& operator=(const FitsFile&) = delete;

    FitsFile(FitsFile&& other) noexcept : _fptr(std::exchange(other._fptr, nullptr)) {}

    FitsFile& operator=(FitsFile&& other) noexcept
    {
        if (this != &other)
        {
            if (_fptr)
            {
                int status = 0;
                fits_close_file(_fptr, &status);
            }

            _fptr = std::exchange(other._fptr, nullptr);
        }

        return *this;
    }

    [[nodiscard]] fitsfile* fptr() const noexcept { return _fptr; }

    [[nodiscard]] Header& header()
    {
        if (!_header) throw std::runtime_error("No header has been loaded");

        return *_header;
    }

    [[nodiscard]] Data& data()
    {
        if (!_data) throw std::runtime_error("No image data has been loaded");

        return *_data;
    }

    void set_header(Header header)
    {
        _header = std::move(header);
    }

    void set_data(Data data)
    {
        _data = std::move(data);
    }

private:
    fitsfile* _fptr = nullptr;

    std::optional<Header> _header;
    std::optional<Data> _data;
};

Header parse_header(const FitsFile& fits_file);
Header parse_header(const Path& path);

Data parse_data(const FitsFile& fits_file);
Data parse_data(const Path& path);

} // namespace ALPS

#endif // #ifndef ALPS_FITS_H