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
	}

	class structured_layout
	{
		using var_container = std::vector<variable_layout>;
	public:
		struct variable
		{
		public:
			variable(variable_layout& layout, byte* ptr) noexcept
				: layout_(layout)
				, ptr_(ptr)
			{
			}

			template <typename T>
			decltype(auto) operator() () const
			{
				static_assert(!std::is_pointer<T>::value, "Can`t a be pointer type.");
				static_assert(!std::is_reference<T>::value, "Can`t be a reference type.");



				if (nullptr == ptr_)
					throw std::exception{};


			}

		private:
			variable_layout& layout_;
			byte*			 ptr_;
		};

	private:
		var_container	variables_;
	};
} }