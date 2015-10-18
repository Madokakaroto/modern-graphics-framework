#pragma once

namespace leaves { namespace pipeline 
{
	namespace detail
	{
		// {type} {name}[count];
		class variable
		{
			using sub_vars = std::list<variable>;

			template <typename T>
			struct helper;

			template <typename T>
			struct helper<T&>
			{
				static decltype(auto) exec(byte const* ptr)
				{
					return *reinterpret_cast<T*>(ptr);
				}
			};

			template <typename T>
			struct helper<T*>
			{
				static decltype(auto) exec(byte const* ptr)
				{
					return reinterpret_cast<T*>(ptr);
				}
			};

		public:
			// constructor
			variable(data_format format, uint16_t count, uint16_t size, uint16_t offset)
				: format_(format)
				, count_(count)
				, size_(size)
				, offset_(offset)
				, sub_vars_()
			{

			}

			// add sub variables
			void push_back(data_format format, uint16_t count, uint16_t size, uint16_t offset)
			{
				//sub_vars_.push_back
			}

			template <typename T>
			auto operator()(byte* ptr)
			{
				using type = std::remove_volatile_t<T>;
				return const_cast<type>(const_cast<variable const&>(*this)<type>(ptr));
			}

			template <typename T>
			auto operator()(byte const* ptr) const
			{
				using underly = std::remove_cv_t<T>;
				using type = std::conditional_t<std::is_reference<underly>::value,
					std::add_lvalue_reference_t<std::add_const_t<underly>>,
					std::add_pointer_t<std::add_const_t<underly>>>

				assert(nullptr != ptr);
				if (sizeof(T) != size_)
					throw std::exception{};

				return helper<type>::exec(ptr + offset_);
			}

		private:
			data_format		format_;
			uint16_t		count_;
			uint16_t		size_;
			uint16_t		offset_;
			sub_vars		sub_vars_;
		};
	}
} }