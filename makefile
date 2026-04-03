# ----------------------------
# Makefile Options
# ----------------------------

NAME = SVPort
ICON = icon.png
DESCRIPTION = "New version of my sdv port for TI 84"
COMPRESSED = NO
ARCHIVED = YES

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

EXTRA_C_SOURCES = src/tilemap/tilemap.c


# ----------------------------

include $(shell cedev-config --makefile)

# rule to convert CSV tilemap to C source file
src/tilemap/tilemap.c: src/tilemap/basic_grasspaths_map.csv
	@convbin -j csv -i $< -k c -o $@ -n tilemap_map
