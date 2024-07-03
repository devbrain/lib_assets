//
// Created by igor on 7/3/24.
//

#include <assets/resources/world/world_object_layer.hh>

namespace neutrino::assets {
	std::vector<object_t>::const_iterator object_layer::begin() const {
		return m_objects.begin();
	}

	std::vector<object_t>::const_iterator object_layer::end() const {
		return m_objects.end();
	}

	bool object_layer::empty() const {
		return m_objects.empty();
	}

	std::size_t object_layer::size() const {
		return m_objects.size();
	}

	void object_layer::add(const object_t& obj) {
		m_objects.emplace_back(obj);
	}
}
