#include "Merge.h"

VOID Perror(CHAR* errorInfo, BOOL bPerror) {
	if (bPerror) {
		perror(errorInfo);
		exit(-1);
	}
}

VOID Print(stFileArg* pFileArg) {
	INT i = 0;
	printf("**********************************************************\n");
	printf("FileArg->fFd: %d, FileArg->fSize: %ld, FileArg->fName: %s, FileArg->fBuf: %p\n", pFileArg->fFd, pFileArg->fSize, pFileArg->fName, pFileArg->fBuf);
	printf("**********************************************************\n");
}

/*
 * void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
 * int munmap(void *addr, size_t length);
 * */
VOID SetFileArg(stFileArg* pFileArg, INT fd, CHAR* fName, BOOL bMap) {

	VOID* addr = bMap ? MemoryMap(fd) : NULL;
	Perror("SetFileArg-MemoryMap", bMap && NULL == addr);

	CHAR* pName = (CHAR*)malloc(strlen(fName) + 1);
	Perror("SetFileArg-malloc", NULL == pName);
	memcpy(pName, fName, strlen(fName) + 1);

	pFileArg->fFd = fd;
	pFileArg->fSize = GetFileSize(fd);
	pFileArg->fName = pName;
	pFileArg->fBuf = addr;

}

VOID* MemoryMap(INT fd) {
	off_t length = GetFileSize(fd);
	assert(length != -1);
	VOID* addr = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0);
	return addr;
}

INT MemoryUnMap(stFileArg* pFileArg) {
	VOID* addr = pFileArg->fBuf;

	INT ret = munmap(addr, pFileArg->fSize);
	if(-1 == ret) {
		perror("MemoryUnMap");
		return -1;
	} else {
		return 0;
	}
}


off_t GetFileSize(INT fd) {
	struct stat buf;

	INT ret = fstat(fd, &buf);

	if(ret == -1) {
		perror("GetFileSize");
		return -1;
	} else {
		return buf.st_size;
	}
}
