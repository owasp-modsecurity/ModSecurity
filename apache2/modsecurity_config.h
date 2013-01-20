/* Some APR files define PACKAGE* constants, which may conflict
 * so this is here to prevent that by removing them.
 */

#ifndef	WIN32

/* Undefine all these so there are no conflicts */
#undef PACKAGE
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_URL
#undef PACKAGE_VERSION

/* Include the real autoconf header */
#include "modsecurity_config_auto.h"

/* Undefine all these (again) so there are no conflicts */
#undef PACKAGE
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_URL
#undef PACKAGE_VERSION

#endif
