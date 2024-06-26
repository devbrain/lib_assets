#include <iostream>
#include <stdexcept>
#include <fstream>
#include "mz/win_exe/windows_ne_file.hh"
#include "assets/resources/exe/winres/rs_icon_group.hh"


int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "USAGE: " << argv[0] << " <path to exe file>" << std::endl;
		return 1;
	}
	using namespace neutrino::assets;
	auto infile = argv[1];
	try
	{
		std::cout << "Parsing " << infile << std::endl;
		std::ifstream ifs(infile, std::ios::in | std::ios::binary);
		windows_ne_file exe(ifs);
		//manifest v;
		//exe.load_resource(v);
		windows_resource_directory rd;
		exe.build_resources(rd);
		auto ni = rd.names_begin ();
		auto end = rd.names_end ();
		bool found = false;
		windows_rs_icon_group out;
		for (auto i = ni; i != end; ++i) {

				if (i->is_id () && i->id () == windows_resource_traits<windows_rs_icon_group>::id) {
					for (auto j = rd.begin (*i); j != rd.end (*i); j++) {
						found = true;
						windows_resource_traits<windows_rs_icon_group>::load (exe, j->second, out);
					}
				}
		}
		return 0;
	}
	catch (std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		return 2;
	}

	return 0;
}