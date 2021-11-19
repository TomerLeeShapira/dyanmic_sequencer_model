#include <iostream>
#include <vector>

#include "sequence.h"

void Sequence::change_num_steps(uint8_t new_steps, bool preserve_pattern)
{
	uint8_t num_steps = get_num_steps();

	// Don't care about the pattern integrity. 
	if (!preserve_pattern){
		// Remove steps if new_steps < num_steps.
		if (new_steps < num_steps){

			uint8_t change = num_steps - new_steps;
			for (int i = 0; i < change; i++){
				steps.pop_back();

			}
		}
		// Add steps if new_steps > num_steps.
		if (new_steps > num_steps){

			uint8_t change = new_steps - num_steps;
			for (int i = 0; i < change; i++){
				add_steps(1);
			}
		}
	}

	// Do our best to preserve the pattern when changing the steps. Number of steps
	// determines the resolution in which this is possible.
	else {
		// TODO: this is gonna suck to do. 
		// Same kind of change as done within a given step when improving its resolution, 
		// but now done across the whole sequence. 
	}


}

// Return step at the given index. Null if out of bounds.
std::shared_ptr<Step> Sequence::get_step(uint8_t position)
{
	// Can't index where nothing exists. 	
	if (position >= get_num_steps()){
		return nullptr;
	}

	// Return the step. 
	else {
		auto it = steps.begin();
		return *(it + position);
	}
}


// Position is the index into the array. 
void Sequence::remove_step(uint8_t position)
{
	uint8_t num_steps = get_num_steps();
	
	// Just in case position is invalid.
	if (position > num_steps){

		//TODO: some error raising system would be nice....

		return;
	}

	// Might be a better way to do this, per the vector docs mentioning this is inefficient?
	steps.erase(steps.begin() + position);
}

// Overloaded for multiple steps.
void Sequence::remove_step(uint8_t start_position, uint8_t end_position)
{
	uint8_t num_steps = get_num_steps();
	
	// Just in case position is invalid.
	if (start_position > num_steps or end_position > num_steps){

		//TODO: some error raising system would be nice....

		return;
	}

	// Start vs end dont matter. We care about a range. 
	if (start_position > end_position){
		uint8_t temp;
		temp = end_position;
		end_position = start_position;
		start_position = temp;
	}

	// Might be a better way to do this, per the vector docs mentioning this is inefficient?
	steps.erase(steps.begin() + start_position, steps.begin() + end_position);
}