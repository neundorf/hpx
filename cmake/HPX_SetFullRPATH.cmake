# Copyright (c) 2011-2014 Thomas Heller
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

set(FULL_LIBDIR "${CMAKE_INSTALL_PREFIX}/lib")
if(DEFINED CMAKE_INSTALL_FULL_LIBDIR)
  set(FULL_LIBDIR "${CMAKE_INSTALL_FULL_LIBDIR}")
endif()

list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${FULL_LIBDIR}" isSystemDir)
if(isSystemDir EQUAL -1)
  set(CMAKE_INSTALL_RPATH "${FULL_LIBDIR}")
endif()
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
