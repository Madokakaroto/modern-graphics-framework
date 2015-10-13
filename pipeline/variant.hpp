#pragma once

#include <cstdint>
#include <typeindex>
#include <type_traits>
#include <tuple>
#include "type_list.hpp"

namespace kingsoft 
{
	template <std::size_t I, std::size_t ... Is>
	struct integral_max;

	template <std::size_t I>
	struct integral_max<I> : std::integral_constant<std::size_t, I>
	{

	};

	template <std::size_t I1, std::size_t I2, std::size_t ... Is>
	struct integral_max<I1, I2, Is...> : std::integral_constant<std::size_t, (I1 > I2) ? 
		integral_max<I1, Is...>::value : integral_max<I2, Is...>::value>
	{

	};

	namespace detail
	{
		template <typename ... Args>
		struct helper;

		template <typename T, typename ... Args>
		struct helper<T, Args...>
		{
			static void destroy(std::type_index index, void* data)
			{
				if (std::type_index{ typeid(T) } == index)
				{
					reinterpret_cast<T*>(data)->~T();
				}
				else
				{
					helper<Args...>::destroy(index, data);
				}
			}

			static void move(std::type_index index, void* src, void* dest)
			{
				if (std::type_index{ typeid(T) } == index)
				{
					new (dest) T(std::move(*reinterpret_cast<T*>(src)));
				}
				else
				{
					helper<Args...>::move(index, src, dest);
				}
			}

			static void copy(std::type_index index, void const* src, void* dest)
			{
				if (std::type_index{ typeid(T) } == index)
				{
					new (dest) T(*reinterpret_cast<T*>(src));
				}
				else
				{
					helper<Args...>::copy(index, src, dest);
				}
			}
		};

		// 这里其实是语法有效语义无效的代码，为了终止递归，还是用展开比较好
		template<>
		struct helper<>
		{
			static void destroy(std::type_index index, void* data)
			{
			}

			static void move(std::type_index index, void* src, void* dest)
			{
			}

			static void copy(std::type_index index, void const* src, void* dest)
			{
			}
		};
	}

	template <typename ... Args>
	class variant
	{
		typedef variant this_type;
		typedef detail::helper<Args...> helper_type;
		typedef make_type_list_t<Args...> type_list_type;

	public:
		static constexpr std::size_t size()
		{
			return integral_max<sizeof(Args)...>::value;
		}

		static constexpr std::size_t align()
		{
			typedef std::tuple<Args...> tuple_type;
			return alignof(tuple_type);
		}

		typedef std::aligned_storage_t<this_type::size(), this_type::align()> storage_type;

	public:
		// default constructor
		variant(void)
			: index_(typeid(void))
			, data_()
		{

		}

		// desctructor
		~variant()
		{
			helper_type::destroy(index_, &data_);
		}

		// copy construct 
		variant(variant const& rhs)
			: index_(rhs.index_)
		{
			helper_type::copy(index_, &rhs.data_, &data_);
		}

		// move construct
		variant(variant&& rhs)
			: index_(rhs.index_)
		{
			helper_type::move(index_, &rhs.data_, &data_);
		}

		// convert construct
		template 
		<
			typename T,
			typename = std::enable_if_t<typelist::index_of<type_list_type, 
														   std::remove_reference_t<T> >::value != -1> 
		>
		variant(T&& value)
			: index_(typeid(void))
		{
			typedef std::remove_reference_t<T> value_type;
			index_ = std::type_index{ typeid(T) };				// typeid will descard reference
			new (&data_) value_type(std::forward<T>(value));
		}

		// copy assign
		variant& operator= (variant const& rhs)
		{
			helper_type::destroy(index_, &data_);
			index_ = rhs.index_;
			helper_type::copy(index_, &rhs.data_, &data_);
			return *this;
		}

		// move assign
		variant& operator= (variant&& rhs)
		{
			helper_type::destroy(index_, &data_);
			index_ = rhs.index_;
			helper_type::move(index_, &rhs.data_, &data_);
			return *this;
		}

		bool operator== (variant const& rhs) const
		{
			return index_ == rhs.index_ && 0 == ::memcmp(&data_, rhs.data_, sizeof(data_))
		}

		bool operator< (variant const& rhs) const
		{
			if (index_ < rhs.index_)
				return true;
			if (::memcmp(&data_, &rhs.data_, sizeof(data_) < 0))
				return true;
			return false;
		}

		template <typename T>
		bool is() const
		{
			return std::type_index{ typeid(T) } == index_;
		}

		bool empty() const
		{
			return std::type_index(typeid(void)) == index_;
		}

		std::type_index type() const
		{
			return index_;
		}

		template <typename T>
		auto get() -> std::add_lvalue_reference_t<std::decay_t<T> >
		{
			typedef std::decay_t<T> underly_type;
			if (!is<underly_type>())
				throw std::bad_cast{};

			return *reinterpret_cast<underly_type*>(&data_);
		}

	private:
		std::type_index		index_;
		storage_type		data_;
	};
}