#pragma once

#include "object.hpp"
#include <memory>
#include <map>
#include <functional>
#include <exception>
#include <assert.h>

namespace leaves { namespace pipeline
{
	class graphics_object_manager
	{
		friend class deleter;
		using object_container = std::map<string, object*>;
		using function_type = std::function<object*(string const& name)>;
		using function_container = std::map<object_type, function_type>;
	public:
		struct deleter
		{
			void operator() (object* obj) const
			{
				assert(nullptr != obj);
				graphics_object_manager::get().delete_object(obj->name(), obj);
			}
		};

		static graphics_object_manager& get()
		{
			static graphics_object_manager factor;
			return factor;
		}

		auto get_object(string const& name, object_type type) -> std::shared_ptr<object>
		{
			auto itr = objects_.find(name);
			object* created_object = nullptr;
			std::shared_ptr<object> obj;

			if (objects_.end() == itr)
			{
				auto func_itr = functinos_.find(type);
				if (functinos_.end() == func_itr)
					throw std::exception{};

				created_object = func_itr->second(name);
				objects_[name] = created_object;

				obj.reset(created_object, deleter{});
			}
			else
			{
				created_object = itr->second;
			}
			return created_object->shared_from_this();
		}

		bool register_creator(object_type type, function_type f)
		{
			auto itr = functinos_.end();
			auto result = false;
			std::tie(itr, result) = functinos_.emplace(type, f);
			return result;
		}

		void unregister_creator(object_type type)
		{
			auto itr = functinos_.find(type);
			if (functinos_.end() != itr)
				functinos_.erase(itr);
		}

	protected:
		void delete_object(string const& name, object* object)
		{
			assert(nullptr != object);

			auto itr = objects_.find(name);
			if (itr == objects_.end())
				throw std::exception{};

			if (itr->second != object)
				throw std::exception{};
			
			delete object;
			objects_.erase(itr);
		}

	private:
		object_container	objects_;
		function_container	functinos_;
	};
} }