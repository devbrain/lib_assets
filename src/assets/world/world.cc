//
// Created by igor on 06/07/2021.
//

#include <algorithm>
#include <assets/resources/world/world.hh>
#include <bsw/exception.hh>

namespace neutrino::assets {
	world::world(orientation_t orientation,
	             render_order_t render_order,
	             const sdl::area_type& world_dims,
	             const sdl::area_type& tile_dims,
	             const sdl::color& bgcolor,
	             unsigned int hex_side_length,
	             stagger_axis_t axis,
	             stagger_index_t index,
	             bool infinite)
		: m_orientation(orientation), m_map_dims(world_dims),
		  m_tile_dims(tile_dims), m_bgcolor(bgcolor),
		  m_render_order(render_order), m_hex_side_length(hex_side_length),
		  m_axis(axis), m_index(index), m_infinite(infinite) {
	}

	world::world(orientation_t orientation, render_order_t render_order, const sdl::area_type& map_dims,
	             const sdl::area_type& tile_dims, const sdl::color& bgcolor)
		: m_orientation(orientation), m_map_dims(map_dims),
		  m_tile_dims(tile_dims), m_bgcolor(bgcolor),
		  m_render_order(render_order), m_hex_side_length(0),
		  m_axis(stagger_axis_t::X), m_index(stagger_index_t::ODD), m_infinite(false) {
	}

	void world::set_empty_tile_id(tile_id_t e) {
		m_empty_tile_id = e;
	}

	tile_id_t world::get_empty_tile_id() const {
		return m_empty_tile_id;
	}

	orientation_t world::get_orientation() const {
		return m_orientation;
	}

	render_order_t world::get_render_order() const {
		return m_render_order;
	}

	sdl::area_type world::get_map_dimension() const {
		return m_map_dims;
	}

	sdl::area_type world::get_tiles_dimension() const {
		return m_tile_dims;
	}

	const sdl::color& world::get_background_color() const {
		return m_bgcolor;
	}

	unsigned int world::get_hex_side_length() const {
		return m_hex_side_length;
	}

	stagger_axis_t world::get_stagger_axis() const {
		return m_axis;
	}

	stagger_index_t world::get_stagger_index() const {
		return m_index;
	}

	bool world::get_infinite() const {
		return m_infinite;
	}

	void world::add_image(image_id_t image_id, sdl::surface image) {
		m_images.insert(std::make_pair(image_id, std::move(image)));
	}

	const sdl::surface& world::get_image(image_id_t image_id) const {
		const auto i = m_images.find(image_id);
		if (i == m_images.end()) {
			RAISE_EX("Can not find image with id ", image_id);
		}
		return i->second;
	}

	void world::add_tile_set(const tiles_set& ts) {
		m_tile_sets.insert(std::upper_bound(m_tile_sets.begin(), m_tile_sets.end(), ts), ts);
	}

	void world::add_animation_sequence(tile_id_t tid, const animation_sequence& aseq) {
		m_animations[tid] = aseq;
	}

	void world::add_layer(const object_layer& layer) {
		m_layers.emplace_back(layer);
	}

	void world::add_layer(const image_layer& layer) {
		m_layers.emplace_back(layer);
	}

	void world::add_layer(const tiles_layer& layer) {
		m_layers.emplace_back(layer);
	}
}
