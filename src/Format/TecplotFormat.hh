#ifndef TECPLOT_FORMAT_H
#define TECPLOT_FORMAT_H

#include "Model.h"

class CTecplot {
public:
	explicit CTecplot(const std::string& fName);
	~CTecplot();

	void WriteHeader();
private:
	CTecplot(const CTecplot& lref) {}
	void operator=(const CTecplot& lref) {}
private:
	CModel* pModel;
};

#endif
