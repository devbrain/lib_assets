add_subdirectory(bin2c)
if (is_top_level)
	add_subdirectory(pe_drv)
	add_subdirectory(show_image)
	add_subdirectory(show_font)
	add_subdirectory(tileset_loader)

	add_subdirectory(tiled)
	add_subdirectory(cc)
	if (NOT WIN32)
		add_subdirectory(semblance)
	endif()
endif()