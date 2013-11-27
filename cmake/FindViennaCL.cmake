# - Find ViennaCL
#
# Defines the following if found:
#   VIENNACL_FOUND        : TRUE if found, FALSE otherwise
#   VIENNACL_INCLUDE_DIRS : Include directories 
#
# Module looks for the path provided by the environment variable
#   VIENNACLPATH


FIND_PATH(VIENNACL_DIR 
          NAMES viennacl/forwards.h
          PATHS $ENV{VIENNACLPATH}
          )

SET(VIENNACL_INCLUDE_DIRS ${VIENNACL_DIR})

find_package_handle_standard_args(ViennaCL DEFAULT_MSG VIENNACL_DIR)

mark_as_advanced(VIENNACL_DIR)


