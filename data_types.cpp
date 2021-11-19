#include "data_types.h"

Step::Step(Slice_t start, Slice_t end, uint8_t resolution)
{
	_start = start;
	_end = end;

	// This is dumb.
	_resolution = 1;
	_slices.push_back(start);
	_slices.push_back(end);

	update_slices(resolution);
	_resolution = resolution;
}

// Called when resolution is changed. Either adds or removes slices from the Step
// while attempting to maintain the shape of the step. Basically bit reduction or addition.
// Resolution is a scaling factor. Resolution + 1 = Double the number of slices.
void Step::update_slices(uint8_t new_res)
{
	SliceVector_t temp;
	// std::vector<uint16_t>::iterator it;
	auto it = _slices.begin();
	temp = _slices;

	int curr_size = (int) _slices.size();
	// printf("Size is %d\n", curr_size);

	// Increase resolution. 
	if (_resolution < new_res){

		// printf("Incr res\n");
		uint8_t new_val;
		int change = new_res - _resolution;
		
		// Run through the midpoint addition as many times as needed to get to the new
		// resolution. Num slices = 2^resolution.
		for (int i = 0; i < change; i++){
			
			// Basically, add a midpoint between each slice. Start from 1 and end at second to last.
			for (int j = 0; j < curr_size - 1; j++){

				// Grab the new iterator.
				it = _slices.begin();

				// Find midpoint (average). Use temp so we can use i. 
				new_val = (temp[j] + temp[j + 1]) / 2;
				// printf("Added %d\n", new_val);

				// Insert into slice vector.
				_slices.insert(it + j*2 + 1, new_val);

			}

			// New round means new data.
			temp = _slices;
			curr_size = _slices.size();
		}

	}

	// Decrease resolution.
	else if (_resolution > new_res){

		// printf("Decr res\n");
		int change = _resolution - new_res;

		// I forget what this algorithm is called....
		// Remove every 2nd element.
		// TODO: is this the best way?
		for (int i = 0; i < change; i++){
			for (int j = curr_size - 1; j >= 2; j -= 2){
				_slices.erase(_slices.end() - j);
			}

			// New round means new data.
			curr_size = _slices.size();
		}
	}
}

void Step::set_resolution(uint8_t resolution)
{ 
	if (resolution > MAX_RES){

		update_slices(MAX_RES);
		_resolution = MAX_RES;
	}
	else {
		update_slices(resolution);
		_resolution = resolution;
	}
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
void Step::program()
{
	// Resolution means number of points on the line from start to end.
	// write_FPGA_step()
}

// Gate Stuff....
// A gate is a pulse of variable width.
void Gate::map_to_slices()
{

	// Total number of slices.
	Slice_t size = (Slice_t) pow(2, MAX_RES);

	// Width is a percent.
	Slice_t high_size = _width*size/100;
	Slice_t low_size = 100 - high_size;

	// Make sub vectors.
	SliceVector_t high_part(high_size, HIGH);
	SliceVector_t low_part(low_size, LOW);

	// Merge the sub vectors.
	_slices = merge({&high_part, &low_part});

}


// CV stuff....
void CV::map_to_slices()
{

}


// Envelope stuff
// For each type, a + d + s + r = 100%
// If that goes over, default to min (0) for remaining values.
// Priority order is a, d, s, r. This can change. 

// Attack-Decay envelope.
Envelope::Envelope(uint8_t a, uint8_t d)
{
	if (a + d <= 100){
		_first_stage = a;
		_second_stage = d;
		_third_stage = 0;
		_fourth_stage = 0;
	}
	else {
		_first_stage = a;
		_second_stage = 0;
		_third_stage = 0;
		_fourth_stage = 0;
	}
	_num_stages = 2;

	map_to_slices();
}

// Attack-Sustain-Release envelope. 
// Sustain is hard defined as HIGH.
Envelope::Envelope(uint8_t a, uint8_t s, uint8_t r)
{
	if (a + s + r <= 100){
		_first_stage = a;
		_second_stage = s;
		_third_stage = r;
		_fourth_stage = 0;
	}
	else if (a + s <= 100){
		_first_stage = a;
		_second_stage = s;
		_third_stage = 100 - a - s;
		_fourth_stage = 0;
	}
	else {
		_first_stage = a;
		_second_stage = 0;
		_third_stage = 0;
		_fourth_stage = 0;
	}
	_num_stages = 3;
	_sustain_level = HIGH;

	map_to_slices();
}

// Attack-Decay-Sustain-Release envelope. 
// Sustain here has a value from LOW to HIGH.
Envelope::Envelope(uint8_t a, uint8_t d, uint8_t s, uint8_t r, Slice_t level)
{
	if (a + d + s + r <= 100){
		_first_stage = a;
		_second_stage = d;
		_third_stage = s;
		_fourth_stage = r;
	}
	else if (a + d + s <= 100){
		_first_stage = a;
		_second_stage = d;
		_third_stage = s;
		_fourth_stage = 100 - a - d - s;
	}
	else if (a + d <= 100){
		_first_stage = a;
		_second_stage = d;
		_third_stage = 100 - a - d;
		_first_stage = 0;
	}
	else {
		_first_stage = a;
		_second_stage = 0;
		_third_stage = 0;
		_fourth_stage = 0;
	}
	_num_stages = 4;
	_sustain_level = level;

	map_to_slices();
}

void Envelope::map_to_slices()
{
	// Envelopes are fixed to resolution of MAX_RES.
	// Plot out the envelope within the vector.
	
	// Vector is 2^MAX_RES elements
	// Build the envelope out of sub vectors.
	Slice_t size = (Slice_t) pow(2, MAX_RES);
	// SliceVector_t env(size);
	Slice_t attack_size, decay_size, sustain_size, release_size = 0;

	// Attack stage is always _first_stage.
	attack_size = _first_stage*size/100;

	// If 2 or 4 stages, decay is _second_stage.
	if (_num_stages == 2 or _num_stages == 4){
		decay_size = _second_stage*size/100;
	}

	// If 3 stages, sustain is _second_stage.
	// If 3 stages, release is _third_stage.
	if (_num_stages == 3){
		sustain_size = _second_stage*size/100;
		release_size = _third_stage*size/100;
	}

	// If 4 stages, sustain is _third_stage, release is _fourth_stage.
	if (_num_stages == 4){
		sustain_size = _third_stage*size/100;
		release_size = _fourth_stage*size/100;
	}

	// Define the sub vectors....
	// Attack is a rising ramp from LOW to HIGH.
	SliceVector_t attack = bound_arange<Slice_t>(LOW, HIGH, attack_size);

	// Decay is a falling ramp from HIGH to sustain level.
	SliceVector_t decay = bound_arange<Slice_t>(HIGH, _sustain_level, decay_size);

	// Sustain is a constant value of sustain level.
	SliceVector_t sustain(sustain_size, _sustain_level);

	// Release is a falling ramp from sustain level to LOW.
	SliceVector_t release = bound_arange<Slice_t>(_sustain_level, LOW, release_size);

	// Combine them into an envelope step.
	_slices = merge({&attack, &decay, &sustain, &release});
}