#include "QIMDTable.h"

QIMDTable::QIMDTable(std::string datapayload)
{
	// We only need to retrieve the first item which is the name
	Name = Utils::SplitStringToVector(datapayload)[0];
}