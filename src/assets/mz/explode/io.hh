#ifndef EXPLODE_IO_HH_
#define EXPLODE_IO_HH_

#include <cstdio>
#include <cstddef>
#include <vector>

#include <sys/types.h>

namespace assets::mz::io
{
  typedef off_t offset_type;

  class input
  {
  public:
    input ();
    virtual ~input ();
    virtual void read_buff (char* buffer, std::size_t size) = 0;
    virtual offset_type tell () = 0;
    virtual offset_type bytes_remains () = 0;
    virtual void seek (offset_type offset) = 0;

    template <typename T>
    void read (T& x)
    {
      union 
      {
	char* bytes;
	T*    words;
      } u;
      u.words = &x;
      this->read_buff (u.bytes, sizeof (T));
    }
  

  private:
    input (const input&);
    input& operator = (const input&);
  };

  // ============================================================

  class  output
  {
  public:
    output ();
    virtual ~output ();
    virtual void write_buff (const char* buffer, std::size_t size) = 0;
    virtual offset_type tell () = 0;
    virtual void seek (offset_type offset) = 0;

    template <typename T>
    void write (const T& x)
    {
      union 
      {
		const char* bytes;
		const T*    words;
      } u;
      u.words = &x;
      write_buff (u.bytes, sizeof (T));
    }


  private:
    output (const output&);
    output& operator = (const output&);
  };

  // ============================================================

  class file_input : public input
  {
  public:
    explicit file_input (const char* path);
    explicit file_input (FILE* file);

    ~file_input () override;

    void read_buff (char* buffer, std::size_t size) override;
    offset_type tell () override;
    offset_type bytes_remains () override;
    void seek (offset_type offset) override;

  private:
    bool m_owner;
    FILE* m_file;
  };
  // ============================================================
  class inmem_input : public input
  {
  public:
	  inmem_input(const unsigned char* data, std::size_t size);
	  
	  void read_buff (char* buffer, std::size_t size) override;
	  offset_type tell() override;
	  offset_type bytes_remains() override;
	  void seek(offset_type offset) override;
  private:
	  const unsigned char* m_data;
	  const std::size_t    m_size;
	  std::size_t          m_ptr;
  };

  // ============================================================
  class file_output : public output
  {
  public:
    explicit file_output (const char* path);
    explicit file_output (FILE* file);

    ~file_output () override;

    void write_buff (const char* buffer, std::size_t size) override;
    offset_type tell () override;
    void seek (offset_type offset) override;

  private:
    bool m_owner;
    FILE* m_file;
  };
  // ============================================================
  class inmem_output : public output
  {
  public:
	  explicit inmem_output(std::vector <char>& out_buff);

	  void write_buff (const char* buffer, std::size_t size) override;
	  offset_type tell() override;
	  void seek(offset_type offset) override;

  private:
	  std::vector <char>& m_buff;
	  std::size_t m_ptr;
  };
}

#endif
