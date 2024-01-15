//
// Created by igor on 1/12/24.
//


#include <algorithm>

#include <assets/datastore/directory_iterator.hh>
#include <assets/datastore/directory_iterator_unix.hh>
#include <bsw/exception.hh>

namespace neutrino::assets::datastore {
	directory_iterator::directory_iterator ()
		: m_pimpl (nullptr) {
	}

	directory_iterator::directory_iterator (const std::string& pth)
		: m_path (pth), m_pimpl (new DirectoryIteratorImpl (pth)) {
		m_path.make_directory ();
		m_path.set_file_name (m_pimpl->get ());
		m_file = m_path;
	}

	directory_iterator::directory_iterator (const directory_iterator& iterator)
		: m_path (iterator.m_path), m_pimpl (iterator.m_pimpl) {
		if (m_pimpl) {
			m_pimpl->duplicate ();
			m_file = m_path;
		}
	}

	directory_iterator::directory_iterator (const file& f)
		: m_path (f.get_path ()), m_pimpl (new DirectoryIteratorImpl (f.get_path ())) {
		m_path.make_directory ();
		m_path.set_file_name (m_pimpl->get ());
		m_file = m_path;
	}

	directory_iterator::directory_iterator (const path& p)
		: m_path (p), m_pimpl (new DirectoryIteratorImpl (p.to_string ())) {
		m_path.make_directory ();
		m_path.set_file_name (m_pimpl->get ());
		m_file = m_path;
	}

	directory_iterator::~directory_iterator () {
		if (m_pimpl) {
			m_pimpl->release ();
		}
	}

	directory_iterator& directory_iterator::operator= (const directory_iterator& it) {
		if (&it != this) {
			if (m_pimpl) { m_pimpl->release (); }
			m_pimpl = it.m_pimpl;
			if (m_pimpl) {
				m_pimpl->duplicate ();
				m_path = it.m_path;
				m_file = m_path;
			}
		}
		return *this;
	}

	directory_iterator& directory_iterator::operator= (const file& f) {
		if (m_pimpl) { m_pimpl->release (); }
		m_pimpl = new DirectoryIteratorImpl (f.get_path ());
		m_path.parse_directory (f.get_path ());
		m_path.set_file_name (m_pimpl->get ());
		m_file = m_path;
		return *this;
	}

	directory_iterator& directory_iterator::operator= (const path& p) {
		if (m_pimpl) { m_pimpl->release (); }
		m_pimpl = new DirectoryIteratorImpl (p.to_string ());
		m_path = p;
		m_path.make_directory ();
		m_path.set_file_name (m_pimpl->get ());
		m_file = m_path;
		return *this;
	}

	directory_iterator& directory_iterator::operator= (const std::string& pth) {
		if (m_pimpl) { m_pimpl->release (); }
		m_pimpl = new DirectoryIteratorImpl (pth);
		m_path.parse_directory (pth);
		m_path.set_file_name (m_pimpl->get ());
		m_file = m_path;
		return *this;
	}

	directory_iterator& directory_iterator::operator++ () {
		if (m_pimpl) {
			m_path.set_file_name (m_pimpl->next ());
			m_file = m_path;
		}
		return *this;
	}

	directory_iterator directory_iterator::operator++ (int dummy) {
		if (m_pimpl) {
			m_path.set_file_name (m_pimpl->next ());
			m_file = m_path;
		}
		return *this;
	}
	// ===================================================================================
	sorted_directory_iterator::sorted_directory_iterator ()
		: directory_iterator (), m_is_finished (true) {
	}

	sorted_directory_iterator::sorted_directory_iterator (const std::string &pth)
		: directory_iterator (pth), m_is_finished (false) {
		scan ();
		next ();
	}

	sorted_directory_iterator::sorted_directory_iterator (const directory_iterator &iterator)
		: directory_iterator (iterator), m_is_finished (false) {
		scan ();
		next ();
	}

	sorted_directory_iterator::sorted_directory_iterator (const file &f)
		: directory_iterator (f), m_is_finished (false) {
		scan ();
		next ();
	}

	sorted_directory_iterator::sorted_directory_iterator (const path &pth)
		: directory_iterator (pth), m_is_finished (false) {
		scan ();
		next ();
	}

	sorted_directory_iterator::~sorted_directory_iterator ()
	= default;

	sorted_directory_iterator &sorted_directory_iterator::operator++ () {
		if (!m_is_finished) {
			next ();
		}
		return *this;
	}

	void sorted_directory_iterator::scan () {
		directory_iterator end_it;
		while (*this != end_it) {
			bool isDir = false;
			try {
				isDir = (*this)->is_directory ();
			}
			catch (...) {
			}
			if (isDir) {
				m_directories.push_back (m_path.to_string ());
			} else {
				m_files.push_back (m_path.to_string ());
			}

			directory_iterator::operator++ ();
		}

		std::sort (m_directories.begin (), m_directories.end ());
		std::sort (m_files.begin (), m_files.end ());
	}

	void sorted_directory_iterator::next () {
		directory_iterator end_it;
		if (!m_directories.empty ()) {
			m_path.assign (m_directories.front ());
			m_directories.pop_front ();
			m_file = m_path;
		} else if (!m_files.empty ()) {
			m_path.assign (m_files.front ());
			m_files.pop_front ();
			m_file = m_path;
		} else {
			m_is_finished = true;
			m_path = end_it.get_path ();
			m_file = m_path;
		}
	}
	// ===================================================================================
	namespace detail {
		//
		// TraverseBase
		//
		traverse_base::traverse_base (depth_func_t depth_determiner, uint16_t max_depth)
			: m_depth_determiner (std::move (depth_determiner)), m_max_depth (max_depth) {
		}

		inline bool traverse_base::is_finite_depth () const {
			return m_max_depth != D_INFINITE;
		}

		bool traverse_base::is_directory (const file& file) {
			try {
				return file.is_directory ();
			}
			catch (...) {
				return false;
			}
		}

//
// ChildrenFirstTraverse
//
		children_first_traverse::children_first_traverse (depth_func_t depth_determiner, uint16_t max_depth)
			: traverse_base (std::move (depth_determiner), max_depth) {
		}

		std::string children_first_traverse::next (stack_t* it_stack, bool* is_finished) {
			// pointer mustn't point to NULL and iteration mustn't be finished
			ENFORCE(is_finished);
			ENFORCE(!(*is_finished));

			std::stack<directory_iterator> it;

			//_depthDeterminer(it);

			// go deeper into not empty directory
			// (if depth limit allows)
			bool isDepthLimitReached = is_finite_depth () && m_depth_determiner (*it_stack) >= m_max_depth;
			if (!isDepthLimitReached && is_directory (*it_stack->top ())) {
				directory_iterator child_it (it_stack->top ().get_path ());
				// check if directory is empty
				if (child_it != m_end) {
					it_stack->push (child_it);
					return child_it->get_path ();
				}
			}

			++(it_stack->top ());

			ENFORCE(!it_stack->empty ());
			// return up until there isn't right sibling
			while (it_stack->top () == m_end) {
				it_stack->pop ();

				// detect end of traversal
				if (it_stack->empty ()) {
					*is_finished = true;
					return m_end->get_path ();
				} else {
					++(it_stack->top ());
				}
			}

			return it_stack->top ()->get_path ();
		}

//
// SiblingsFirstTraverse
//
		siblings_first_traverse::siblings_first_traverse (depth_func_t depth_determiner, uint16_t max_depth)
			: traverse_base (std::move (depth_determiner), max_depth) {
			m_dirs_stack.emplace ();
		}

		std::string siblings_first_traverse::next (stack_t* it_stack, bool* isFinished) {
			// pointer mustn't point to NULL and iteration mustn't be finished
			ENFORCE(isFinished);
			ENFORCE(!(*isFinished));

			// add dirs to queue (if depth limit allows)
			bool isDepthLimitReached = is_finite_depth () && m_depth_determiner (*it_stack) >= m_max_depth;
			if (!isDepthLimitReached && is_directory (*it_stack->top ())) {
				const std::string& p = it_stack->top ()->get_path ();
				m_dirs_stack.top ().push (p);
			}

			++(it_stack->top ());

			ENFORCE(!it_stack->empty ());
			// return up until there isn't right sibling
			while (it_stack->top () == m_end) {
				// try to find first not empty directory and go deeper
				while (!m_dirs_stack.top ().empty ()) {
					std::string dir = m_dirs_stack.top ().front ();
					m_dirs_stack.top ().pop ();
					directory_iterator child_it (dir);

					// check if directory is empty
					if (child_it != m_end) {
						it_stack->push (child_it);
						m_dirs_stack.emplace ();
						return child_it->get_path ();
					}
				}

				// if fail go upper
				it_stack->pop ();
				m_dirs_stack.pop ();

				// detect end of traversal
				if (it_stack->empty ()) {
					*isFinished = true;
					return m_end->get_path ();
				}
			}

			return it_stack->top ()->get_path ();
		}
	}
}