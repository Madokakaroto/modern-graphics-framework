#pragma once

namespace leaves { namespace pipeline 
{
	struct subresource
	{
		uint16_t	mip_level;
		uint16_t	array_index;
		uint16_t	width;
		uint16_t	height;
		uint16_t	depth;
		size_t		row_pitch;
		size_t		slice_pitch;
		byte*		pointer;
	};

	struct texture_meta
	{
		pixel_format			format;
		uint16_t				width;
		uint16_t				height;
		uint16_t				depth;
		uint16_t				array_size;
		bool					has_mipmap;
	};
} }