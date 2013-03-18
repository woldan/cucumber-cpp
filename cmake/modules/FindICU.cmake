# Locate ICU library to act as optional backed for boost::regex
#
# Defines the following variables:
#
#   ICU_FOUND - Found ICU
#   ICU_INCLUDE_DIR - Include directories
#   ICU_LIBRARIES - Dynamic libraries
#

find_path(
  ICU_INCLUDE_DIRS unicode/utypes.h
  HINTS ${ICU_ROOT}/include
        /opt/local/include
        /usr/local/Cellar/icu4c/50.1/include
)

find_library(
  ICU_LIBRARIES
  NAMES icuuc
  HINTS ${ICU_ROOT}/lib
        /opt/local/lib
        /usr/local/Cellar/icu4c/50.1/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  ICU
  DEFAULT_MSG
  ICU_LIBRARIES ICU_INCLUDE_DIRS
)

mark_as_advanced(ICU_INCLUDE_DIRS ICU_LIBRARIES)
