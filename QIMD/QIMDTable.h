#pragma once
#include <string>
#include <vector>
#include "Utils.h"
#include "QIMDColumn.h"

class QIMDTable {
public:
	QIMDTable(std::string datapayload);
	std::string Name;
private:
	QIMDColumn columns;
};