#ifndef __BSW_FS_ISTREAM_WRAPPER_HPP__
#define __BSW_FS_ISTREAM_WRAPPER_HPP__

#include <iostream>
#include <stdexcept>
#include <cstddef>
#include <optional>
#include <cstring>
#include <cstdint>
#include <type_traits>

namespace bsw {
	class istream_wrapper;

	template<typename T>
	istream_wrapper& operator>>(istream_wrapper& is, T& x);

	class istream_wrapper {
		template<typename T>
		friend istream_wrapper& operator>>(istream_wrapper& is, T& x);

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
			std::istream* stream;
			std::optional <std::streampos> m_old_pos;
			std::size_t m_size;
			std::streampos m_start_pos;
	};

	template<typename T>
	inline
	istream_wrapper& operator>>(istream_wrapper& is, T& x) {
		is.stream->read(reinterpret_cast <char*>(&x), sizeof (x));
		if (!(*is.stream)) {
			throw std::runtime_error("I/O error");
		}
		return is;
	}
} // ns bsw
#endif
