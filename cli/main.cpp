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

#include <iostream>
#include <sstream>
#include <string>

#include "alps/fits.h"

using namespace ALPS;

int main()
{
    std::cout << "AlpX Copyright (C) 2026 Stefano De Angelis" << '\n';

    std::string cmd;
    std::string token;
    while (std::getline(std::cin, cmd))
    {
        token.clear();
        std::istringstream iss(cmd);
        iss >> token;

        if (token == "quit" || token == "exit") { break; }

        if (token == "header")
        {
            Path path;

            if (!(iss >> path)) { std::cerr << "Could not parse path"; }

            FITS::Header header = FITS::parse_header(path);

            for (FITS::Card card : header)
            {
                std::cout << card.keyword << " = " << card.value << '\n';
            }
        }
    }

    return 0;
}