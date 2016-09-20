#ifndef LIBDAG_IO_EXPECT_HPP_INCLUDED
#define LIBDAG_IO_EXPECT_HPP_INCLUDED

#include <libdag/io/get_char.hpp>
#include <exception>
#include <iosfwd>
#include <string>
#include <stdexcept>


namespace libdag
{
namespace io
{

inline
void
expect(
	std::istream &_stream,
	char const _value
)
try
{
	char const result{
		libdag::io::get_char(
			_stream
		)
	};

	if(
		result != _value
	)
		throw
			std::runtime_error{
				std::string{
					"Expected "
				}
				+
				_value
			};
}
catch(
	std::exception const &_error
)
{
	throw
		std::runtime_error{
			std::string{
				_error.what()
			}
			+
			" while expecting "
			+
			_value
		};
}

}
}

#endif
