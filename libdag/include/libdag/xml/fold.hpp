#ifndef LIBDAG_XML_FOLD_HPP_INCLUDED
#define LIBDAG_XML_FOLD_HPP_INCLUDED

#include <libdag/xml/util/next_tag_closes.hpp>
#include <libdag/xml/util/read_closing_tag.hpp>
#include <libdag/xml/util/read_opening_tag.hpp>
#include <iosfwd>
#include <functional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>


namespace libdag
{
namespace xml
{

/**
\brief Folds an XML-file post-order

The XML file \a _stream is read in post-order. The initial state of the algorithm is \a _state, which
is carried through all calls of the function. Each time a whole sub-tree <code>f(t_1, ..., t_n)</code> has been read,
we inductively have produced results <code>r_1, ..., r_n</code> of type Result. We then
call <code>_update_state(f, {r_1,...,r_n}, s)</code>, where <code>s</code> is the current state.
*/
template<
	typename Result,
	typename State
>
std::pair<
	Result,
	State
>
fold(
	std::istream &_stream,
	State &&_state,
	std::function<
		std::pair<
			Result,
			State
		>(
			std::string,
			std::vector<
				Result
			>,
			State
		)
	> const _update_state
)
{
	std::string const opening_tag{
		libdag::xml::util::read_opening_tag(
			_stream
		)
	};

	typedef
	std::vector<
		Result
	>
	child_vector;

	child_vector children;

	while(
		!libdag::xml::util::next_tag_closes(
			_stream
		)
	)
	{
		std::pair<
			Result,
			State
		> result{
			libdag::xml::fold(
				_stream,
				std::move(
					_state
				),
				_update_state
			)
		};

		children.push_back(
			std::move(
				result.first
			)
		);

		_state =
			std::move(
				result.second
			);
	}

	std::string closing_tag(
		libdag::xml::util::read_closing_tag(
			_stream
		)
	);

	if(
		opening_tag
		!=
		closing_tag
	)
		throw
			std::runtime_error{
				"Tag mismatch: <"
				+
				opening_tag
				+
				"></"
				+
				closing_tag
				+
				">"
			};

	return
		_update_state(
			std::move(
				closing_tag
			),
			std::move(
				children
			),
			std::move(
				_state
			)
		);
}

}
}

#endif
