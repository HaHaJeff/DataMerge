#include "Model.h"

static const int sBlocksOfModel = 12;

void operator+=(MapStrOff& lref, MapStrOff& rref) {
    for (MapStrOff::iterator it = lref.begin(); it != lref.end(); it++) {
        std::string key = it->first;
        lref[key] += rref[key];
    }
}

MapIntSPBlock::iterator operator+(MapIntSPBlock::iterator& lref, INT offset) {
	
	MapIntSPBlock::iterator it = lref;

	for (int i = 0; i < offset; i++) {
		it++;
	}

	return it;
}
/*********************** CModle Start **************************/
/***************************************************************/
/*Public Start*/

CModel::CModel(const CHAR* fName) : fName_(fName)//, dName_(fName) 
{
    Construct_();
}

CModel::CModel(const std::string& fName) : fName_(fName)//, dName_(fName) 
{
    Construct_();
}

VOID CModel::Update() {
    (*spTopo_).Update();
    SetMapIntSPBlock_();
    SetMapStrOffset_();
}

VOID CModel::Print(std::ostream& os) {
    for (MapIntSPBlock::iterator it = spMapIntSPBlock_->begin(); it != spMapIntSPBlock_->end(); it++) {
        (it->second)->Print(os);
    }

    std::cout << "*******************************Base offet start***************************************" << std::endl;
    for (MapStrOff::iterator it = spMapStrOffset_->begin(); it != spMapStrOffset_->end(); it++) {
        std::cout << "name: " << it->first << "\t" << "offset: " << it->second << std::endl;
    }
    std::cout << "*******************************Base offet end*****************************************" << std::endl;
}

const MapIntSPBlock& CModel::GetMapIntSPBlock() const {
    return *spMapIntSPBlock_;
} 

void CModel::SetIterator(MapIntSPBlock::iterator& lref, INT offset) {
	for (int i = 0; i < offset; i++)
		lref++;
}

const MapStrOff& CModel::GetMapStrOff() const {
    return *spMapStrOffset_;
}

const CTopology& CModel::GetTopo() const {
    return *spTopo_;
}

CModel::~CModel() {

}

/*
 * Define a varaible sBlocksOfModel
 * */
OFF_T CModel::Write(const std::string& baseFileName) {
    CFileOperation* fOp = new CFileOperation();

	for (int i = 0; i < spMapIntSPBlock_->size(); i += sBlocksOfModel) {
		INT startIndex = i, endIndex = startIndex + sBlocksOfModel;
		std::string fileName = baseFileName + std::to_string(i/sBlocksOfModel);
    	INT fdOut = fOp->Open(fileName, O_RDWR);

    	fOp->Lseek(fdOut, GetBlocksSize(startIndex, endIndex) );
		WriteBlocks(fdOut, startIndex, endIndex, fileName, 0);
		fOp->Close(fdOut);
	}
    delete(fOp);
	fOp = NULL;
}

OFF_T CModel::WriteBlocks(INT fdOut, INT startIndex, INT endIndex, const std::string& fieldName, OFF_T offset) {
    
    OFF_T startOffset = offset;

	MapIntSPBlock::iterator itBegin = spMapIntSPBlock_->begin();
	MapIntSPBlock::iterator itEnd = spMapIntSPBlock_->begin();

	SetIterator(itBegin, startIndex);
	SetIterator(itEnd, endIndex);
    for (MapIntSPBlock::iterator it = itBegin; it != itEnd; it++)
    {
        CBlock&       block  = *(it->second);
        startOffset += block.Write(fdOut, fieldName, startOffset);
    }

    return startOffset - offset;
}

OFF_T CModel::GetBlocksSize(int startIndex, int endIndex) {
    OFF_T size = 0;

	MapIntSPBlock::iterator itBegin = spMapIntSPBlock_->begin();
	MapIntSPBlock::iterator itEnd = spMapIntSPBlock_->begin();
	SetIterator(itBegin, startIndex);
	SetIterator(itEnd, endIndex);

    for (MapIntSPBlock::iterator it = itBegin; it != itEnd; it++) {
        size += it->second->GetBlockSize();
    } 

    return size;
}

/*Public End*/

/*Private Start*/
VOID CModel::SetMapIntSPBlock_() {
    MapIntStr topo = spTopo_->GetTopo();
    
    std::string dName(fName_);

    dName.erase(dName.find(".topo") + dName.begin(), dName.end());

    for (MapIntStr::iterator it = topo.begin(); it != topo.end(); it++) {
        std::string blockName = it->second;
        SPBlock block = std::make_shared<CBlock>(DirToFileName(dName, blockName));
        block->Update();
     //   std::pair<std::string, SPBlock> p = std::make_pair(blockName, block);
     //   spMapIntSPBlock_->insert(p);
        //spMapIntSPBlock_->insert(std::pair<std::string, SPBlock>(blockName, block));
        spMapIntSPBlock_->insert(std::make_pair(it->first, block));

    }
}

VOID CModel::SetMapStrOffset_() {
    (*spMapStrOffset_) = (spMapIntSPBlock_->begin()->second)->GetMapOffset();

    MapIntSPBlock::iterator it = spMapIntSPBlock_->begin();


    for (it++; it != spMapIntSPBlock_->end(); it++) 
    {
        // XXX: use rref as tempory varible, if use it->second->GetMapOffset() as += parament directly happens bug
        MapStrOff rref = it->second->GetMapOffset();
        (*spMapStrOffset_) +=  rref;
    } 
}

VOID CModel::Construct_() {
    spMapIntSPBlock_ = std::make_shared<MapIntSPBlock>();
    spMapStrOffset_  = std::make_shared<MapStrOff>();
    spTopo_          = std::make_shared<CTopology>(fName_);
}

CModel::CModel(const CModel& lref) {}

CModel& CModel::operator=(const CModel& lref) {}

/*Private End*/
/***************************************************************/
/*********************** CModle End ****************************/
