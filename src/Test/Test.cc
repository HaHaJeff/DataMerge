#include "../File.h"
#include "../Block.h"
#include "../Model.h"

#include <algorithm>

class CTest {
public: virtual void Test() = 0;
};

class CTestFile : public CTest{
public:
    CTestFile(const std::string& fName) : fName_(fName) {
    }
	
	CTestFile(const char* fName) : fName_(fName) {
	}

    virtual void Test() {
        CFileBase file(fName_);
        file.Update();
        file.Print(std::cout);
    }
protected:
	std::string fName_; 
}; 

class CTestField : public CTest {
public:
    CTestField(const std::string& fName) : fName_(fName){
    }
    
	CTestField(const char* fName) : fName_(fName) {
	}
    virtual void Test() {
        CFileSingleField field(fName_);
        field.Update();
        field.Print(std::cout);
    } 
protected:
	std::string fName_; 
};

class CTestDir : public CTest {
public:
    CTestDir(const std::string& fName) : fName_(fName){
    }

    CTestDir(const char* fName) : fName_(fName){
    }
    virtual void Test() {
        CDirBase dir(fName_);
        dir.Update();
        dir.Print(std::cout);
    }
protected:
	std::string fName_; 
};

class CTestBlock : public CTest {
public:
    CTestBlock(const std::string& fName) : fName_(fName){
    }

    CTestBlock(const char* fName) : fName_(fName){
    }
    virtual void Test() {
        CBlock* pBlock = new CBlock(fName_);
        pBlock->Update();
        pBlock->Print(std::cout);

        delete(pBlock);
		pBlock = NULL;
    }
protected:
	std::string fName_; 
};

class CTestModel : public CTest {
public:
    CTestModel(const std::string& fName) : fName_(fName){
    }
    
    CTestModel(const char* fName) : fName_(fName){
    }

    virtual void Test() {
        CModel* pModel = new CModel(fName_);
        pModel->Update();
    //    pModel->Print(std::cout);
        pModel->Write("model");
        delete(pModel);
    }
protected:
	std::string fName_; 
};

class CTestTopo : public CTest {
public:
    CTestTopo(const std::string& fName) : fName_(fName) {
    }

    CTestTopo(const char* fName) : fName_(fName) {
    }
    virtual void Test() {
        CTopology* pTopo = new CTopology(fName_);
      
        MapIntStr data;
        data.insert(std::pair<INT, std::string>(0, "x"));
        data.insert(std::pair<INT, std::string>(1, "y"));
        data.insert(std::pair<INT, std::string>(2, "z"));
        data.insert(std::pair<INT, std::string>(3, "pvx"));
        data.insert(std::pair<INT, std::string>(4, "pvy"));
        data.insert(std::pair<INT, std::string>(5, "pvz"));
        data.insert(std::pair<INT, std::string>(6, "wma"));
        data.insert(std::pair<INT, std::string>(7, "p"));
        data.insert(std::pair<INT, std::string>(8, "q11"));
        data.insert(std::pair<INT, std::string>(9, "q16"));

        /*
        data.insert(std::pair<INT, std::string>(0, "pvx"));
        data.insert(std::pair<INT, std::string>(1, "pvy"));
        data.insert(std::pair<INT, std::string>(2, "pvz"));
        data.insert(std::pair<INT, std::string>(3, "wma"));
        data.insert(std::pair<INT, std::string>(4, "p"));
        data.insert(std::pair<INT, std::string>(5, "q11"));
        data.insert(std::pair<INT, std::string>(6, "q16"));
        */
        pTopo->Update(); 
        pTopo->Write(data);
        
       // pTopo->Print(std::cout);
        delete(pTopo);
		pTopo = NULL;
    }
protected:
	std::string fName_; 
};

void WriteModelTopo(char* root);
void WriteBlockTopo(char* root);

int main(int argc, char** argv) {
    CTestModel* test = new CTestModel(argv[1]);
    
    test->Test();

    delete(test);
    test = NULL;

//	WriteBlockTopo(argv[1]);
//    WriteModelTopo(argv[1]);

}

/*
 * XXX: 
 * if (left.size() < right.size()) return true;
 * else {
 * 	this is a wrong method for sort string; becaues if left.size() >= right.size(); a430 will < a95
 * 	return left < right;
 * }
 * */
static bool Less(const std::string& left, const std::string& right) {
	if (left.size() < right.size()) return true;
	else {
		if (left.size() > right.size()) return false;
		return left < right;
	};
}

void WriteModelTopo(char* root) {

    DIR* dir = NULL;
    struct dirent* ptr = NULL;
    dir = opendir(root);
    std::vector<std::string> vec;

    while ((ptr = readdir(dir)) != NULL) {
        if ( NULL == strstr(ptr->d_name, "block") ) continue;
        
        vec.push_back(ptr->d_name);
    }

	std::sort(vec.begin(), vec.end(), Less); 

    std::string fName = DirToFileName(root, ".topo");

    CTopology* pTopo = new CTopology(fName);
    MapIntStr data;

    for (int i = 0; i < vec.size(); i++) {
        data.insert(std::pair<INT, std::string>(i, vec[i]));
    }

    pTopo->Update(); 
    pTopo->Write(data);
        
    pTopo->Print(std::cout);
    delete(pTopo);
}


void WriteBlockTopo(char* root) {

    DIR* dir = NULL;
    struct dirent* ptr = NULL;
    dir = opendir(root);

    while ((ptr = readdir(dir)) != NULL) {
        if ( NULL == strstr(ptr->d_name, "block") ) continue;

        std::string filename = DirToFileName( DirToFileName(root, ptr->d_name), ".topo");

        CTestTopo* test = new CTestTopo(filename);
        test->Test();
        delete(test);
		test = NULL;
    
    }

}
