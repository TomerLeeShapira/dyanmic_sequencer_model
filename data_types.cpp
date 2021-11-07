#include "data_types.h"
#include <math.h>

Step::Step(uint8_t start, uint8_t end, uint8_t resolution)
{
	_start = start;
	_end = end;
	_resolution = resolution;
	update_slices();
}

// Called when resolution is changed. Either adds or removes slices from the Step
// while attempting to maintain the shape of the step. Basically bit reduction or addition.
// Resolution is a scaling factor. Resolution + 1 = Double the number of slices.
void Step::update_slices(uint8_t new_res)
{
	std::vector<uint8_t> temp;
	std::vector<uint8_t>::iterator it;
	temp = _slices;

	int curr_size = (int)_slices.size();

	// Increase resolution. 
	if (_resolution < new_res){

		uint8_t new_val;
		
		// Run through the midpoint addition as many times as needed to get to the new
		// resolution. Num slices = 2^resolution.
		for (int i = 0; i < new_res; i++){
			
			// Basically, add a midpoint between each slice. Start from 1 and end at second to last.
			for (int j = 1; j < curr_size - 1; j++){

				// Grab the new iterator.
				it = _slices.begin();

				// Find midpoint (average). Use temp so we can use i. 
				new_val = (temp[j] + temp[j + 1]) / 2;

				// Insert into slice vector.
				_slices.insert(it + j*2, new_val);

			}
		}

	}

	// Decrease resolution.
	else if (_resolution > new_res){

		// I forget what this algorithm is called....
		// Remove every 2nd element.
		// TODO: is this the best way?
		for (int i = 0; i < new_res; i++){
			for (int j = curr_size - 1; j >= 2; j -= 2){
				_slices.erase(_slices.end() - j + 1);
			}
		}
	}
}

virtual void Step::set_resolution(uint8_t resolution)
{ 
	if (resolution > MAX_RES){

		update_slices(MAX_RES);
		_resolution = MAX_RES;
	}
	else {
		update_slices(resolution);
		_resolution = resolution;
	}

	// Always update the slices when resolution is changed.
	update_slices();
}

// // Helper to add a slice.
// void Step::add_slices(uint8_t number)
// {
// 	for (uint8_t i = 0; i < number; i++){
// 		_slices.push_back(0);
// 	}
// }

// // Helper to remove a slice.
// void Step::remove_slices()
// {

// }

// Generic write params to FPGA. 
virtual void Step::program()
{
	// Resolution means number of points on the line from start to end.
	write_FPGA_step()
}