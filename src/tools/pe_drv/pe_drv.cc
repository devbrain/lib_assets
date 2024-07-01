#include <iostream>
#include <stdexcept>
#include <fstream>
#include <assets/assets.hh>
#include <bsw/io/binary_reader.hh>


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
		data_manager<> dm;
		auto rd = dm.load<windows_resource_directory>(ifs);
		for (auto i = rd.names_begin(); i != rd.names_end(); ++i) {
			if (i->id()) {
				std::cout << to_string(static_cast<windows_resource_type>(i->id())) << std::endl;
			} else {
				std::cout << *i << std::endl;
			}
			for (auto j = rd.begin(*i); j != rd.end(*i); ++j) {
				std::cout << "\t" << j->first << std::endl;
			}
		}
		auto v = rd.load<windows_rs_version>();
		return 0;
	}
	catch (std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		return 2;
	}

	return 0;
}