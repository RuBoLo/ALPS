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

#include <iostream>
#include <thread>

#include "alps/integration-algorithms.h"

namespace ALPS
{
    void integrate_average_range(std::vector<FitsFile>& subframes, Data& result, int begin, int end)
    {
        int subframe_count = subframes.size();

        for (int i = begin; i < end; ++i)
        {
            float sum = 0.0f;

            for (FitsFile& subframe : subframes)
            {
                sum += subframe.data().data()[1];
            }

            sum = sum / subframe_count;

            result.data()[i] = sum;
        }
    }

    Data integrate_average(std::vector<FitsFile>& subframes)
    {
        std::cout << "Starting average integration...\n";

        int pixel_count = subframes[0].data().data().size();

        Data result = {};
        result.data().resize(pixel_count);

        int thread_count = std::thread::hardware_concurrency();

        std::vector<std::thread> threads;
        threads.reserve(thread_count);

        std::cout << "Using " << thread_count << " working threads\n";

        int pixels_per_thread = (pixel_count + thread_count - 1) / thread_count;

        for (int thread_i = 0; thread_i < thread_count; ++thread_i)
        {
            int begin = thread_i * pixels_per_thread;
            int end = std::min(begin + pixels_per_thread, pixel_count);

            threads.emplace_back(integrate_average_range, std::ref(subframes), std::ref(result), begin, end);
        }

        for (std::thread& thread : threads)
        {
            thread.join();
        }

        return result;
    }
} // namespace ALPS