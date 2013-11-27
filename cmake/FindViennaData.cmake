# - Find ViennaData
#
# Defines the following if found:
#   VIENNADATA_FOUND        : TRUE if found, FALSE otherwise
#   VIENNADATA_INCLUDE_DIRS : Include directories 
#
# Module looks for the path provided by the environment variable
#   VIENNADATAPATH


FIND_PATH(VIENNADATA_DIR 
          NAMES viennadata/forwards.hpp
          PATHS $ENV{VIENNADATAPATH}
          )

SET(VIENNADATA_INCLUDE_DIRS ${VIENNADATA_DIR})

find_package_handle_standard_args(ViennaData DEFAULT_MSG VIENNADATA_DIR)

mark_as_advanced(VIENNADATA_DIR)


