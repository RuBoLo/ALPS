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

#include "alps/fits.h"

#include "alps/types.h"

namespace ALPS
{

namespace FITS
{

    Header parse_header(std::ifstream& file)
    {
        if (!file)
        {
            throw;
        }

        Header header;

        std::size_t header_bytes = 0;

        while (true)
        {
            char card[card_size];

            if (!file.read(card, card_size))
            {
                throw;
            }

            std::string keyword(card, 8);

            if (!trim(keyword)) { continue; }

            if (keyword == "END") { break; }

            std::string value(card + 10);

            header_bytes += card_size;

            header.data.emplace_back(keyword, value, "");
        }

        std::size_t padding = (block_size - (header_bytes % block_size)) % block_size;
        file.seekg(static_cast<std::streamoff>(padding), std::ios::cur);

        return header;
    }

    Header parse_header(const Path& path)
    {
        std::ifstream file(path, std::ios::binary);
        return parse_header(file);
    }

} // namespace FITS

} // namespace ALPS