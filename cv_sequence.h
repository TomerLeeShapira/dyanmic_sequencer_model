#ifndef _cv_sequence_h_
#define _cv_sequence_h_


#include "sequence.h"
// #include "data_types.h"

class CV_Sequence : public Sequence
{
	public:
		CV_Sequence(uint8_t ID);

		// Adds a new Step to the sequence.
		void add_steps(uint8_t num_steps);

		void modify_step(uint8_t position);

		// Replaces step atat position.
		void replace_step(uint8_t position, std::shared_ptr<Step> new_step);
};


#endif