//
// Created by igor on 1/12/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_DATASTORE_DIRECTORY_ITERATOR_HH_
#define ASSETS_INCLUDE_ASSETS_DATASTORE_DIRECTORY_ITERATOR_HH_

#include <string>
#include <stack>
#include <functional>
#include <queue>
#include <assets/assets_export.h>
#include <assets/datastore/file.hh>
#include <assets/datastore/path.hh>

namespace neutrino::assets::datastore {
	class DirectoryIteratorImpl;

	class ASSETS_EXPORT directory_iterator
		/// The DirectoryIterator class is used to enumerate
		/// all files in a directory.
		///
		/// DirectoryIterator has some limitations:
		///   * only forward iteration (++) is supported
		///   * an iterator copied from another one will always
		///     point to the same file as the original iterator,
		///     even is the original iterator has been advanced
		///     (all copies of an iterator share their state with
		///     the original iterator)
		///   * because of this you should only use the prefix
		///     increment operator
	{
	 public:
		directory_iterator ();
		/// Creates the end iterator.

		explicit directory_iterator (const std::string& pth);
		/// Creates a directory iterator for the given path.

		directory_iterator (const directory_iterator& iterator);
		/// Creates a directory iterator for the given path.

		explicit directory_iterator (const file& f);
		/// Creates a directory iterator for the given file.

		explicit directory_iterator (const path& p);
		/// Creates a directory iterator for the given path.

		virtual ~directory_iterator ();
		/// Destroys the DirectoryIterator.

		[[nodiscard]] const std::string& name () const;
		/// Returns the current filename.

		[[nodiscard]] const path& get_path () const;
		/// Returns the current path.

		directory_iterator& operator= (const directory_iterator& it);
		directory_iterator& operator= (const file& f);
		directory_iterator& operator= (const path& p);
		directory_iterator& operator= (const std::string& pth);

		virtual directory_iterator& operator++ ();   // prefix

		//@ deprecated
		directory_iterator operator++ (int); // postfix
		/// Please use the prefix increment operator instead.

		const file& operator* () const;
		file& operator* ();
		const file* operator-> () const;
		file* operator-> ();

		bool operator== (const directory_iterator& iterator) const;
		bool operator!= (const directory_iterator& iterator) const;

	 protected:
		path m_path;
		file m_file;
	 private:
		DirectoryIteratorImpl* m_pimpl;
	};

//
// inlines
//
	inline const std::string& directory_iterator::name () const {
		return m_path.get_file_name ();
	}

	inline const path& directory_iterator::get_path () const {
		return m_path;
	}

	inline const file& directory_iterator::operator* () const {
		return m_file;
	}

	inline file& directory_iterator::operator* () {
		return m_file;
	}

	inline const file* directory_iterator::operator-> () const {
		return &m_file;
	}

	inline file* directory_iterator::operator-> () {
		return &m_file;
	}

	inline bool directory_iterator::operator== (const directory_iterator& iterator) const {
		return name () == iterator.name ();
	}

	inline bool directory_iterator::operator!= (const directory_iterator& iterator) const {
		return name () != iterator.name ();
	}
	// =========================================================================================
	class ASSETS_EXPORT sorted_directory_iterator : public directory_iterator
		/// The SortedDirectoryIterator class is similar to
		/// DirectoryIterator class, but places directories before files
		/// and sorts content alphabetically.
	{
	 public:
		sorted_directory_iterator ();
		/// Creates the end iterator.

		explicit sorted_directory_iterator (const std::string& pth);
		/// Creates a directory iterator for the given path.

		explicit sorted_directory_iterator (const directory_iterator& iterator);
		/// Creates a directory iterator for the given path.

		explicit sorted_directory_iterator (const file& f);
		/// Creates a directory iterator for the given file.

		explicit sorted_directory_iterator (const path& pth);
		/// Creates a directory iterator for the given path.

		~sorted_directory_iterator () override;
		/// Destroys the DirsFirstDirectoryIterator.

		sorted_directory_iterator& operator++ () override;   // prefix

	 private:
		bool m_is_finished;
		std::deque<std::string> m_directories;
		std::deque<std::string> m_files;

		void next ();
		/// Take next item
		void scan ();
		/// Scan directory to collect its children directories and files
	};

	namespace detail {
		class ASSETS_EXPORT traverse_base {
		 public:
			traverse_base () = delete;
			traverse_base (const traverse_base&) = delete;
			traverse_base& operator= (const traverse_base&) = delete;

			using stack_t = std::stack<directory_iterator>;
			using depth_func_t = std::function<uint16_t (const stack_t&)>;

			enum {
				D_INFINITE = 0 /// Special value for infinite traverse depth.
			};

			explicit traverse_base (depth_func_t depth_determiner, uint16_t max_depth = D_INFINITE);

		 protected:
			[[nodiscard]] bool is_finite_depth () const;
			static bool is_directory (const file& file);

			depth_func_t m_depth_determiner;
			uint16_t m_max_depth;
			directory_iterator m_end;

		 private:

		};

		class ASSETS_EXPORT children_first_traverse : public traverse_base {
		 public:
			explicit children_first_traverse (depth_func_t depth_determiner, uint16_t max_depth = D_INFINITE);

			std::string next (stack_t* it_stack, bool* is_finished);

			children_first_traverse () = delete;
			children_first_traverse (const children_first_traverse&) = delete;
			children_first_traverse& operator= (const children_first_traverse&) = delete;
		};

		class ASSETS_EXPORT siblings_first_traverse : public traverse_base {
		 public:
			explicit siblings_first_traverse (depth_func_t depth_determiner, uint16_t max_depth = D_INFINITE);

			std::string next (stack_t* it_stack, bool* isFinished);

			siblings_first_traverse () = delete;
			siblings_first_traverse (const siblings_first_traverse&) = delete;
			siblings_first_traverse& operator= (const siblings_first_traverse&) = delete;

		 private:
			std::stack<std::queue<std::string>> m_dirs_stack;
		};

		template <class TTraverseStrategy = children_first_traverse>
		class recursive_directory_iterator_impl {
		 public:
			enum {
				D_INFINITE = 0 /// Special value for infinite traverse depth.
			};

			explicit recursive_directory_iterator_impl (const std::string& pth, uint16_t max_depth = D_INFINITE)
				: m_max_depth (max_depth), m_traverse_strategy (depth_fun, m_max_depth), m_is_finished (false), m_rc (1) {
				m_stack.emplace (pth);
				m_current = m_stack.top ()->get_path ();
			}

			~recursive_directory_iterator_impl () = default;

			inline void duplicate () {
				++m_rc;
			}

			inline void release () {
				if (--m_rc == 0) {
					delete this;
				}
			}

			[[nodiscard]] inline uint16_t depth () const {
				return depth_fun (m_stack);
			}

			[[nodiscard]] inline uint16_t max_depth () const {
				return m_max_depth;
			}

			[[nodiscard]] inline const std::string& get () const {
				return m_current;
			}

			const std::string& next () {
				if (m_is_finished) {
					return m_current;
				}

				m_current = m_traverse_strategy.next (&m_stack, &m_is_finished);

				return m_current;
			}

		 private:
			typedef std::stack<directory_iterator> stack_t;

			static uint16_t depth_fun (const stack_t& stack)
			/// Function which implements the logic of determining
			/// recursion depth.
			{
				return static_cast<uint16_t>(stack.size ());
			}

			uint16_t m_max_depth;
			TTraverseStrategy m_traverse_strategy;
			bool m_is_finished;
			stack_t m_stack;
			std::string m_current;
			int m_rc;
		};

		template <class TTravStr = children_first_traverse>
		class recursive_directory_iterator
			/// The RecursiveDirectoryIterator class is used to enumerate
			/// all files in a directory and its subdirectories.
			///
			/// RecursiveDirectoryIterator has some limitations:
			///   * only forward iteration (++) is supported
			///   * an iterator copied from another one will always
			///     point to the same file as the original iterator,
			///     even is the original iterator has been advanced
			///     (all copies of an iterator share their state with
			///     the original iterator)
			///
			/// The class can follow different traversal strategies:
			///     * depth-first strategy;
			///     * siblings-first strategy.
			/// The strategies are set by template parameter.
			/// There are two corresponding typedefs:
			///     * SimpleRecursiveDirectoryIterator;
			///     * SiblingsFirstRecursiveDirectoryIterator.
			///
			/// The depth of traversal can be limited by constructor
			/// parameter max_depth (which sets the infinite depth by default).
		{
		 public:
			typedef recursive_directory_iterator<TTravStr> MyType;

			enum {
				D_INFINITE = 0 /// Constant for infinite traverse depth.
			};

			recursive_directory_iterator ()
			/// Creates the end iterator.
				: m_pimpl (0) {
			}

			explicit recursive_directory_iterator (const std::string& pth, uint16_t max_depth = D_INFINITE)
			/// Creates a recursive directory iterator for the given path.
				: m_pimpl (new impl_t (pth, max_depth)), m_path (m_pimpl->get ()), m_file (m_path) {
			}

			recursive_directory_iterator (const MyType& iterator)
				:
			/// Creates a copy of another recursive directory iterator.
				m_pimpl (iterator.m_pimpl), m_path (iterator.m_path), m_file (iterator.m_file) {
			}

			explicit recursive_directory_iterator (const directory_iterator& iterator, uint16_t max_depth = D_INFINITE)
				:
			/// Creates a recursive directory iterator for the path of
			/// non-recursive directory iterator.
				m_pimpl (new impl_t (iterator->get_path (), max_depth)), m_path (m_pimpl->get ()), m_file (m_path) {
			}

			explicit recursive_directory_iterator (const file& file, uint16_t maxDepth = D_INFINITE)
				:
			/// Creates a recursive directory iterator for the given path.
				m_pimpl (new impl_t (file.get_path (), maxDepth)), m_path (m_pimpl->get ()), m_file (m_path) {
			}

			explicit recursive_directory_iterator (const path& path, uint16_t maxDepth = D_INFINITE)
				:
			/// Creates a recursive directory iterator for the given path.
				m_pimpl (new impl_t (path.to_string (), maxDepth)), m_path (m_pimpl->get ()), m_file (m_path) {
			}

			~recursive_directory_iterator ()
			/// Destroys the DirectoryIterator.
			{
				if (m_pimpl) {
					m_pimpl->release ();
				}
			}

			[[nodiscard]] const std::string& name () const
			/// Returns the current filename.
			{
				return m_path.get_file_name ();
			}

			[[nodiscard]] const path& get_path () const
			/// Returns the current path.
			{
				return m_path;
			}

			[[nodiscard]] uint16_t depth () const
			/// Depth of recursion (counting from 1).
			{
				return m_pimpl->depth ();
			}

			[[nodiscard]] uint16_t max_depth () const
			/// Max depth of recursion (counting from 1).
			{
				return m_pimpl->max_depth ();
			}

			MyType& operator= (const MyType& it) {
				if (m_pimpl) {
					m_pimpl->release ();
				}
				m_pimpl = it.m_pimpl;
				if (m_pimpl) {
					m_pimpl->duplicate ();
					m_path = it.m_path;
					m_file = m_path;
				}
				return *this;
			}

			MyType& operator= (const file& file) {
				if (m_pimpl) {
					m_pimpl->release ();
				}
				m_pimpl = new impl_t (file.get_path ());
				m_path = path (m_pimpl->get ());
				m_file = m_path;
				return *this;
			}

			MyType& operator= (const path& path) {
				if (m_pimpl) {
					m_pimpl->release ();
				}
				m_pimpl = new impl_t (path.to_string ());
				m_path = path (m_pimpl->get ());
				m_file = m_path;
				return *this;
			}

			MyType& operator= (const std::string& path) {
				if (m_pimpl) {
					m_pimpl->release ();
				}
				m_pimpl = new impl_t (path);
				m_path = path (m_pimpl->get ());
				m_file = m_path;
				return *this;
			}

			MyType& operator++ () {
				if (m_pimpl) {
					m_path = path (m_pimpl->next ());
					m_file = m_path;
				}
				return *this;
			}

			const file& operator* () const {
				return m_file;
			}

			file& operator* () {
				return m_file;
			}

			const file* operator-> () const {
				return &m_file;
			}

			file* operator-> () {
				return &m_file;
			}

			template <class T1, class T2>
			friend inline bool
			operator== (const recursive_directory_iterator<T1>& a, const recursive_directory_iterator<T2>& b);
			template <class T1, class T2>
			friend inline bool
			operator!= (const recursive_directory_iterator<T1>& a, const recursive_directory_iterator<T2>& b);

		 private:
			typedef recursive_directory_iterator_impl<TTravStr> impl_t;

			impl_t* m_pimpl;
			path m_path;
			file m_file;
		};

		//
		// friend comparsion operators
		//
		template <class T1, class T2>
		inline bool operator== (const recursive_directory_iterator<T1>& a, const recursive_directory_iterator<T2>& b) {
			return a.get_path ().to_string () == b.get_path ().to_string ();;
		}

		template <class T1, class T2>
		inline bool operator!= (const recursive_directory_iterator<T1>& a, const recursive_directory_iterator<T2>& b) {
			return a.get_path ().to_string () != b.get_path ().to_string ();;
		}
	} // ns detail
	using simple_recursive_directory_iterator = detail::recursive_directory_iterator<detail::children_first_traverse>;
	using siblings_first_recursive_directory_iterator = detail::recursive_directory_iterator<detail::siblings_first_traverse>;
}

#endif //ASSETS_INCLUDE_ASSETS_DATASTORE_DIRECTORY_ITERATOR_HH_
