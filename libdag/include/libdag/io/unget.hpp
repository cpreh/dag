#ifndef LIBDAG_IO_UNGET_HPP_INCLUDED
#define LIBDAG_IO_UNGET_HPP_INCLUDED

#include <istream>
#include <stdexcept>


namespace libdag
{
namespace io
{

inline
void
unget(
	std::istream &_stream
)
{
	if(
		!_stream.unget()
	)
		throw
			std::runtime_error{
				"unget failed"
			};
}

}
}

#endif
