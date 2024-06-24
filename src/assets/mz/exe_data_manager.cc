//
// Created by igor on 5/30/24.
//
#include "assets/resources/exe/exe_data_manager.hh"
#include "mz/explode/knowledge_dynamics.hh"
#include "mz/explode/unlzexe.hh"
#include "mz/explode/unpklite.hh"
#include "mz/explode/unexepack.hh"

namespace neutrino::assets {
	enum exe_t {
		KD,
		LZEXE,
		PKLITE,
		EXEPACK
	};
	template <exe_t T>
	struct exe_traits;

	template <>
	struct exe_traits<KD> {
		static constexpr auto name = "KnowledgeDynamics";
		using loader = mz::knowledge_dynamics;
	};

	template <>
	struct exe_traits<LZEXE> {
		static constexpr auto name = "LZEXE";
		using loader = mz::unlzexe;
	};

	template <>
	struct exe_traits<PKLITE> {
		static constexpr auto name = "PKLITE";
		using loader = mz::unpklite;
	};

	template <>
	struct exe_traits<EXEPACK> {
		static constexpr auto name = "EXEPACK";
		using loader = mz::unexepack;
	};

	template <exe_t T>
	class exe_resource_loader : public abstract_resource_loader<std::vector<char>> {
	 private:
		using loader_t = typename exe_traits<T>::loader;

		bool accept(std::istream& is) const override {
			mz::io::file_input input(is);
			mz::input_exe_file iexe(input);
			return loader_t::accept(iexe);
		}

		std::vector<char> load(std::istream& is) const override {
			mz::io::file_input input(is);
			mz::input_exe_file iexe(input);
			loader_t loader(iexe);
			assets::mz::full_exe_file fo(loader.decomp_size());
			loader.unpack(fo);
			std::vector<char> out_buff;
			assets::mz::io::inmem_output ow(out_buff);
			fo.write(ow);
			return out_buff;
		};
	};

	exe_data_manager::exe_data_manager () {
		register_loader (exe_traits<PKLITE>::name, std::make_unique<exe_resource_loader<PKLITE>>());
		register_loader (exe_traits<LZEXE>::name, std::make_unique<exe_resource_loader<LZEXE>>());
		register_loader (exe_traits<EXEPACK>::name, std::make_unique<exe_resource_loader<EXEPACK>>());
		register_loader (exe_traits<KD>::name, std::make_unique<exe_resource_loader<KD>>());
	}
}