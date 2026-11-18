/*
  AlpX, an open-source command-line tool for interfacing with the ALPS library
  Copyright (C) 2026 Stefano De Angelis

  AlpX is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  AlpX is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef ALPX_ENGINE_H
#define ALPX_ENGINE_H

#include <optional>

#include "alps/fits.h"

namespace ALPS
{

class Engine
{
public:
    Engine() = default;

    void open_fits(const Path& path) { _fits.emplace(path); }
    void close_fits() { _fits.reset(); }

    void load_header();
    void load_header(const Path& path);

    [[nodiscard]] FitsFile& fits() noexcept { return *_fits; }

    [[nodiscard]] bool has_open_fits() const noexcept { return _fits.has_value(); }

private:
    std::optional<FitsFile> _fits;
};

} // namespace ALPS

#endif // #ifndef ALPX_ENGINE_H