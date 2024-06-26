//
// Created by igor on 7/1/24.
//
#include <iostream>
#include <assets/assets.hh>

int main(int argc, char* argv[]) {

	if (argc != 2)
	{
		std::cerr << "USAGE: " << argv[0] << " <path to exe file>" << std::endl;
		return 1;
	}
	try {
		using namespace neutrino::assets;

		data_manager<> dm;

		world_fs_resolver resolver(argv[1], dm.get_data_loader<neutrino::sdl::surface>());


		auto w = dm.load<world>(resolver.get_stream(), resolver);

	} catch (const bsw::exception& e) {
		std::cerr << e.what() << std::endl;
		e.trace().print(std::cerr, true);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}