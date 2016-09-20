#ifndef LIBDAG_CONTAINER_FIND_ERROR_HPP_INCLUDED
#define LIBDAG_CONTAINER_FIND_ERROR_HPP_INCLUDED

#include <exception>
#include <type_traits>


namespace libdag
{
namespace container
{

template<
	typename Container
>
inline
typename
std::conditional<
	std::is_const<
		Container
	>::value,
	typename
	Container::mapped_type const &,
	typename
	Container::mapped_type &
>::type
find_error(
	Container &_container,
	typename
	Container::key_type const &_key
)
{
	auto const it{
		_container.find(
			_key
		)
	};

	if(
		it
		==
		_container.end()
	)
		std::terminate();

	return
		it->second;
}

}
}

#endif
