#include "StdAfx.h"
#include "ShadingCreator.h"

CShadingCreator::CShadingCreator()
{	
}

CShadingCreator::~CShadingCreator()
{
}

CShading* CShadingCreator::Create(BYTE type)
{
	switch (type)
	{
	case SHADING_TYPE_SAMSUNG:
		return new CShadingSamSung();
		
	case SHADING_TYPE_MTK:
		return new CShadingMTK();
		
	case SHADING_TYPE_OV:
		return new CShadingOV();
		
	case SHADING_TYPE_QUALCOMM:
		return new CShadingQualcomm();
		
	default:
		return NULL;
	}	
}