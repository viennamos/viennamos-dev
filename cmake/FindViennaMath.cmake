# - Find ViennaMath
#
# Defines the following if found:
#   VIENNAMATH_FOUND        : TRUE if found, FALSE otherwise
#   VIENNAMATH_INCLUDE_DIRS : Include directories 
#
# Module looks for the path provided by the environment variable
#   VIENNAMATHPATH


FIND_PATH(VIENNAMATH_DIR 
          NAMES viennamath/forwards.h
          PATHS $ENV{VIENNAMATHPATH}
          )

SET(VIENNAMATH_INCLUDE_DIRS ${VIENNAMATH_DIR})

find_package_handle_standard_args(ViennaMath DEFAULT_MSG VIENNAMATH_DIR)

mark_as_advanced(VIENNAMATH_DIR)


