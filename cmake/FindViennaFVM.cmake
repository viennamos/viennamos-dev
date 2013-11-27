# - Find ViennaFVM
#
# Defines the following if found:
#   VIENNAFVM_FOUND        : TRUE if found, FALSE otherwise
#   VIENNAFVM_INCLUDE_DIRS : Include directories 
#
# Module looks for the path provided by the environment variable
#   VIENNAFVMPATH


FIND_PATH(VIENNAFVM_DIR 
          NAMES viennafvm/forwards.h
          PATHS $ENV{VIENNAFVMPATH}
          )

SET(VIENNAFVM_INCLUDE_DIRS ${VIENNAFVM_DIR})

find_package_handle_standard_args(ViennaFVM DEFAULT_MSG VIENNAFVM_DIR)

mark_as_advanced(VIENNAFVM_DIR)


