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
#include <exception>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

#include "alps/fits.h"
#include "engine.h"

using namespace ALPS;

int main()
{
    std::cout << "AlpX Copyright (C) 2026 Stefano De Angelis" << '\n';

    Engine engine = {};

    std::string cmd;
    std::string token;
    while (std::getline(std::cin, cmd))
    {
        token.clear();
        std::istringstream iss(cmd);
        iss >> token;

        if (token == "quit" || token == "exit") { break; }

        else if (token == "open")
        {
            Path path;

            if (!(iss >> path))
            {
                std::cerr << "\033[38;5;208mUsage: open <path>\033[0m\n";
                continue;
            }

            try
            {
                auto start = std::chrono::steady_clock::now();
                engine.open_fits(path);
                auto end = std::chrono::steady_clock::now();

                float elapsed = std::chrono::duration<float, std::milli>(end - start).count();
                std::cout << "\033[32mOpened FITS file in " << elapsed << " ms\033[0m\n";
            }
            catch(const std::exception& e)
            {
                std::cerr << "\033[31mError: " << e.what() << "\033[0m\n";
            }
        }

        else if (token == "close")
        {
            engine.close_fits();
        }

        else if (token == "load")
        {
            if (iss >> token)
            {
                if (token == "header")
                {
                    Path path;

                    if (iss >> path)
                    {
                        engine.load_header(path);
                        std::cout << "Loaded header from path\n";
                    }
                    else
                    {
                        engine.load_header();
                        std::cout << "Loaded header from opened file\n";
                    }
                }
            }
        }

        else if (token == "header")
        {
            Path path;
            std::optional<FitsFile> requested_file;
            FitsFile* fits_file = nullptr;

            if (iss >> path)
            {
                requested_file.emplace(path);
                fits_file = &*requested_file;
            }
            else if (engine.has_open_fits())
            {
                fits_file = &engine.fits();
            }
            else
            {
                std::cerr << "\033[31mNo FITS file is currently open: please provide a path!\033[0m\n";
                continue;
            }

            auto start = std::chrono::steady_clock::now();

            Header header = parse_header(*fits_file);

            auto end = std::chrono::steady_clock::now();

            float elapsed = std::chrono::duration<float, std::milli>(end - start).count();

            for (const Card& card : header.cards())
            {
                std::cout << card.keyword() << " = " << card.value() << '\n';
            }

            std::cout << "\n\033[32mParsed FITS header in " << elapsed << " ms\033[0m\n";
        }

        else if (token == "help")
        {
            std::cout
                << "COMMANDS:\n"
                << "    help                Displays this help message\n"
                << "    open <path>         Opens a FITS file\n"
                << "    close               Closes the current file\n"
                << "    header [path]       Displays the FITS header\n"
                << "    quit, exit          Exits AlpX\n";
        }

        else
        {
            std::cout << "\033[31mUnrecognized command \'" << cmd << "\'.\033[0m\n";
        }
    }

    return 0;
}