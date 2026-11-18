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
#include <fitsio.h>
#include <longnam.h>
#include <stdexcept>

namespace ALPS
{

    FitsFile::FitsFile(const Path& path)
    {
        int status = 0;

        fits_open_file(&_fptr, path.string().c_str(), READONLY, &status);

        if (status)
        {
            char error[FLEN_STATUS] = {};
            fits_get_errstatus(status, error);

            throw std::runtime_error(std::string("Failed to open FITS file: ") + error);
        }
    }

    FitsFile::~FitsFile()
    {
        if (_fptr)
        {
            int status = 0;
            fits_close_file(_fptr, &status);
        }
    }

    Header parse_header(const FitsFile& fits_file)
    {
        fitsfile* fptr = fits_file.fptr();

        int status = 0;
        int n_keys = 0;
        int a_keys = 0;

        fits_get_hdrspace(fptr, &n_keys, &a_keys, &status);

        if (status)
        {
            throw std::runtime_error("Failed to read FITS header size");
        }

        std::vector<Card> cards;

        for (int i = 1; i <= n_keys; ++i)
        {
            char keyword[FLEN_KEYWORD] = {};
            char value[FLEN_VALUE] = {};
            char comment[FLEN_COMMENT] = {};

            fits_read_keyn(fptr, i, keyword, value, comment, &status);

            if (status)
            {
                throw std::runtime_error("Failed to read FITS header card");
            }

            cards.emplace_back(keyword, value, comment);
        }

        return Header(std::move(cards));
    }

    Header parse_header(const Path& path)
    {
        return parse_header(FitsFile(path));
    }

    Data parse_data(const FitsFile &fits_file)
    {
        fitsfile* fptr = fits_file.fptr();

        int status = 0;
        int dimensions = 0;

        fits_get_img_dim(fptr, &dimensions, &status);

        if (status) throw std::runtime_error("Failed to read FITS image dimensions");

        long axes[3] = {1, 1, 1};

        fits_get_img_size(fptr, 3, axes, &status);
        
        if (status) throw std::runtime_error("Failed to read FITS image size");

        Data data;

        data.width() = axes[0];
        data.height() = axes[1];
        data.channels() = axes[2];
        
        std::size_t pixel_num = axes[0] * axes[1] * axes[2];

        data.data().resize(pixel_num);

        long first_pixel[3] = {1, 1, 1};
        int any_null = 0;

        fits_read_pix(
            fptr,
            TFLOAT,
            first_pixel,
            static_cast<long>(pixel_num),
            nullptr,
            data.data().data(),
            &any_null,
            &status
        );

        if (status) throw std::runtime_error("Failed to read FITS image data");

        return data;
    }

} // namespace ALPS