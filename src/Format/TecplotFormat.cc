#include "TecplotFormat.h"

CTecplot::CTeplot(const std::string& fName)  {
	
}

void CTecplot::WriteHeader() {
	
}

void CTecplot::WriteBody() {
	
}

CTecplot::~CTecplot() {
	if (NULL == pModel) {
		delete(pModel);
		pModel = NULL;
	}
}
