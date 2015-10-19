#pragma once

namespace leaves { namespace pipeline 
{
	// input layout
	class input_layout
	{
	public:
		struct attribute
		{
			data_format		format;
			data_semantic	semantic;
			uint16_t		offset;
		};

		static constexpr size_t attribute_count = 16;
		using attribute_container = std::vector<attribute>;

	public:

		input_layout()
		{
			attributes_.reserve(attribute_count);
		}

		void add(data_format format, data_semantic semantic) noexcept
		{
			uint16_t offset = 0;
			if (!attributes_.empty())
			{
				auto const& back = attributes_.back();
				offset = back.offset + detail::size_of(back.format);
			}
			attributes_.push_back({ format, semantic, offset });
		}

		decltype(auto) get(size_t index) const
		{
			return attributes_[index];
		}

		decltype(auto) find(data_semantic semantic) const
		{
			return *std::find_if(attributes_.begin(), attributes_.end(),
				[semantic](attribute const& attr)
			{
				return semantic == attr.semantic;
			});
		}

		uint16_t size() const noexcept
		{
			if (attributes_.empty())
				return 0;

			auto& back = attributes_.back();
			return back.offset + detail::size_of(back.format);
		}

	private:
		attribute_container	attributes_;
	};
} }