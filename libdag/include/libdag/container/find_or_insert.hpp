#ifndef LIBDAG_CONTAINER_FIND_OR_INSERT_HPP_INCLUDED
#define LIBDAG_CONTAINER_FIND_OR_INSERT_HPP_INCLUDED

#include <functional>
#include <utility>


namespace libdag
{
namespace container
{

/**
\brief Gets a mapped value from a map, or inserts a new one if not found

Searches for \a _key in \a _container. Returns the corresponding mapped value
if \a _key is found. Otherwise, calls <code>m := _make_mapped()</code>, inserts
<code>(_key, m)</code> and returns m. The second component of the result tells
if the key has been found.
*/
template<
	typename Container
>
std::pair<
	typename
	Container::mapped_type,
	bool
>
find_or_insert(
	Container &_container,
	typename
	Container::key_type const &_key,
	std::function<
		typename
		Container::mapped_type ()
	> const _make_mapped
)
{
	typename
	Container::iterator const it{
		_container.find(
			_key
		)
	};

	return
		it
		!=
		_container.end()
		?
			std::make_pair(
				it->second,
				true
			)
		:
			std::make_pair(
				_container.insert(
					std::make_pair(
						_key,
						_make_mapped()
					)
				).first->second,
				false
			)
		;
}

}
}

#endif
