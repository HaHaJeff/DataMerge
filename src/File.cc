#include "File.h"

#include "Common.h"

#include <cstdarg>

/************************ Global Function Start ************************/
/***********************************************************************/
std::string DirToFileName(const std::string& dir, const std::string& file) {
    std::string ret;

    if (dir[dir.size() - 1] != '/') {
        ret = dir + "/" + file;
    } else {
        ret = dir + file;
    }

    return ret;
}
/************************ Global Function End **************************/


/*********************** CFileOperation Start **************************/
/******************************************************************/

/* Public Start */
CFileOperation::CFileOperation() {}

INT CFileOperation::Open(const std::string& fileName, INT flag) {

	assert (!fileName.empty());
	
    if (!Access(fileName)) flag |= O_CREAT;

	INT mode = -1;

	if (0 != (flag & O_CREAT)) {
		//default creaete mode, regular file 
		mode = 0664;
	}

	INT fd = -1;

	if (-1 != mode) {
		fd = open(fileName.c_str(), flag, mode);
	} else {
		fd = open(fileName.c_str(), flag);
	}

	if (-1 == fd) {
		printf("Open %s error\n", fileName.c_str());
		exit(-1);
	}
	return fd;
}

INT CFileOperation::Open(const CHAR* fileName, INT flag) {

	assert (NULL != fileName);
	
	INT mode = -1;

    if (!Access(fileName)) flag |= O_CREAT;

	if (0 != flag & O_CREAT) {
		//default creaete mode, regular file 
		mode = 0664;
	}

	INT fd = -1;

	if (-1 != mode) {
		fd = open(fileName, flag, mode);
	} else {
		fd = open(fileName, flag);
	}

	if (-1 == fd) {
		printf("Open %s error\n", fileName);
		exit(-1);
	}
	return fd;
}

VOID CFileOperation::Close(const INT fd) {

	if (0 == Valid(fd)) {
		close (fd);
	}
	
}

INT CFileOperation::Write(const INT fd, const VOID* pBuf, SIZE_T size) {

	assert (pBuf != NULL);
	assert (size > 0);
	assert (0 == Valid(fd));
	return write(fd, pBuf, size);

}

INT CFileOperation::Read(const INT fd, VOID* pBuf, SIZE_T size) {

	assert (NULL != pBuf);
	assert (size > 0);
	assert (0 == Valid(fd));
	return read(fd, pBuf, size);
}

INT CFileOperation::Fprintf(FILE* fp, const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(fp, format, args);
    va_end(args);
}

INT CFileOperation::Fscanf(FILE* fp, const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfscanf(fp, format, args);
    va_end(args);
}

INT CFileOperation::Lseek(const INT fd, OFF_T offset) {
	
	assert (0 == Valid(fd));
	assert (offset >= 0);
	return lseek(fd, offset, SEEK_SET);
}

VOID* CFileOperation::MemoryMap(const INT fd) {

	assert (0 == Valid(fd) );

	OFF_T length = Size(fd);
	assert(length != -1);

	VOID* addr = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0);
	return addr;

}

VOID CFileOperation::MemoryUnMap(const INT fd, VOID* addr) {

	assert (0 == Valid(fd));
	assert (NULL != addr);
	
	SIZE_T size = Size(fd);

	munmap(addr, size);
}

INT CFileOperation::Valid(const INT fd) {

	if (-1 == fcntl(fd, F_GETFL)) {
		printf("%d file description is invalid\n", fd);
		return -1;
	} else {
		return 0;
	}

}

OFF_T CFileOperation::Size(INT fd) {
	struct stat buf;

	INT ret = fstat(fd, &buf);

	if(ret == -1) {
		printf("%d file description is error\n", fd);
		return -1;
	} else {
		return buf.st_size;
	}
}

BOOL CFileOperation::Access(const std::string& fName) {
    return 0 == access(fName.c_str(), F_OK) ? true : false;
}

/* Public End */

/*********************** CFileOperation End ***************************/
/******************************************************************/


/*********************** CFileBase Start **************************/
/******************************************************************/

/* Public Start*/
CFileBase::CFileBase() : fName_(""), fOp_(NULL){
    Construct_();
}

CFileBase::CFileBase(const std::string& fName) : fName_(fName), fOp_(NULL){
	Construct_();
}

CFileBase::~CFileBase() {

	Deconstruct_();

}

const INT CFileBase::GetFd() const {
	return fFd_;	
}

const std::string CFileBase::GetFileName() const {
	return fName_;
}

const ULONG CFileBase::GetFileSize() const {
	return fSize_;
}

VOID CFileBase::ChangeFileName(const std::string& fName) {
   fName_ = fName;
   Construct_();
}

//virtual
VOID CFileBase::Update() {

    assert (!fName_.empty());
    fFd_ = fOp_->Open(fName_, O_RDWR);
    SetFileSize_();

}

//virtual
VOID CFileBase::PrintSelf(std::ostream& os) {
    
    os << "------CFileBase-----" << std::endl;
    os << "file name: " << fName_ << std::endl;
    os << "open fd: " << fFd_ << std::endl;
    os << "file size: " << fSize_ << std::endl;
    os << "--------------------" << std::endl;

}

VOID CFileBase::Print(std::ostream& os) {
    
    assert(NULL != os);
    PrintSelf(os);

}
/* Public End*/

/*Protected Start*/
VOID CFileBase::Deconstruct_() {

	if (-1 != fFd_) {
		close(fFd_);
		fFd_ = -1;
	}

	if (NULL != fOp_) {
		delete(fOp_);
		fOp_ = NULL;
	}

}

VOID CFileBase::Construct_() {

    if (fOp_ == NULL) {
        fOp_ = new CFileOperation;
    }
    fFd_ = -1;
    fSize_ = 0;
}

VOID CFileBase::SetFd_() {
	
	if (-1 != fFd_ && !fName_.empty()) {
		fFd_ = fOp_->Open(fName_, O_RDONLY);
	}

}

VOID CFileBase::SetFileSize_() {
		
	if (0 == fSize_) {
		fSize_ = fOp_->Size(fFd_);	
	}

}
/*Protected End*/
/******************************************************************/
/*********************** CFileBase End ****************************/


/*********************** CFileSingleField Start **************************/ /*************************************************************************/

/*Public Start*/
CFileSingleField::CFileSingleField() : CFileBase() {
    fBuf_ = NULL;
}

CFileSingleField::CFileSingleField(const CHAR* fName):CFileBase(fName) {
    
    fBuf_ = NULL;
}

CFileSingleField::CFileSingleField(const std::string& fName):CFileBase(fName) {
    fBuf_ = NULL;
}

CFileSingleField::~CFileSingleField() {

    if (NULL != fBuf_) {
        MemoryUnMap_();
    }

}

//virtual
VOID CFileSingleField::Update() {

    CFileBase::Update();
    MemoryMap_();

}

//virtual
VOID CFileSingleField::PrintSelf(std::ostream& os) {

    CFileBase::PrintSelf(os);
    os << "-----CFileSingleField-----" << std::endl;
    os << "fBuf_: " << fBuf_ << std::endl;
    os << "--------------------------" << std::endl;

}

VOID CFileSingleField::Print(std::ostream& os) {

    assert(NULL != os);
    PrintSelf(os);

}

OFF_T CFileSingleField::Write(INT fdOut, OFF_T offset) {
    assert(0 == fOp_->Valid(fdOut));
    assert(-1 != fOp_->Lseek(fdOut, offset));    
    return fOp_->Write(fdOut, fBuf_, fSize_);
}
/*Public End*/


/*Private Start*/
VOID CFileSingleField::MemoryMap_() {
    fBuf_ = fOp_->MemoryMap(fFd_);  
}

VOID CFileSingleField::MemoryUnMap_() {
   fOp_->MemoryUnMap(fFd_, fBuf_); 
}
/*Private End*/
/*************************************************************************/
/*********************** CFileSingleField End ****************************/



/*********************** CTopology Start **************************/
/******************************************************************/
/*Public Start*/
CTopology::CTopology() : CFileBase() {
}

CTopology::CTopology(const std::string& dName) : CFileBase(dName) {
}

//Virtual
CTopology::~CTopology() {
}

//Virtual
VOID CTopology::Update() {
    assert(!fName_.empty());
    CFileBase::Update();
    Read();
}

//Virtual
VOID CTopology::PrintSelf(std::ostream& os) {
    CFileBase::PrintSelf(os);
    os << "------CTopology------" << std::endl;
    for(MapIntStr::iterator it = topo_.begin(); it != topo_.end(); it++) {
        os << "key: " << it->first << "\t" << "value: " << it->second << std::endl;
    }
    os << "---------------------" << std::endl;
}

VOID CTopology::Print(std::ostream& os) {
    assert(NULL != os);
    PrintSelf(os);
}

const MapIntStr CTopology::GetTopo() const {
    return topo_;
}

VOID CTopology::Read() {
    assert(!fName_.empty());
    
    FILE* fp = fdopen(fFd_, "r");
    assert(NULL != fp);
    
    while(!feof(fp)) {
        INT id = 0;
        char buf[32] = {};
        fOp_->Fscanf(fp, "%d %s\n", &id, buf);
        topo_.insert(std::pair<INT, std::string>(id, std::string(buf))); 
    }

	fclose(fp);
}

VOID CTopology::Write(MapIntStr& buf) {
    assert(!fName_.empty()); 
    
    if (fFd_ == -1) fFd_ = fOp_->Open(fName_, O_RDWR); 

    FILE* fp = fdopen(fFd_, "w+");
    assert(NULL != fp);
    
    for(MapIntStr::iterator it = buf.begin(); it != buf.end(); it++) {
        fOp_->Fprintf(fp, "%d %s\n", it->first, (it->second).c_str());
    }
    fclose(fp);
}
/*Public End*/

/*********************** CTopology End ****************************/
/******************************************************************/
