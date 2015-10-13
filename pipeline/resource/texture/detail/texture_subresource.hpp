#pragma once

namespace leaves { namespace pipeline 
{
	uint16_t calculate_mipmap_level(uint16_t width, uint16_t height, uint16_t depth)
	{
		auto mipmap_level = std::max<uint16_t>(log2_integral(width), log2_integral(height));
		mipmap_level = std::max<uint16_t>(log2_integral(depth), mipmap_level);
		return mipmap_level;
	}

	// this is a complex function
	auto calculate_pitch_size(pixel_format format, uint16_t width, uint16_t height)
		-> std::tuple<size_t, size_t>
	{
		size_t row = 0, slice = 0;
		
		// to be implemented

		return std::tie(row, slice);
	}

	// 与texture3D的不同
	// 1. 除了row_pitch 与 slice_pitch之外，还要考虑depth
	class texture_subresource
	{
	public:

		explicit texture_subresource(texture_meta const& meta)
		{
			construct(meta.format, meta.width, meta.height, meta.depth, meta.array_size, meta.has_mipmap);
		}

		void construct(pixel_format format, uint16_t width, uint16_t height, uint16_t depth, uint16_t array_size, bool mipmap)
		{
			// 没有texture3D array
			if (depth > 1 && array_size > 1)
				throw std::exception();

			// 不支持超过15层的mipmap，原始像素太大
			auto mip_levels = mipmap ? uint16_t{ 0 } : calculate_mipmap_level(width, height, depth);
			if (mip_levels > 15)
				throw std::exception();

			auto subresource_count = (mip_levels + 1) * array_size;
			subresources_.resize(subresource_count);

			size_t texture_size = 0;
			auto loop = size_t{ 0 };
			for (uint16_t array_index = 0; array_index < array_size; ++array_index)
			{
				for (uint16_t mip_level = 0; mip_level <= mip_levels; ++mip_level)
				{
					auto& subres = subresources_[loop++];
					subres.mip_level = mip_level;
					subres.array_index = array_index;
					subres.width  = std::max<uint16_t>(width >> mip_level, 1);
					subres.height = std::max<uint16_t>(height >> mip_level, 1);
					subres.depth  = std::max<uint16_t>(depth >> mip_level, 1);
					std::tie(subres.row_pitch, subres.slice_pitch) =
						calculate_pitch_size(format, subres.width, subres.height);

					texture_size += subres.slice_pitch * depth;
				}
			}
			
			data_.resize(texture_size);
			auto ptr = data_.data();
			for (auto& sub_res : subresources_)
			{
				sub_res.pointer = ptr;
				ptr += sub_res.slice_pitch * sub_res.depth;
			}
		}

		auto move_data() noexcept -> std::vector<byte>&& 
		{
			return std::move(data_);
		}

		auto move_subres() noexcept -> std::vector<subresource>&&
		{
			return std::move(subresources_);
		}

	private:
		std::vector<subresource>	subresources_;
		std::vector<byte>			data_;
	};
} }