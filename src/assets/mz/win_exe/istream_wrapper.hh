#ifndef __BSW_FS_ISTREAM_WRAPPER_HPP__
#define __BSW_FS_ISTREAM_WRAPPER_HPP__

#include <iostream>
#include <stdexcept>
#include <cstddef>
#include <optional>
#include <cstdint>
#include <vector>
#include <boost/pfr.hpp>
#include <type_traits>
#include <bsw/io/binary_reader.hh>
#include <bsw/exception.hh>

namespace bsw {
	class istream_wrapper;

	template<typename T>
	istream_wrapper& operator>>(istream_wrapper& is, T& x);

	class istream_wrapper {
		template<typename T>
		friend istream_wrapper& operator>>(istream_wrapper& is, T& x);

		friend istream_wrapper& operator>>(istream_wrapper& is, wchar_t& x);
		public:
			explicit istream_wrapper(std::istream& is);

			istream_wrapper(std::istream& is, std::size_t offs, std::size_t len);

			~istream_wrapper();

			[[nodiscard]] std::streampos current_pos() const;
			[[nodiscard]] std::streamsize size_to_end() const;

			[[nodiscard]] std::streamsize size() const;

			void advance(std::streampos delta);
			void seek(std::streampos pos);
			void read(char* buff, std::size_t size);

			void assert_word(uint16_t word);
			void assert_dword(uint32_t word);
			void assert_string(const wchar_t* s, bool align = true);

			bool check_string(const wchar_t* s, bool align = true);

			void assert_space(uint64_t s) const;

			void align4();

			void read_string(std::wstring& result, std::size_t n, bool ensure_null);

		private:
			void _seek(std::streampos pos, bool truncate);
			std::istream& m_stream;
			io::binary_reader m_reader;
			std::optional <std::streampos> m_old_pos;
			std::size_t m_size;
			std::streampos m_start_pos;
	};

	inline
	istream_wrapper& operator>>(istream_wrapper& is, wchar_t& x) {
		uint16_t u;
		is.m_reader >> u;
		x = u;
		return is;
	}

	template<typename T>
	istream_wrapper& operator>>(istream_wrapper& is, std::vector<T>& x) {
		for (std::size_t i=0; i<x.size(); i++) {
			is >> x[i];
		}
		return is;
	}

	template<typename T>
	istream_wrapper& operator>>(istream_wrapper& is, T& x) {
		static_assert(!std::is_union_v<T>);

		if constexpr (std::is_integral_v<T>) {
			is.m_reader >> x;
		} else {
			if constexpr (std::is_class_v<T>) {
				boost::pfr::for_each_field(x, [&is](auto& field) {
					is >> field;
				});
			} else if constexpr (std::is_union_v<T>) {
				RAISE_EX("Should not be here");
			}
		}

		if (!is.m_stream) {
			RAISE_EX("I/O error");
		}
		return is;
	}
} // ns bsw
#endif
