#pragma once
#include "GWSystem1.0.h"
#include "Scanning_Control.h"


class AllParaSet
{
public:
	AllParaSet();
	~AllParaSet();


	// Ӳ����������
	bool HardwareParaSet(Scanning_Control * ScanningParaSet, ALL_PAREMETER *allpara);
};

