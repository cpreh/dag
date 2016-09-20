#ifndef LIBDAG_UTIL_GET_CHAR_HPP_INCLUDED
#define LIBDAG_UTIL_GET_CHAR_HPP_INCLUDED

#include <istream>
#include <stdexcept>


namespace libdag
{
namespace io
{

inline
char
get_char(
	std::istream &_stream
)
{
	char result;

	if(
		!(_stream >> result)
	)
		throw
			std::runtime_error{
				"Failed to read char from stream!"
			};

	return
		result;
}

}
}

#endif
