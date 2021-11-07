#include "cv_sequence.h"



void CV_Sequence::add_step()
	{
		steps.push_back(new CV_Step);
	}