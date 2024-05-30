//
// Created by igor on 5/29/24.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_DETAIL_ISTREAM_POS_KEEPER_HH_
#define ASSETS_INCLUDE_ASSETS_RESOURCES_DETAIL_ISTREAM_POS_KEEPER_HH_

#include <istream>

namespace assets::detail {
	struct istream_pos_keeper {
		std::streampos curr;
		std::istream& stream;

		explicit istream_pos_keeper (std::istream& is)
			: curr (is.tellg ()),
			  stream (is) {
		}

		void rewind () {
			stream.seekg (curr, std::ios::beg);
		}

		~istream_pos_keeper () {
			auto state = stream.rdstate();   // get state
			state &= ~std::ios_base::failbit;  // remove failbit from it
			stream.clear(state);
			rewind ();
		}
	};
}

#endif //ASSETS_INCLUDE_ASSETS_RESOURCES_DETAIL_ISTREAM_POS_KEEPER_HH_
