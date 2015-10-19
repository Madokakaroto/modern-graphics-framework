#pragma once

#include "numeric_variable.hpp"

namespace leaves { namespace pipeline 
{
	namespace detail
	{
		template <typename T>
		struct interpret_as;

		template <typename T>
		struct interpret_as<T&>
		{
			static decltype(auto) exec(byte const* ptr)
			{
				return *reinterpret_cast<T*>(ptr);
			}
		};

		template <typename T>
		struct interpret_as<T*>
		{
			static decltype(auto) exec(byte const* ptr)
			{
				return reinterpret_cast<T*>(ptr);
			}
		};

		struct numeric_variable
		{
		public:
			numeric_variable(variable_layout const& layout, byte* ptr) noexcept
				: layout_(layout)
				, ptr_(ptr)
			{
			}

			template <typename T>
			decltype(auto) operator() ()
			{
				return const_cast<T>(const_cast<variable const&>(*this)<T>());
			}

			template <typename T>
			decltype(auto) operator() () const
			{
				static_assert(!std::is_pointer<T>::value, "Can`t a be pointer type.");
				static_assert(!std::is_reference<T>::value, "Can`t be a reference type.");

				if (sizeof(T) != layout_.size() || nullptr == ptr_)
					throw std::exception{};

				using type = std::add_pointer_t<std::add_const_t<T>>;
				return *reinterpret_cast<type*>(ptr_ + layout_.offset());
			}

			numeric_variable operator[] (size_t index) const
			{
				return{ layout_[index], ptr_ };
			}

		private:
			variable_layout const&	layout_;
			byte*					ptr_;
		};

		template <typename T>
		struct large_class_wrapper
		{
		};
	}

	template <typename T>
	auto wrap_large_class()
	{
		return detail::large_class_wrapper<typename T::tuple_type>{};
	}

	class structured_layout
	{
		using var_container = std::vector<variable_layout>;
		static constexpr size_t attribute_count = 8;

		struct foo
		{
			uint16_t offset;
			uint16_t reg;
		};

	public:
		structured_layout()
		{
			variables_.reserve(attribute_count);
		}

		template <typename T>
		structured_layout(detail::large_class_wrapper<T> tuple)
			: structured_layout()
		{
			using type = std::tuple_element_t<0, T>;
			std::tuple_size<T>::value;
		}

		void add(data_format format, uint16_t count, uint16_t size, uint16_t offset)
		{
			variables_.emplace_back(format, count, size, offset);
		}

	private:

		template <typename T, size_t ... Is>
		void init_from_tupel(detail::large_class_wrapper<T> tuple, std::index_sequence<Is...> seq)
		{
			foo helper = { 0, 0 };

			using swallow_t = bool[];
			swallow_t s = { (add_numeric<std::tuple_element_t<Is, T>>(helper), true)... };
		}

		template <typename T>
		void add_numeric(foo& helper)
		{
			using traits_type = numeric_traits<T>;
			
			// º∆À„ size & offset
			add(traits_type::format(), traits_type::count(), )
		}

	private:
		var_container	variables_;
	};
} }