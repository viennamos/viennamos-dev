# - Find ViennaGrid
#
# Defines the following if found:
#   VIENNAGRID_FOUND        : TRUE if found, FALSE otherwise
#   VIENNAGRID_INCLUDE_DIRS : Include directories 
#
# Module looks for the path provided by the environment variable
#   VIENNAGRIDPATH


FIND_PATH(VIENNAGRID_DIR 
          NAMES viennagrid/forwards.hpp
          PATHS $ENV{VIENNAGRIDPATH}
          )

SET(VIENNAGRID_INCLUDE_DIRS ${VIENNAGRID_DIR})

find_package_handle_standard_args(ViennaGrid DEFAULT_MSG VIENNAGRID_DIR)

mark_as_advanced(VIENNAGRID_DIR)


