#ifndef LIBDAG_XML_UTIL_READ_CLOSING_TAG_HPP_INCLUDED
#define LIBDAG_XML_UTIL_READ_CLOSING_TAG_HPP_INCLUDED

#include <libdag/io/expect.hpp>
#include <libdag/io/read_until.hpp>
#include <iosfwd>
#include <string>


namespace libdag
{
namespace xml
{
namespace util
{

inline
std::string
read_closing_tag(
	std::istream &_stream
)
{
	libdag::io::expect(
		_stream,
		'<'
	);

	libdag::io::expect(
		_stream,
		'/'
	);

	return
		libdag::io::read_until(
			_stream,
			'>'
		);
}

}
}
}

#endif
