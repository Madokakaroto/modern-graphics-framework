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
			size_t			offset;
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
			size_t offset = 0;
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

		size_t size() const noexcept
		{
			return std::accumulate(attributes_.begin(), attributes_.end(), size_t{ 0 }, [](size_t sum, attribute const& attr)
			{
				return sum + detail::size_of(attr.format);
			});
		}

	private:
		attribute_container	attributes_;
	};
} }