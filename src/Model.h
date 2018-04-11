#ifndef MODEL_H
#define MODEL_H

#include "Common.h"

#include "Block.h"

void operator+=(MapStrOff& lref, MapStrOff& rref);

MapIntSPBlock::iterator operator+(MapIntSPBlock::iterator& lref, INT offset);
//CModel means an output file
class CModel{

public:
	explicit CModel(const CHAR* fName);
	explicit CModel(const std::string& fName);

	VOID Update();
    VOID PrintSelf(std::ostream&);

	VOID Print(std::ostream&);

    const MapIntSPBlock& GetMapIntSPBlock() const;
    const MapStrOff&     GetMapStrOff() const;
    const CTopology&             GetTopo() const;

    OFF_T Write(const std::string& baseFileName);

    //RETURN count BLOCKs SIZE
    OFF_T WriteBlocks(INT fdOut, INT startIndex, INT endIndex, const std::string& fieldName, OFF_T offset);
    
	void SetIterator(MapIntSPBlock::iterator& it, INT offset);

    OFF_T GetBlocksSize(int start, int end);

	~CModel();

//FUNCTION
private:
	VOID SetMapIntSPBlock_();
	VOID SetMapStrOffset_();
    VOID Construct_();
    CModel (const CModel&);
    CModel& operator=(const CModel&);

//VARIBLES
private:
    SPMapIntSPBlock        spMapIntSPBlock_;
    SPMapStrOff            spMapStrOffset_;
    SPTopo                 spTopo_;
    std::string            fName_; //topo file name
};

#endif

