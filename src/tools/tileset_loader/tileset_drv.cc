#include <iostream>
#include <stdexcept>
#include <fstream>
#include <assets/resources/tileset/tileset_data_loader.hh>
#include <assets/resources/font/rom_font.hh>
#include <bsw/io/binary_reader.hh>
#include <sdlpp/video/color_names.hh>


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
		using namespace neutrino;

		std::cout << "Parsing " << infile << std::endl;
		std::ifstream ifs(infile, std::ios::in | std::ios::binary);
		prographx_resource rc(ifs, true, 0, 50);
		tileset_data_loader dm;
		auto ts = dm.load(rc);
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