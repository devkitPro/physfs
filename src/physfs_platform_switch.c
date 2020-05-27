/*
 * Nintendo Switch support routines for PhysicsFS.
 *
 * Please see the file LICENSE.txt in the source's root directory.
 *
 */


#define __PHYSICSFS_INTERNAL__
#include "physfs_platforms.h"

#ifdef PHYSFS_PLATFORM_SWITCH

#include <switch.h>

// for getcwd
#include <unistd.h>
// for PATH_MAX
#include <limits.h>

#include "physfs_internal.h"

int __PHYSFS_platformInit(void)
{
	return 1;  /* always succeed. */
} /* __PHYSFS_platformInit */


void __PHYSFS_platformDeinit(void)
{
	/* no-op */
} /* __PHYSFS_platformDeinit */


void __PHYSFS_platformDetectAvailableCDs(PHYSFS_StringCallback cb, void *data)
{
	/* no-op */
}

char *__PHYSFS_switchCalcUserDir(void)
{
	/* Use the jail directory (hopefully) found before. */
	return __PHYSFS_strdup(PHYSFS_getBaseDir());
}

char *__PHYSFS_platformCalcBaseDir(const char *argv0)
{
	char *retval = NULL;
	/* As there is no system-specific directory, directly inspect argv0. */
	if (argv0 == NULL)
	{
		/* User did not provide a path, just use the current working directory.
		 *  As physfs should be initialized soon after application start, this
		 *  should give us a useable directory.
		 */
		char fullpath[PATH_MAX];
		if (getcwd(fullpath, sizeof(fullpath)) != NULL)
		{
			const size_t cwdlen = strlen(fullpath);
			/* getcwd does not provide a trailing slash, add it. */
			retval = (char*) allocator.Malloc(cwdlen + 2);
			BAIL_IF(!retval, PHYSFS_ERR_OUT_OF_MEMORY, NULL);
			strncpy(retval, fullpath, cwdlen);
			retval[cwdlen] = '/';
			retval[cwdlen + 1] = '\0';
		}
	}
	else
	{
		/* nx-hbmenu should give us the full path of the application, this may
		 *  reside in a subfolder. Higher level code will strip away the name
		 *  and extension.
		 */
		return NULL;
	}

	if (!retval)
		/* Last resort: use `/switch` directory. */
		retval = __PHYSFS_strdup("/switch/");

	return retval;
} /* __PHYSFS_platformCalcBaseDir */

char *__PHYSFS_platformCalcPrefDir(const char *org, const char *app)
{
	char *retval = NULL;
	size_t len = 0;

	/* Use the jail directory (hopefully) found before. This way we do not
	 *  need to add an application folder, because it is exclusive.
	 */
	const char *envr = __PHYSFS_getUserDir();
	BAIL_IF_ERRPASS(!envr, NULL);
	const char *append = ".config/";
	len = strlen(envr) + strlen(append) + 1;
	retval = (char *) allocator.Malloc(len);
	BAIL_IF(!retval, PHYSFS_ERR_OUT_OF_MEMORY, NULL);
	snprintf(retval, len, "%s%s", envr, append);

	return retval;
} /* __PHYSFS_platformCalcPrefDir */


#endif /* PHYSFS_PLATFORM_SWITCH */

/* end of physfs_platform_switch.cpp ... */
