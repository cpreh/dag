#include <libdag/container/find_error.hpp>
#include <libdag/container/find_or_insert.hpp>
#include <libdag/xml/fold.hpp>
#include <algorithm>
#include <cstdlib>
#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>


namespace
{

typedef
unsigned
tree_id;

struct tree_result
{
	tree_id tree_id_;
};

bool
operator<(
	tree_result const &_left,
	tree_result const &_right
)
{
	return
		_left.tree_id_
		<
		_right.tree_id_;
}

typedef
std::vector<
	tree_result
>
tree_list;

typedef
std::unordered_map<
	tree_id,
	std::pair<
		std::string,
		tree_list
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
	unfold_map::mapped_type const &mapped{
		libdag::container::find_error(
			_map,
			_id
		)
	};

	_stream
		<<
		'<'
		<<
		mapped.first
		<<
		">\n";

	for(
		tree_result const child
		:
		mapped.second
	)
		unfold(
			_stream,
			_map,
			child.tree_id_
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
		seen_trees_{},
		tree_structure_{}
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
		tree_list,
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

std::pair<
	tree_result,
	state
>
update_state(
	std::string &&_node_name,
	tree_list &&_children,
	state &&_state
)
{
	std::sort(
		_children.begin(),
		_children.end()
	);

	std::pair<
		tree_id,
		bool
	> const result{
		libdag::container::find_or_insert(
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

}

int
main()
try
{
	std::pair<
		tree_result,
		state
	> const result{
		libdag::xml::fold(
			std::cin,
			state{},
			std::function<
				std::pair<
					tree_result,
					state
				>(
					std::string,
					tree_list,
					state
				)
			>{
				update_state
			}
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
