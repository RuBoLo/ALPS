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

#include <chrono>
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

        else if (token == "header")
        {
            Path path;

            if (!(iss >> path)) { std::cerr << "Could not parse path"; }

            auto start = std::chrono::steady_clock::now();
            FITS::Header header = FITS::parse_header(path);
            auto end = std::chrono::steady_clock::now();

            float elapsed = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1000000.0f);

            for (FITS::Card card : header.data)
            {
                std::cout << card.keyword << " = " << card.value << '\n';
            }

            std::cout << "\nParsed FITS header in " << elapsed << " ms\n";
        }

        else
        {
            std::cout << "\033[31mUnrecognized command \'" << cmd << "\'.\033[0m\n\n";
        }
    }

    return 0;
}