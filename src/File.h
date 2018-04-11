#ifndef FILE_H
#define FILE_H

#include "Common.h"

std::string DirToFileName(const std::string& dir, const std::string& file);

class CFileOperation {

public:
	explicit CFileOperation();
    
    INT         Open(const std::string& fName, INT mode);
	INT 	    Open(const CHAR* fileName, INT mode);
	VOID 	    Close(const INT fd);
	INT 	    Write(const INT fd, const VOID* pBuf, SIZE_T size);
	INT 	    Read(const INT fd, VOID* pBuf, SIZE_T size);
    INT         Fprintf(FILE* fp, const char* format, ...);
    INT         Fscanf(FILE* fp, const char* foramt, ...);
	INT 	    Lseek(const INT fd, OFF_T offset);   //offset from file header
	VOID* 	    MemoryMap(const INT fd);
	VOID 	    MemoryUnMap(const INT fd, VOID* addr);
	OFF_T 	    Size(const INT fd);
    INT         Valid(const INT fd);
    BOOL        Access(const std::string& fName);

};

class CFileBase {

public:
    explicit CFileBase();
	explicit CFileBase(const std::string& fName);
	virtual ~CFileBase();
	
	const INT           GetFd() const;
	const std::string   GetFileName() const;
	const ULONG         GetFileSize() const;

    VOID ChangeFileName(const std::string& fName);

	virtual VOID Update(); //when object call this function, make fFd_ and fSize sense
    virtual VOID PrintSelf(std::ostream&);

	VOID Print(std::ostream&);

//FUNCIOTNS
protected:
	VOID SetFd_();
	VOID SetFileSize_();
    VOID Construct_();
    VOID Deconstruct_();

//VARIBLES
protected:
	INT             fFd_; 	  //file description
	ULONG           fSize_;   //fize size
    std::string     fName_;   //file name equal file path
	CFileOperation* fOp_;
};

class CFileSingleField : public CFileBase {
	
public:
    explicit CFileSingleField();
	explicit CFileSingleField(const CHAR* fName);
	explicit CFileSingleField(const std::string& fName);
	virtual ~CFileSingleField();

	virtual VOID Update();
    virtual VOID PrintSelf(std::ostream&);

	VOID Print(std::ostream&);
    OFF_T Write(INT fdOut, OFF_T offset);

	const VOID* GetFileBuffer() const;

//FUNCTIONS
private:
	VOID MemoryUnMap_();
	VOID MemoryMap_();	

    CFileSingleField(const CFileSingleField&);
    CFileSingleField& operator=(const CFileSingleField&);

//VARIBLES
private:
	VOID* fBuf_;		
};

typedef std::shared_ptr<CFileSingleField>      SPField;
typedef std::vector<OFF_T> VecOffset;
typedef std::vector<std::string>                VecStr;
typedef std::map<std::string, SPField>          MapStrSPField;
typedef std::map<INT, std::string>              MapIntStr;
typedef std::map<std::string, INT>              MapStrInt;
typedef std::map<std::string, OFF_T>            MapStrOff;

typedef std::shared_ptr<MapStrSPField>          SPMapStrSPField;
typedef std::shared_ptr<VecStr>                 SPVecStr;
typedef std::shared_ptr<MapIntStr>              SPMapIntStr;
typedef std::shared_ptr<MapStrInt>              SPMapStrInt;
typedef std::shared_ptr<MapStrOff>              SPMapStrOff;
typedef std::shared_ptr<MapStrOff>              SPMapStrOff;

class CTopology : public CFileBase {

public:
    explicit CTopology();
	explicit CTopology(const std::string& dName);
    
    virtual ~CTopology();
    virtual VOID Update();
    virtual VOID PrintSelf(std::ostream& os);
    VOID Print(std::ostream& os);
    const MapIntStr GetTopo() const;

	VOID Read();
	VOID Write(MapIntStr&);
private:
    MapIntStr topo_;
};

typedef std::shared_ptr<CTopology>              SPTopo;

#endif
