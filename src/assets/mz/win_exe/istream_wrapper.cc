#include <sstream>
#include <vector>
#include <cstring>
#include "istream_wrapper.hh"
#include <bsw/strings/wchar.hh>
#include <bsw/exception.hh>
#include <assets/resources/detail/istream_pos_keeper.hh>

namespace bsw {
	istream_wrapper::istream_wrapper(std::istream& is)
		: m_stream(is),
		  m_reader(is, io::binary_reader::LITTLE_ENDIAN_BYTE_ORDER) {
		neutrino::assets::detail::istream_pos_keeper keeper(is);
		m_start_pos = is.tellg();
		is.seekg(0, std::ios::end);
		m_size = is.tellg() - m_start_pos;
	}

	istream_wrapper::istream_wrapper(std::istream& is, std::size_t offs, std::size_t len)
		: m_stream(is),
		  m_reader(is, io::binary_reader::LITTLE_ENDIAN_BYTE_ORDER),
		  m_size(len) {
		m_old_pos = is.tellg();
		is.seekg(offs, std::ios::beg);
		m_start_pos = offs;
	}

	istream_wrapper::~istream_wrapper() {
		if (m_old_pos) {
			m_stream.seekg(*m_old_pos, std::ios::beg);
		}
	}

	std::streampos istream_wrapper::current_pos() const {
		return m_stream.tellg() - m_start_pos;
	}

	std::streamsize istream_wrapper::size_to_end() const {

		return m_size - current_pos();
	}

	std::streamsize istream_wrapper::size() const {
		return m_size;
	}

	void istream_wrapper::advance(std::streampos delta) {
		m_stream.seekg( delta, std::ios::cur);
		if (!m_stream) {
			RAISE_EX("I/O error");
		}
	}

	void istream_wrapper::_seek(std::streampos pos, bool truncate) {
		m_stream.seekg(m_start_pos + pos, std::ios::beg);
		if (!m_stream) {
			if (truncate) {
				m_stream.seekg(m_start_pos, std::ios::end);
				return;
			}
			RAISE_EX("I/O error");
		}
	}

	void istream_wrapper::seek(std::streampos pos) {
		_seek(pos, false);
	}

	void istream_wrapper::read(char* buff, std::size_t size) {
		m_stream.read(buff, size);
		if (!m_stream) {
			RAISE_EX("I/O error");
		}
	}

	void istream_wrapper::assert_word(uint16_t word) {
		uint16_t w;
		m_reader >> w;
		if (w != word) {
			RAISE_EX("expected ", word, " actual ", w);
		}
	}

	void istream_wrapper::assert_dword(uint32_t word) {
		uint32_t w;
		m_reader >> w;
		if (w != word) {
			RAISE_EX("expected ", word, " actual ", w);
		}
	}

	void istream_wrapper::assert_string(const wchar_t* s, bool align) {
		const std::size_t n = wcslen(s);
		std::vector <wchar_t> d(n + 1, 0);
		(*this) >> d;
		for (std::size_t i = 0; i <= n; i++) {
			const auto e = s[i];
			const auto a = d[i];
			if (a != e && a != (e ^ 0x20)) {
				RAISE_EX("expected ", bsw::wstring_to_utf8 (s), " actual ", bsw::wstring_to_utf8 (d.data()));
			}
		}
		if (align) {
			align4();
		}
	}

	void istream_wrapper::assert_string(const char* s, bool align) {
		const std::size_t n = strlen(s);
		std::vector <char> d(n + 1, 0);
		(*this) >> d;
		for (std::size_t i = 0; i <= n; i++) {
			const auto e = s[i];
			const auto a = d[i];
			if (a != e) {
				RAISE_EX("expected ", s, " actual ", d.data());
			}
		}
		if (align) {
			align4();
		}
	}

	bool istream_wrapper::check_string(const wchar_t* s, bool align) {
		uint64_t curr = current_pos();
		const std::size_t n = wcslen(s);
		std::vector <wchar_t> d(n + 1, 0);
		(*this) >> d;
		for (std::size_t i = 0; i <= n; i++) {
			const auto e = s[i];
			const auto a = d[i];
			if (a != e && a != (e ^ 0x20)) {
				seek(curr);
				return false;
			}
		}
		if (align) {
			align4();
		}

		return true;
	}

	bool istream_wrapper::check_string(const char* s, bool align) {
		uint64_t curr = current_pos();
		const std::size_t n = strlen(s);
		std::vector <char> d(n + 1, 0);
		(*this) >> d;
		for (std::size_t i = 0; i <= n; i++) {
			const auto e = s[i];
			const auto a = d[i];
			if (a != e) {
				seek(curr);
				return false;
			}
		}
		if (align) {
			align4();
		}

		return true;
	}

	void istream_wrapper::align4() {
		uint64_t x = current_pos();
		x += 3;
		x &= ~(uint64_t)3;
		_seek(x, true);
	}

	void istream_wrapper::assert_space(uint64_t s) const {
		const uint64_t a = this->size_to_end();
		if (s > a) {
			RAISE_EX("Not enough space. requested ", s, " actual ", a);
		}
	}

	void istream_wrapper::read_string(std::wstring& result, std::size_t n, bool ensure_null) {
		if (n == 0) {
			return;
		}
		bool has_null = false;
		for (std::size_t i = 0; i < n; i++) {
			wchar_t x;
			(*this) >> x;

			if (x) {
				result += x;
			} else {
				has_null = true;
				break;
			}
		}
		if (ensure_null && !has_null) {
			RAISE_EX("Bad string length");
		}
	}

	void istream_wrapper::read_string(std::string& result, std::size_t n, bool ensure_null) {
		if (n == 0) {
			return;
		}
		bool has_null = false;
		for (std::size_t i = 0; i < n; i++) {
			char x;
			(*this) >> x;

			if (x) {
				result += x;
			} else {
				has_null = true;
				break;
			}
		}
		if (ensure_null && !has_null) {
			RAISE_EX("Bad string length");
		}
	}
} // ns spy
