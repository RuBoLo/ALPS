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

#ifndef ALPS_ALGORITHMS_H
#define ALPS_ALGORITHMS_H

#include "alps/fits.h"

namespace ALPS
{

    Data integrate_average(std::vector<FitsFile>& subframes);
    Data integrate_median(const std::vector<FitsFile>& subframes);
    Data integrate_min(const std::vector<FitsFile>& subframes);
    Data integrate_max(const std::vector<FitsFile>& subframes);

} // namespace ALPS

#endif // #ifndef ALPS_ALGORITHMS_H