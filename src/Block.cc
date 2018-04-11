#include "Block.h" 
/*********************** CDirOperation Start **************************/
/**********************************************************************/
/*Public Start*/
CDirOperation::CDirOperation() {

}

INT CDirOperation::MkDir(const std::string& dName, INT mode) {
    if (mkdir(dName.c_str(), 0775)) {
        std::cout << "mkdir " << dName << " error!" << std::endl;
        return -1;
    }
    return 0;
}

DIR* CDirOperation::OpenDir(const std::string& dName) {
    DIR* pDir = NULL;
    pDir = opendir(dName.c_str());

    if (NULL == pDir) {
        std::cout << "open dir " << dName << " error!" << std::endl;
    }

    return pDir;
}

INT CDirOperation::CloseDir(DIR* pDir) {

    if (NULL == pDir) {
        std::cout << "close dir(pDir is NULL) error!" << std::endl;
        return -1;
    }

    if (-1 == closedir(pDir)) {
        std::cout << "close dir " << " error!" << std::endl;
        return - 1;
    } 
    return 0;
}

INT CDirOperation::Count(DIR* pDir) {

    assert (pDir != NULL);

    struct dirent *ptr = NULL;
    struct stat buf;
    INT count = 0;

    while ((ptr = readdir(pDir)) != NULL) {
        lstat(ptr->d_name, &buf);

        if (S_ISDIR(buf.st_mode))
            continue;
        else 
            count += 1;
    }

    return count; 
}

INT CDirOperation::Count(const std::string& dName) {

    DIR* pDir = OpenDir (dName);

    return Count(pDir);
}

VecStr CDirOperation::ReadDir(DIR* pDir) {

    assert (pDir != NULL);

    VecStr ret;  

    struct dirent *ptr = NULL;

    while ((ptr = readdir(pDir)) != NULL) {
        if (strcmp(ptr->d_name, ".") == 0 ||
                strcmp(ptr->d_name, "..") == 0)
            continue;

        ret.push_back(ptr->d_name);
    }

    return ret;
}

VecStr CDirOperation::ReadDir(const std::string& dName) {
    DIR* pDir = OpenDir(dName);

    VecStr ret = ReadDir(pDir);

    CloseDir(pDir);

    return ret;
}

/*Public End*/
/*********************** CDirOperation End ****************************/
/**********************************************************************/






/*********************** CDirBase Start *******************************/
/**********************************************************************/
/*Public Start*/
CDirBase::CDirBase() : dOp_(NULL) {
    Construct_();
}

CDirBase::CDirBase(const std::string& dName) : dName_(dName), dOp_(NULL){
    Construct_();
} 

//Virtual
CDirBase::~CDirBase() {
    Deconstruct_();
}

//Virtual
VOID CDirBase::Update() {
    SetDirFiles_();
    SetCount_();
}

//Virtual
VOID CDirBase::PrintSelf(std::ostream& os) {

    os << "---------CDirBase---------" << std::endl;
    os << "dir name: " << dName_ << "\thas " << count_ << " files:"<< std::endl;
    for (VecStr::iterator iter = vFiles_.begin(); iter != vFiles_.end(); iter++) {
        os << *iter << "\t";
    }
    os << std::endl;

    os << "--------------------------" << std::endl;
}

VOID CDirBase::ChangeDirName(const std::string& dName) {
    dName_ = dName;
    Construct_();
}

VOID CDirBase::Print(std::ostream& os) {
    assert(NULL != os);
    PrintSelf(os);
}

std::string CDirBase::GetDirName() const {
    return dName_;
}

VecStr CDirBase::GetDirFiles() const {
    return vFiles_;
}

SIZE_T CDirBase::GetCount() const {
    return count_;
}
/*Public End*/


/*Protected Start*/
CDirBase::CDirBase(const CDirBase&) {
}

CDirBase& CDirBase::operator=(const CDirBase&) {
}

VOID CDirBase::Construct_() {
    count_ = -1;
	if (NULL == dOp_) {
		dOp_ = new CDirOperation();
	}
}

VOID CDirBase::Deconstruct_() {
    if (NULL != dOp_) {
        delete(dOp_);
        dOp_ = NULL;
    }
}

VOID CDirBase::SetDirFiles_() {
    assert(!dName_.empty());
    vFiles_ = dOp_->ReadDir(dName_);
}

VOID CDirBase::SetCount_() {
    count_ = vFiles_.size();
}

/*Protected End*/

/*********************** CDirBase End *********************************/
/**********************************************************************/





/*********************** CBlock Start **************************/
/***************************************************************/
/*Public Start*/
CBlock::CBlock() : CDirBase() {
    Construct_();
}

CBlock::CBlock(const std::string& dName) : CDirBase(dName) {
    Construct_();
}

//Virtual
VOID CBlock::Update() {

    if (GetCount() != 0) {

        CDirBase::Update();
        ReadTopo_();
        SetMapStrSPField_();
        SetMapOffset_();
    }

}

//Virtual
VOID CBlock::PrintSelf(std::ostream& os) {

    CDirBase::PrintSelf(os);

    os << "-----CBlock------" << std::endl;
    os << "map field file:" << std::endl;

    MapStrSPField &tmp = *spMapStrSPField_;
    for (MapStrSPField::iterator it = spMapStrSPField_->begin(); it !=  spMapStrSPField_->end(); it++) {
        (it->second)->Print(os);
   }
    os << "-----------------" << std::endl;
}

VOID CBlock::Print(std::ostream& os) {
    assert(NULL != os);
    PrintSelf(os);
}

OFF_T CBlock::GetBlockSize() {
    OFF_T size = 0;
    for (MapStrSPField::iterator it = spMapStrSPField_->begin(); it !=spMapStrSPField_->end(); it++) {
        size += it->second->GetFileSize();
    }

    return size;
}

MapStrSPField CBlock::GetMapStrSPField() const {
    return *spMapStrSPField_;
}

MapStrOff CBlock::GetMapOffset() const {
    return *spMapStrOffset_;
}

CTopology CBlock::GetTopo() const {
    return *spTopo_;
}

//Virtual
CBlock::~CBlock() {
}
/*Public End*/

/*Private Start*/
VOID CBlock::SetMapStrSPField_() {

    OFF_T offset = 0;
    MapIntStr topo = spTopo_->GetTopo();
    for (MapIntStr::iterator it = topo.begin(); it != topo.end(); it++) {

        std::string fileName = it->second;
        SPField field   = std::make_shared<CFileSingleField>(DirToFileName(dName_, fileName));
        field->Update();

        spMapStrSPField_->insert(std::pair<std::string, SPField>(fileName, field));
        
        spMapStrOffset_->insert(std::pair<std::string, OFF_T>(fileName, offset));
        offset += field->GetFileSize();
    } 
}

VOID CBlock::SetMapOffset_() {

}

VOID CBlock::ReadTopo_() {
    
    spTopo_->Update();
    
}

CBlock::CBlock(const CBlock& lref) {}

CBlock& CBlock::operator=(const CBlock& lref) {}

VOID CBlock::Construct_() {
    spMapStrSPField_   = std::make_shared<MapStrSPField>();
    spMapStrOffset_    = std::make_shared<MapStrOff>();
    
    spTopo_            = std::make_shared<CTopology>(DirToFileName(dName_, ".topo"));
}

OFF_T CBlock::Write(INT fdOut, const std::string& fieldName, OFF_T offset) {
    MapStrSPField& mapStrSPField = *spMapStrSPField_;
    MapStrOff&  mapStrOffset  = *spMapStrOffset_;
    OFF_T size = 0;

    for (MapStrOff::iterator iter = mapStrOffset.begin(); iter != mapStrOffset.end(); iter++) {
        OFF_T startOffset = iter->second + offset;
        size += mapStrSPField[iter->first]->Write(fdOut, startOffset);
    }
    return size; 
}

/*Private Edn*/
/***************************************************************/
/*********************** CBlock End ****************************/
