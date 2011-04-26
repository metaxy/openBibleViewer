# This module looks for clucene (http://clucene.sf.net) support
# It will define the pkgconfig values with a CLUCENE1_ prefix... 
include(FindPkgConfig)
pkg_check_modules(CLUCENE1 libclucene-core)