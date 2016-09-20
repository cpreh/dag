#ifndef LIBDAG_XML_UTIL_NEXT_TAG_CLOSES_HPP_INCLUDED
#define LIBDAG_XML_UTIL_NEXT_TAG_CLOSES_HPP_INCLUDED

#include <libdag/io/expect.hpp>
#include <libdag/io/get_char.hpp>
#include <libdag/io/unget.hpp>
#include <iosfwd>


namespace libdag
{
namespace xml
{
namespace util
{

inline
bool
next_tag_closes(
	std::istream &_stream
)
{
	libdag::io::expect(
		_stream,
		'<'
	);

	char const result{
		libdag::io::get_char(
			_stream
		)
	};

	libdag::io::unget(
		_stream
	);

	libdag::io::unget(
		_stream
	);

	return
		result
		==
		'/';
}

}
}
}

#endif
