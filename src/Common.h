
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <dirent.h>


#include <map>
#include <vector>
#include <memory>    //c++11 for share_ptr
#include <iostream>

#define exit_if(r, ...) if(r) {printf(__VA_ARGS__); printf("%s:%d error no: %d error msg %s\n", __FILE__, __LINE__, errno, strerror(errno)); exit(1);}

typedef int 		    INT;
typedef unsigned long 	ULONG;
typedef char 		    CHAR;
typedef void 		    VOID;
typedef bool 		    BOOL;
typedef size_t 		    SIZE_T;
typedef off_t 	 	    OFF_T;


#endif
