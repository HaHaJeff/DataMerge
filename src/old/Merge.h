#ifndef MERGE_H
#define MERGE_H

// for open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// for opendir
#include <sys/types.h>
#include <dirent.h>


// for lseek
#include <sys/types.h>
#include <unistd.h>

// for mmap
#include <sys/mman.h>

// for sendfile
#include <sys/sendfile.h>

// for clock_t
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>

// for strstr
#include <string.h>

#include <vector>
#include <string>
#include <cassert>

#define MAP 1

using namespace std;

const string VERSION = "1.0.0";

class Time {
	public:
		explicit Time() {
			clktck_ = (double)sysconf(_SC_CLK_TCK);
			start_ = times(&t_start_);
		}

		void print() {
			printf("real: %7.2f, user: %7.2f, system: %7.2f\n", (end_ - start_)/(double)clktck_, (t_end_.tms_utime - t_start_.tms_utime)/(double)clktck_, (t_end_.tms_stime - t_start_.tms_stime)/(double)clktck_ );
		}

		~Time() {
			end_ = times(&t_end_);
			print();
		}
	private:
		struct tms t_start_;
		struct tms t_end_;
		clock_t start_;
		clock_t end_;
	    long clktck_;
};

typedef int INT;
typedef unsigned long ULONG ;
typedef char CHAR;
typedef void VOID;
typedef bool BOOL;

/* DESCRIPTION: struct which used to descript the file description map to memory
 * PARA:    	$1 file description, $2 file size, $
 * */
typedef struct FileArg {
	INT   fFd;
	ULONG fSize;
	CHAR* fName;
	VOID* fBuf;
}stFileArg;

/* FUNCTION: 	MemoryMap
 * DESCRIPTION: map $1 to memory
 * PARA: 		$1 fd
 * RETURN:      start addr of fd	
 * */
void* MemoryMap(INT);

/* FUNCTION: 	MemoryUnMap
 * DESCRIPTION: unmap $1->fFd
 * PARA: 		$1 stFileArg
 * RETURN:      
 * */
INT MemoryUnMap(stFileArg*);

/* FUNCTION: 	SetFileArg
 * DESCRIPTION: find minimal fd in $1->fd[i](=0), set fd's size and map to memory
 * PARA: 		$1 ADDR, $2 fd, $3 fName, $4 if flag is true, call mmap
 * RETURN:  	
 * */
void SetFileArg(stFileArg*, INT, CHAR*, BOOL);

/* FUNCTION: 	Print
 * DESCRIPTION: print infomation
 * PARA: 		$1 ADDR
 * RETURN:  	
 * */
void Print(stFileArg*);

/* FUNCTION: 	Perror
 * DESCRIPTION: wrap for perror
 * PARA:        $1 is error information, $2 is a flag to identify whether call perror
 * RETURN: 	    
 * */
void Perror(CHAR*, BOOL);

/* FUNCTION: 	GetFileSize
 * DESCRIPTION: get $1 size
 * PARA: 		$1 fd
 * RETURN:      file size	
 * */
off_t GetFileSize(INT);
#endif
