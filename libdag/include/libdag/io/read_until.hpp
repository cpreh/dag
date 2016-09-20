#ifndef LIBDAG_IO_READ_UNTIL_HPP_INCLUDED
#define LIBDAG_IO_READ_UNTIL_HPP_INCLUDED

#include <iosfwd>
#include <stdexcept>
#include <string>


namespace libdag
{
namespace io
{

inline
std::string
read_until(
	std::istream &_stream,
	char const _value
)
{
	std::string result;

	if(
		!std::getline(
			_stream,
			result,
			_value
		)
	)
		throw
			std::runtime_error{
				std::string{
					"Failed to read until "
				}
				+
				_value
			};

	return
		result;
}

}
}

#endif
