#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(ERRNO)\
	errno = ERRNO;\
	fprintf(stderr, "[ERROR] (%s:%d: errno: %s) \n", __FILE__, __LINE__,\
		clean_errno());

#define log_warn(ERRNO) errno = ERRNO; fprintf(stderr,\
	"[WARN] (%s:%d: errno: %s) \n",\
	__FILE__, __LINE__, clean_errno());

#define log_info(INFO) fprintf(stderr, "[INFO] (%s:%d) " INFO "\n",\
	__FILE__, __LINE__);

#define check(A, ERRNO)\
	if(!(A)) {\
		log_err(ERRNO); errno=0; goto error;\
	} else {\
		errno = 0;\
	}

#endif