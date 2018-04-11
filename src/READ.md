# DataMerge

## File.cc
```
//Package nceessary file operation; 
class CFileOperation;

//Base class for CFileSingleField and CTopo;
//Function Update() is used to open file or some basic operation about file, for example set file size;
//Function Print() is used to print self infomation; 
class CFileBase;

//Concrete field file.
//To improve IO, using function mmap() to create a new mapping in the virtual address of th calling process. 
//Function Write() is used to persist data;
//As subclass of CFileBase, need override the virtual function PrintSelf() and Update(); 
class CFileSingleField;

//Record topology information;
class CTopo;
```

## Block.cc
```
//Pacgage necessary dir operation;
class CDirOperation;

//Function Update() is used to get all matched files in dir and get their counts;
//As similiar as CFileBase, Function Print() is used to print dir information;
class CDirBase;

//Use standard container "map" to record CFileSingleField, means numbers CFileSingleField object contains one block;
//Use function Write() to merge CFileSingleFields, eg: the sequence of CFileSingleField;
class CBlock;
```

## Model.cc
```
//Original desigin: firstly, merge CFileSingleFields between each block, secondly, merge each field data; it is the fatal desigin;
//Current desigin: firstly, merge all CFileSingleFields in each block, secondly, merge ecah block data; i think this method can help me to solve problem;
//Function Write() is used to merge the file;
class CModel;
```

## XML

