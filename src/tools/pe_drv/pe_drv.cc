#include <iostream>
#include <stdexcept>
#include <fstream>
#include "mz/pefile/exefile.hh"




int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "USAGE: " << argv[0] << " <path to exe file>" << std::endl;
		return 1;
	}
	using namespace assets::pefile;
	auto infile = argv[1];
	try
	{
		std::cout << "Parsing " << infile << std::endl;
		std::ifstream ifs(infile, std::ios::in | std::ios::binary);
		exe_file_c exe(ifs);
		manifest v;
		exe.load_resource(v);
		
		return 0;
	}
	catch (std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		return 2;
	}

	return 0;
}