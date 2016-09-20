#ifndef LIBDAG_UTIL_XML_NEXT_TAG_CLOSES_HPP_INCLUDED
#define LIBDAG_UTIL_XML_NEXT_TAG_CLOSES_HPP_INCLUDED

#include <libdag/io/expect.hpp>
#include <libdag/io/get_char.hpp>
#include <istream>


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

	_stream.unget();

	_stream.unget();

	return
		result
		==
		'/';
}

}
}
}

#endif
