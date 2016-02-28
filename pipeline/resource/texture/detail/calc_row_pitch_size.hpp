#pragma once

namespace leaves { namespace pipeline
{
	namespace detail
	{
		auto calculate_row_pitch_size_bc1(pixel_format format, uint16_t width, uint16_t height)
			-> std::tuple<size_t, size_t>
		{
			assert(pixel_format::bc1 == format);
			size_t row = ceil_to_multiple_of_4(width) << 1;
			size_t pitch = row * ceil_to_multiple_of_4(height);
			return std::tie(row, pitch);
		}

		auto calculate_row_pitch_size_common(pixel_format format, uint16_t width, uint16_t height)
			->std::tuple<size_t, size_t>
		{
			assert(pixel_format::rgba_32 == format);
			size_t row = width * size_of(format);
			size_t pitch = row * height;
			return std::tie(row, pitch);
		}

		auto calculate_row_pitch_size(pixel_format format, uint16_t width, uint16_t height)
			-> std::tuple<size_t, size_t>
		{
			switch (format)
			{
			case pixel_format::bc1:
				return calculate_row_pitch_size_bc1(format, width, height);
			case pixel_format::rgba_32:
				return calculate_row_pitch_size_common(format, width, height);
			default:
				throw;
				return std::tuple<size_t, size_t>{0, 0};
			}
		}
	}
} }