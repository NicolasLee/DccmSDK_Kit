#ifndef SHADING_CREATOR_H
#define SHADING_CREATOR_H

#pragma once

#include "Shading.h"
#include "ShadingSamSung.h"
#include "ShadingMTK.h"
#include "ShadingOV.h"
#include "ShadingQualcomm.h"


class CShadingCreator
{
public:	
	CShadingCreator();
	virtual ~CShadingCreator();

	CShading* Create(BYTE type);
};

#endif  /* SHADING_CREATOR_H */
