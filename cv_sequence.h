#ifndef _cv_sequence_h_
#define _cv_sequence_h_

#include "sequence.h"

class CV_Sequence : public Sequence
{
	CV_Sequence(uint8_t track_number);

	// Adds a new Step to the sequence.
	void add_step();
}


#endif