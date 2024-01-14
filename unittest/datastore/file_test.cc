//
// Created by igor on 1/12/24.
//
#include <fstream>
#include <set>
#include <doctest/doctest.h>
#include <assets/datastore/file.hh>
#include <assets/datastore/path.hh>
#include <bsw/predef.h>

using namespace neutrino::assets::datastore;

TEST_SUITE("file test") {

	TEST_CASE("file attribs") {
		file fw ("testfile.dat");
		if (fw.exists ()) {
			fw.remove ();
		}

		SUBCASE("create file") {
			file f ("testfile.dat");
			bool created = f.create_file ();
			REQUIRE (created);
			REQUIRE (!f.is_hidden ());
			created = f.create_file ();
			REQUIRE (!created);
		}

		SUBCASE("file attributes") {
			file f ("testfile.dat");
			REQUIRE (!f.exists ());

			CHECK_THROWS_AS((void)f.can_read (), std::exception);
			CHECK_THROWS_AS((void)f.can_write (), std::exception);
			CHECK_THROWS_AS((void)f.is_file (), std::exception);
			CHECK_THROWS_AS((void)f.is_directory (), std::exception);
			CHECK_THROWS_AS((void)f.get_size (), std::exception);
			CHECK_THROWS_AS((void)f.set_size (0), std::exception);
			CHECK_THROWS_AS((void)f.set_writeable (), std::exception);
			CHECK_THROWS_AS((void)f.set_read_only (), std::exception);
			CHECK_THROWS_AS((void)f.copy_to ("copy.dat"), std::exception);
			CHECK_THROWS_AS((void)f.move_to ("copy.dat"), std::exception);
			CHECK_THROWS_AS((void)f.rename_to ("copy.dat"), std::exception);
			CHECK_THROWS_AS((void)f.remove (), std::exception);
		}

		SUBCASE("file attributes 2") {
			file f ("testfile.dat");
			bool created = f.create_file ();
			REQUIRE (created);

			REQUIRE (f.exists ());
			REQUIRE (f.can_read ());
			REQUIRE (f.can_write ());
			REQUIRE (f.is_file ());
			REQUIRE (!f.is_directory ());

			f.set_writeable (false);
			REQUIRE (!f.can_write ());
			REQUIRE (f.can_read ());

			f.set_read_only (false);
			REQUIRE (f.can_write ());
			REQUIRE (f.can_read ());

		}
	}

	TEST_CASE("compare") {
		file f1 ("abc.txt");
		file f2 ("def.txt");
		file f3 ("abc.txt");

		REQUIRE (f1 == f3);
		REQUIRE (!(f1 == f2));
		REQUIRE (f1 != f2);
		REQUIRE (!(f1 != f3));
		REQUIRE (!(f1 == f2));
		REQUIRE (f1 < f2);
		REQUIRE (f1 <= f2);
		REQUIRE (!(f2 < f1));
		REQUIRE (!(f2 <= f1));
		REQUIRE (f2 > f1);
		REQUIRE (f2 >= f1);
		REQUIRE (!(f1 > f2));
		REQUIRE (!(f1 >= f2));

		REQUIRE (f1 <= f3);
		REQUIRE (f1 >= f3);
	}

	TEST_CASE("test root dir") {
#if !defined(PREDEF_PLATFORM_UNIX)
		File f1("/");
		File f2("c:/");
		File f3("c:\\");
		File f4("\\");
		REQUIRE (f1.exists());
		REQUIRE (f2.exists());
		REQUIRE (f3.exists());
		REQUIRE (f4.exists());
#else
		file f1 ("/");
		REQUIRE (f1.exists ());
#endif
	}

	TEST_CASE("test swap") {
		file f1 ("abc.txt");
		file f2 ("def.txt");
		f1.swap (f2);
		REQUIRE (f1.get_path () == "def.txt");
		REQUIRE (f2.get_path () == "abc.txt");
	}

	TEST_CASE("test size") {

		std::ofstream ostr ("testfile.dat");
		ostr << "Hello, world!" << std::endl;
		ostr.close ();
		file f ("testfile.dat");
		REQUIRE (f.get_size () > 0);
		f.set_size (0);
		REQUIRE (f.get_size () == 0);
		f.remove ();
	}

	TEST_CASE("test directory") {
		file d ("testdir");
		try {
			d.remove (true);
		}
		catch (...) {
		}

		bool created = d.create_directory ();
		REQUIRE (created);
		REQUIRE (d.is_directory ());
		REQUIRE (!d.is_file ());
		std::vector<std::string> files;
		d.list (files);
		REQUIRE (files.empty ());

		file f (path ("testdir/file1", path::PATH_UNIX));
		f.create_file ();
		f = path ("testdir/file2", path::PATH_UNIX);
		f.create_file ();
		f = path ("testdir/file3", path::PATH_UNIX);
		f.create_file ();

		d.list (files);
		REQUIRE (files.size () == 3);

		std::set<std::string> fs;
		fs.insert (files.begin (), files.end ());
		REQUIRE (fs.find ("file1") != fs.end ());
		REQUIRE (fs.find ("file2") != fs.end ());
		REQUIRE (fs.find ("file3") != fs.end ());

		file dd (path ("testdir/testdir2/testdir3", path::PATH_UNIX));
		dd.create_directories ();
		REQUIRE (dd.exists ());
		REQUIRE (dd.is_directory ());

		file ddd (path ("testdir/testdirB/testdirC/testdirD", path::PATH_UNIX));
		ddd.create_directories ();
		REQUIRE (ddd.exists ());
		REQUIRE (ddd.is_directory ());

		d.remove (true);
	}

	TEST_CASE("test copy") {
		std::ofstream ostr ("testfile.dat");
		ostr << "Hello, world!" << std::endl;
		ostr.close ();

		file f1 ("testfile.dat");
		file f2 ("testfile2.dat");
		f1.set_read_only ().copy_to (f2.get_path ());
		REQUIRE (f2.exists ());
		//REQUIRE (!f2.canWrite());
		REQUIRE (f1.get_size () == f2.get_size ());
		f1.set_writeable ().remove ();
		f2.remove ();
	}

	TEST_CASE("test copy fail if destination exists") {
		std::ofstream ostr ("testfile.dat");
		ostr << "Hello, world!" << std::endl;
		ostr.close ();

		file f1 ("testfile.dat");
		file f2 ("testfile2.dat");

		f2.create_file ();
		CHECK_THROWS_AS(f1.set_read_only ().copy_to (f2.get_path (), file::OPT_FAIL_ON_OVERWRITE), std::exception);

		f1.set_writeable ().remove ();
	}

	TEST_CASE("test move") {
		std::ofstream ostr ("testfile.dat");
		ostr << "Hello, world!" << std::endl;
		ostr.close ();

		file f1 ("testfile.dat");
		file::file_size_t sz = f1.get_size ();
		file f2 ("testfile2.dat");
		f1.move_to (f2.get_path ());
		REQUIRE (f2.exists ());
		REQUIRE (f2.get_size () == sz);
		REQUIRE (f1.exists ());
		REQUIRE (f1 == f2);
		f2.remove ();
	}

	TEST_CASE("test move fail if destination exists") {
		std::ofstream ostr ("testfile.dat");
		ostr << "Hello, world!" << std::endl;
		ostr.close ();

		file f1 ("testfile.dat");
		file f2 ("testfile2.dat");
		f2.create_file ();

		CHECK_THROWS_AS(f1.move_to (f2.get_path (), file::OPT_FAIL_ON_OVERWRITE), std::exception);

		f1.set_writeable ().remove ();
	}

	TEST_CASE("test create directory") {
		path pd1 ("testdir");
		file fd1 (pd1);
		try {
			fd1.remove (true);
		}
		catch (...) {
		}
		fd1.create_directories ();
		path pd2 (pd1, "subdir");
		file fd2 (pd2);
		fd2.create_directories ();
		path pf1 (pd1, "testfile1.dat");
		std::ofstream ostr1 (pf1.to_string ().c_str ());
		ostr1 << "Hello, world!" << std::endl;
		ostr1.close ();
		path pf2 (pd1, "testfile2.dat");
		std::ofstream ostr2 (pf2.to_string ().c_str ());
		ostr2 << "Hello, world!" << std::endl;
		ostr2.close ();
		path pf3 (pd2, "testfile3.dat");
		std::ofstream ostr3 (pf3.to_string ().c_str ());
		ostr3 << "Hello, world!" << std::endl;
		ostr3.close ();

		file fd3 ("testdir2");

		try {
			fd3.remove (true);
		}
		catch (...) {
		}

		fd1.copy_to ("testdir2");

		path pd1t ("testdir2");
		file fd1t (pd1t);
		REQUIRE (fd1t.exists ());
		REQUIRE (fd1t.is_directory ());

		path pd2t (pd1t, "subdir");
		file fd2t (pd2t);
		REQUIRE (fd2t.exists ());
		REQUIRE (fd2t.is_directory ());

		path pf1t (pd1t, "testfile1.dat");
		file ff1t (pf1t);
		REQUIRE (ff1t.exists ());
		REQUIRE (ff1t.is_file ());

		path pf2t (pd1t, "testfile2.dat");
		file ff2t (pf2t);
		REQUIRE (ff2t.exists ());
		REQUIRE (ff2t.is_file ());

		path pf3t (pd2t, "testfile3.dat");
		file ff3t (pf3t);
		REQUIRE (ff3t.exists ());
		REQUIRE (ff3t.is_file ());

		fd1.remove (true);
		fd3.remove (true);
	}

	TEST_CASE("test rename") {
		std::ofstream ostr ("testfile.dat");
		ostr << "Hello, world!" << std::endl;
		ostr.close ();

		file f1 ("testfile.dat");
		file f2 ("testfile2.dat");
		f1.rename_to (f2.get_path ());

		REQUIRE (f2.exists ());
		REQUIRE (f1.exists ());
		REQUIRE (f1 == f2);

		f2.remove ();
	}

	TEST_CASE("test rename fails if exists") {
		std::ofstream ostr ("testfile.dat");
		ostr << "Hello, world!" << std::endl;
		ostr.close ();

		file f1 ("testfile.dat");
		file f2 ("testfile2.dat");
		f2.create_file ();

		CHECK_THROWS_AS(f1.rename_to (f2.get_path (), file::OPT_FAIL_ON_OVERWRITE), std::exception);

		f1.rename_to (f2.get_path ());

		REQUIRE(f2.exists ());
		REQUIRE(f1.exists ());
		REQUIRE(f1 == f2);

		f2.remove ();
	}
}