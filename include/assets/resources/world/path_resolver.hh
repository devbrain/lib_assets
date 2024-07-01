//
// Created by igor on 26/07/2021.
//

#ifndef ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_PATH_RESOLVER_HH
#define ASSETS_INCLUDE_ASSETS_RESOURCES_WORLD_PATH_RESOLVER_HH

#include <functional>
#include <string>

namespace neutrino::assets {
  // path resolver accepts path in tmx file and returns content of the file pointed by path
  using path_resolver_t = std::function<std::string (const std::string& path)>;
}

#endif
