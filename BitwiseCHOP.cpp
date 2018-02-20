/* Shared Use License: This file is owned by Derivative Inc. (Derivative) and
 * can only be used, and/or modified for use, in conjunction with 
 * Derivative's TouchDesigner software, and only if you are a licensee who has
 * accepted Derivative's TouchDesigner license or assignment agreement (which
 * also govern the use of this file).  You may share a modified version of this
 * file with another authorized licensee of Derivative's TouchDesigner software.
 * Otherwise, no redistribution or sharing of this file, with or without
 * modification, is permitted.
 */

#include "BitwiseCHOP.h"

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <assert.h>

extern "C"
{
	DLLEXPORT
	int32_t GetCHOPAPIVersion(void)
	{
		return CHOP_CPLUSPLUS_API_VERSION;
	}

	DLLEXPORT
	CHOP_CPlusPlusBase* CreateCHOPInstance(const OP_NodeInfo* info)
	{
		return new BitwiseCHOP(info);
	}

	DLLEXPORT
	void DestroyCHOPInstance(CHOP_CPlusPlusBase* instance)
	{
		delete (BitwiseCHOP*)instance;
	}
};

BitwiseCHOP::BitwiseCHOP(const OP_NodeInfo* info) 
{
	executeCount = 0;
}

BitwiseCHOP::~BitwiseCHOP()
{
}

void BitwiseCHOP::getGeneralInfo(CHOP_GeneralInfo* ginfo)
{
	ginfo->cookEveryFrameIfAsked = true;
	ginfo->timeslice = false;
}

bool BitwiseCHOP::getOutputInfo(CHOP_OutputInfo* info)
{
	if (info->opInputs->getNumInputs() > 0)
	{
		return false;
	} 
	else
	{
		info->numChannels = 3;
		info->numSamples = 100;
		info->startIndex = 0;
		return true;
	}

}

const char* BitwiseCHOP::getChannelName(int32_t index, void* reserved)
{
	return "chan1";
}

void BitwiseCHOP::execute(const CHOP_Output* output, OP_Inputs* inputs, void* reserved)
{
	executeCount++;

	if (inputs->getNumInputs() > 0)
	{
		// Grab the parameter `Value`, which is the number that we 
		// will perform a bitwise AND with
		int value = inputs->getParInt("Value");

		int index = 0;
		for (int i = 0 ; i < output->numChannels; i++)
		{
			for (int j = 0; j < output->numSamples; j++)
			{
				const OP_CHOPInput *cinput = inputs->getInputCHOP(0);

				// Perform bitwise op
				output->channels[i][j] = static_cast<int>(cinput->getChannelData(i)[index]) & value;
				index++;

				// Make sure we don't read past the end of the CHOP input
				index = index % cinput->numSamples;
			}
		}
	}
	else
	{
		// There is no input CHOP, so do something else...
	}
}

int32_t BitwiseCHOP::getNumInfoCHOPChans()
{
	return 0;
}

void BitwiseCHOP::getInfoCHOPChan(int32_t index, OP_InfoCHOPChan* chan)
{
}

bool BitwiseCHOP::getInfoDATSize(OP_InfoDATSize* infoSize)
{
	infoSize->rows = 0;
	infoSize->cols = 0;
	return true;
}

void BitwiseCHOP::getInfoDATEntries(int32_t index, int32_t nEntries, OP_InfoDATEntries* entries)
{
}

void BitwiseCHOP::setupParameters(OP_ParameterManager* manager)
{
	{
		OP_NumericParameter	np;

		np.name = "Value";
		np.label = "Value";
		np.defaultValues[0] = 1;

		OP_ParAppendResult res = manager->appendInt(np);
		assert(res == OP_ParAppendResult::Success);
	}
}

void BitwiseCHOP::pulsePressed(const char* name)
{
}

