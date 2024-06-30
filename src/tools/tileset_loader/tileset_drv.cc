#include <iostream>
#include <stdexcept>
#include <fstream>
#include <assets/resources/tileset/tileset_data_loader.hh>
#include <bsw/io/binary_reader.hh>

#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

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
		tileset_data_loader dm;
		auto ts = dm.load(ifs);
		ts.get_surface().save_bmp("zopa.bmp");
		return 0;
	}
	catch (std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		return 2;
	}

	return 0;
}