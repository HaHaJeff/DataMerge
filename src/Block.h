#ifndef BLOCK_H
#define BLOCK_H

#include "Common.h"

#include "File.h"

class CDirOperation {
public:
    explicit CDirOperation();
    INT     MkDir(const std::string& dName, INT mode);
    DIR*    OpenDir(const std::string& dName);
    INT     CloseDir(DIR* pDir);
    INT     Count(DIR* pDir);
    INT     Count(const std::string& dName);
    VecStr  ReadDir(DIR* pDir);
    VecStr  ReadDir(const std::string& dName);
};

class CDirBase {
public:
    explicit CDirBase();
    explicit CDirBase(const std::string& dName);

    virtual ~CDirBase();
    virtual VOID Update();
    virtual VOID PrintSelf(std::ostream& os);

    VOID ChangeDirName(const std::string& dName);
    VOID Print(std::ostream& os);
    
    std::string GetDirName() const;
    VecStr      GetDirFiles() const;
    SIZE_T      GetCount() const;

//FUNCTIONS
protected:
    CDirBase(const CDirBase&);
    CDirBase& operator=(const CDirBase&);

    VOID Construct_();
    VOID Deconstruct_();

    VOID SetDirFiles_();
    VOID SetCount_();

//VARIBLES
protected:
    VecStr          vFiles_;
    std::string     dName_;     //dir name
    CDirOperation*  dOp_;
    SIZE_T          count_;     //files count
};



class CBlock : public CDirBase {

public:
    explicit CBlock();
	explicit CBlock(const std::string& dName);

	virtual VOID Update();
    virtual VOID PrintSelf(std::ostream&);

	VOID Print(std::ostream&);
    OFF_T GetBlockSize();

	MapStrSPField   GetMapStrSPField() const;
	MapStrOff       GetMapOffset() const;
    CTopology       GetTopo() const;

    OFF_T Write(INT fdOut, const std::string& fieldName, OFF_T offset);

	virtual ~CBlock();
//FUNCTIONS
private:
	VOID SetMapStrSPField_();
	VOID SetMapOffset_();
    VOID ReadTopo_();

    CBlock(const CBlock&);
    CBlock& operator=(const CBlock&);

    VOID PrintMap(const MapStrSPField& map);

    VOID Construct_();
//VARIBLES
private:
    SPMapStrSPField           spMapStrSPField_;        //Field is shared_ptr;
    SPMapStrOff               spMapStrOffset_;
    SPTopo spTopo_;

};

typedef std::shared_ptr<CBlock>        SPBlock;
typedef std::map<std::string, SPBlock> MapStrSPBlock;
typedef std::map<INT, SPBlock> MapIntSPBlock;
typedef std::shared_ptr<MapStrSPBlock> SPMapStrSPBlock;
typedef std::shared_ptr<MapIntSPBlock> SPMapIntSPBlock;

#endif
