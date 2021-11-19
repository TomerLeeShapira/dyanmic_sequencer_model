#include "cv_sequence.h"
#include <iostream>

void print_vect_intr(SliceVector_t v);

CV_Sequence::CV_Sequence(uint8_t ID)
{
	// No init length means start with 1 step.
	add_steps(1);
	track_ID = ID;
}

// Add any number of CV steps to the sequence.
void CV_Sequence::add_steps(uint8_t num_steps)
	{
		// Create a temp vector of new CV steps.
		std::vector<std::shared_ptr<Step>> temp;
		for (int i = 0; i < (int)num_steps; i++){
			auto new_step = std::make_shared<CV>();
			temp.push_back(new_step);
		}

		// Append steps to the end of the sequence.
		steps.insert(steps.end(), temp.begin(), temp.end());
	}

// Modify the step at the given index with new parameters.
void CV_Sequence::modify_step(uint8_t position)
{
	return;
}

// Replace step at position with new step.
void CV_Sequence::replace_step(uint8_t position, std::shared_ptr<Step> new_step)
{
	steps[position] = new_step;
}

void print_vect_intr(SliceVector_t v)
{
	for (auto i = v.begin(); i != v.end(); ++i){
    	std::cout << *i << ' ';
	}
	std::cout << std::endl;
}