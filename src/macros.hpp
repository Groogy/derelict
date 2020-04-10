#pragma once

#include <sstream>
#include <algorithm>

#define PP_STRING(a) #a
#define PP_CONCAT2(a,b)  a##b
#define PP_CONCAT3(a, b, c) a##b##c
#define PP_UNIQUE_LABEL_(prefix, suffix) PP_CONCAT2(prefix, suffix)
#define PP_UNIQUE_LABEL(prefix) PP_UNIQUE_LABEL_(prefix, __LINE__)

#define UNUSED(x) __attribute__((unused)) x

namespace macros
{
	inline std::string displayFloat(float value)
	{
		std::stringstream stream;
		stream << value;
		return stream.str();
	}

	template<typename Container, typename Type>
	typename Container::const_iterator find(const Container& container, const Type& obj)
	{
		return std::find(container.begin(), container.end(), obj);
	}

	template<typename Container, typename Type>
	typename Container::iterator find(Container& container, const Type& obj)
	{
		return std::find(container.begin(), container.end(), obj);
	}

	template<typename Container, typename Functor>
	typename Container::const_iterator findIf(const Container& container, Functor func)
	{
		return std::find_if(container.begin(), container.end(), func);
	}

	template<typename Container, typename Functor>
	typename Container::iterator findIf(Container& container, Functor func)
	{
		return std::find_if(container.begin(), container.end(), func);
	}

	template<typename Container, typename Type>
	bool contains(const Container& container, const Type& obj)
	{
		return find(container, obj) != container.end();
	}
}
