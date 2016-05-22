#pragma once

#include "buffer.hpp"
#include "iterator.hpp"

namespace leaves { namespace pipeline
{
	template <>
	struct buffer_traits<vertex_buffer>
	{
		static constexpr object_type type() noexcept
		{
			return object_type::vertex_buffer;
		}
	};
	
	class vertex_buffer : public buffer<vertex_buffer>
	{
	public:
		using base_type = buffer<vertex_buffer>;
	
		template <typename T>
		using iterator = stream_buffer_iterator<T>;
	
	public:
		// constructor
		vertex_buffer(string&& name, input_layout&& layout, size_t count)
			: base_type(std::move(name), layout.size(), count, device_access::none, device_access::read)
			, layout_(std::move(layout))
		{
			allocate();
		}

		vertex_buffer(string&& name, input_layout const& layout, size_t count)
			: base_type(std::move(name), layout.size(), count, device_access::none, device_access::read)
			, layout_(layout)
		{
			allocate();
		}
	
		// attribute access
		input_layout const& layout() const noexcept
		{
			return layout_;
		}
	
		// attribute modify
		void resize(input_layout&& meta_data, size_t count)
		{
			auto elem_size = meta_data.size();
			base_type::resize(elem_size, count);
			layout_ = std::move(meta_data);
		}
	
		// content access
		template <typename T>
		auto begin(data_semantic semantic)
		{
			auto& attr = layout_.find(semantic);
	
			if (sizeof(T) != detail::size_of(attr.format))
				throw std::exception{};
	
			using iterator_type = iterator<T>;
			return iterator_type{ data(), data() + size(), attr.offset, static_cast<std::ptrdiff_t>(elem_size()) };
		}
	
		template <typename T>
		auto end(data_semantic semantic)
		{
			auto& attr = layout_.find(semantic);
	
			if (sizeof(T) != detail::size_of(attr.format))
				throw std::exception{};
	
			using iterator_type = iterator<T>;
			auto ptr = data() + size();
			return iterator_type{ ptr, ptr, attr.offset, static_cast<std::ptrdiff_t>(elem_size()) };
		}
	
		template <typename T>
		auto begin()
		{
			if(sizeof(T) != elem_size())
				throw std::exception{};
	
			using iterator_type = iterator<T>;
			return iterator_type{ data(), data() + size(), 0, sizeof(T) };
		}
	
		template <typename T>
		auto end()
		{
			if (sizeof(T) != elem_size())
				throw std::exception{};
	
			using iterator_type = iterator<T>;
			auto ptr = data() + size();
			return iterator_type{ ptr, ptr, 0, sizeof(T) };
		}
	
	private:
		input_layout	layout_;
	};
} }