#include <algorithm>
#include <cstdlib>
#include <cstddef>
#include <exception>
//#include <functional>
#include <fstream>
#include <istream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>


namespace
{

namespace util
{

template<
	typename Container
	,
	typename Lambda
>
std::pair<
	//std::reference_wrapper<
		typename
		Container::mapped_type
	//>
	,
	bool
>
get_or_insert(
	Container &_container,
	typename
	Container::key_type const &_key,
	Lambda const _make_mapped
	/*
	std::function<
		typename
		Container::mapped_type ()
	> const _make_mapped*/
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
				//std::ref(
					it->second
				//),
				,
				true
			)
		:
			std::make_pair(
				//std::ref(
					_container.insert(
						std::make_pair(
							_key,
							_make_mapped()
						)
					).first->second
				//),
				,
				false
			)
		;
}

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

inline
void
expect(
	std::istream &_stream,
	char const _value
)
{
	char const result{
		util::get_char(
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


typedef
unsigned
tree_id;

typedef
std::vector<
	tree_id
>
tree_id_list;

typedef
std::unordered_map<
	tree_id,
	std::pair<
		std::string,
		tree_id_list
	>
>
unfold_map;

void
unfold(
	std::ostream &_stream,
	unfold_map const &_map,
	tree_id const _id
)
{
	unfold_map::const_iterator const it{
		_map.find(
			_id
		)
	};

	if(
		it
		==
		_map.end()
	)
		std::terminate();

	unfold_map::mapped_type const &mapped{
		it->second
	};

	_stream
		<<
		'<'
		<<
		mapped.first
		<<
		">\n";

	for(
		tree_id const child
		:
		mapped.second
	)
		unfold(
			_stream,
			_map,
			child
		);

	_stream
		<<
		"</"
		<<
		mapped.first
		<<
		">\n";
}

struct state
{
	state()
	:
		next_id_{
			0u
		},
		seen_trees_{}
	{
	}

	state(
		state &&
	) = default;

	state(
		state const &
	) = delete;

	state &
	operator=(
		state &&
	) = default;

	state &
	operator=(
		state const &
	) = delete;


	tree_id next_id_;

	typedef
	std::map<
		tree_id_list,
		tree_id
	>
	child_map;

	typedef
	std::unordered_map<
		std::string,
		child_map
	>
	tree_map;

	tree_map seen_trees_;

	unfold_map tree_structure_;
};

struct tree_result
{
	tree_id tree_id_;
};

std::pair<
	tree_result,
	state
>
update_state(
	std::string &&_node_name,
	tree_id_list &&_children,
	state &&_state
)
{
	std::pair<
		tree_id,
		bool
	> const result{
		util::get_or_insert(
			_state.seen_trees_[
				_node_name
			],
			_children,
			[
				&_state
			]{
				return
					_state.next_id_++;
			}
		)
	};

	if(
		!result.second
	)
		_state.tree_structure_.insert(
			std::make_pair(
				result.first,
				std::make_pair(
					std::move(
						_node_name
					),
					std::move(
						_children
					)
				)
			)
		);

	return
		std::make_pair(
			tree_result{
				result.first
			},
			std::move(
				_state
			)
		);
}

void
read_opening_tag(
	std::istream &_stream
)
{
	util::read_until(
		_stream,
		'>'
	);
}

bool
next_tag_closes(
	std::istream &_stream
)
{
	util::expect(
		_stream,
		'<'
	);

	char const result{
		util::get_char(
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

std::string
read_closing_tag(
	std::istream &_stream
)
{
	util::expect(
		_stream,
		'<'
	);

	util::expect(
		_stream,
		'/'
	);

	return
		util::read_until(
			_stream,
			'>'
		);
}

std::pair<
	tree_result,
	state
>
parse(
	std::istream &_stream,
	state &&_state
)
{
	read_opening_tag(
		_stream
	);

	tree_id_list children;

	while(
		!next_tag_closes(
			_stream
		)
	)
	{
		std::pair<
			tree_result,
			state
		> result{
			parse(
				_stream,
				std::move(
					_state
				)
			)
		};

		children.push_back(
			result.first.tree_id_
		);

		_state =
			std::move(
				result.second
			);
	}

	std::sort(
		children.begin(),
		children.end()
	);

	std::string closing_tag(
		read_closing_tag(
			_stream
		)
	);

	return
		update_state(
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

int
main(
	int argc,
	char *argv[]
)
try
{
	if(
		argc
		!=
		2
	)
	{
		std::cerr
			<<
			"Usage: <filename>\n";

		return
			EXIT_FAILURE;
	}

	char const *const filename{
		argv[1]
	};

	std::ifstream ifs(
		filename
	);

	if(
		!ifs.is_open()
	)
	{
		std::cerr
			<<
			"Failed to open "
			<<
			filename
			<<
			'\n';

		return
			EXIT_FAILURE;
	}

	std::pair<
		tree_result,
		state
	> const result{
		parse(
			ifs,
			state{}
		)
	};

	unfold(
		std::cout,
		result.second.tree_structure_,
		result.first.tree_id_
	);

	return
		EXIT_SUCCESS;
}
catch(
	std::exception const &_error
)
{
	std::cerr
		<<
		_error.what()
		<<
		'\n';

	return
		EXIT_FAILURE;
}
