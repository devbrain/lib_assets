//
// Created by igor on 7/2/24.
//
#include <assets/resources/world/world_component.hh>

namespace neutrino::assets {
	bool component::contains(const std::string& name) const noexcept {
		return m_prop.find(name) != m_prop.end();
	}

	bool component::empty() const noexcept {
		return m_prop.empty();
	}

	std::optional <property_t> component::get(const std::string& name) const noexcept {
		auto i = m_prop.find(name);
		if (i == m_prop.end()) {
			return std::nullopt;
		}
		return i->second;
	}
}