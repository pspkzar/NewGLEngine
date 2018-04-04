#Try to find freeglut

find_package(PkgConfig)
pkg_check_modules(PC_FREEGLUT QUIET freeglut)
set(FREEGLUT_DEFINITIONS ${PC_FREEGLUT_CFLAGS_OTHER})

find_path(freeglut_INCLUDE_DIR GL/freeglut.h
        HINTS ${PC_FREEGLUT_INCLUDEDIR} ${PC_FREEGLUT_INCLUDE_DIRS})

find_library(freeglut_LIBRARY NAMES glut libglut
        HINTS ${PC_FREEGLUT_LIBDIR} ${PC_FREEGLUT_LIBRARY_DIRS})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(freeglut  DEFAULT_MSG
        freeglut_LIBRARY freeglut_INCLUDE_DIR)

mark_as_advanced(freeglut_INCLUDE_DIR, freeglut_LIBRARY)

set(freeglut_LIBRARIES ${freeglut_LIBRARY})
set(freeglut_INCLUDE_DIRS ${freeglut_INCLUDE_DIR})