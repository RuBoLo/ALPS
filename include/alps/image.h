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

#ifndef ALPS_IMAGE_H
#define ALPS_IMAGE_H

#include <cstddef>

#include "alps/types.h"

namespace ALPS
{

class Image
{
public:
    Image() = default;
    Image(const std::size_t width, const std::size_t height, const std::size_t channels) :
        _data(width * height * channels),
        _width(width),
        _height(height),
        _channels(channels)
    {}

    [[nodiscard]] Pixels data() const noexcept { return _data; }
    [[nodiscard]] std::size_t width() const noexcept { return _width; }
    [[nodiscard]] std::size_t height() const noexcept { return _height; }
    [[nodiscard]] std::size_t channels() const noexcept { return _channels; }

private:
    Pixels _data;
    std::size_t _width;
    std::size_t _height;
    std::size_t _channels;

};

} // namespace ALPS

#endif // #ifndef ALPS_IMAGE_H