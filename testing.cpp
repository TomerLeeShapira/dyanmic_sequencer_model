#include <iostream>
#include <vector>
#include <math.h>
#include "data_types.h"
#include "cv_sequence.h"

void update_slices(std::vector<uint16_t> &slices, int resolution);
void print_vect(std::vector<uint16_t> v);
int curr_res;

int main()
{

	// std::vector<uint16_t> slices {0, 100};
	// curr_res = 1;
	// Step* step = new Step(0, 100, 2);

	// update_slices(slices, 3);

	// std::cout << "Init is ";
	// print_vect(step->get_slices());

	// std::cout << "Testfnc says ";
	// print_vect(slices);

	// std::cout << "Curr res is ";
	// std::cout << (int)step->get_resolution() << std::endl;
	
	// step->set_resolution(4);
	// std::cout << "New res is ";
	// std::cout << (int)step->get_resolution() << std::endl;

	// std::cout << "Inc to 4 res is ";
	// // update_slices(slices, 20);
	// print_vect(step->get_slices());

	// std::cout << "Double res is ";
	// // update_slices(slices, 40);
	// step->set_resolution(8);
	// print_vect(step->get_slices());
	
	// std::cout << "Reduce by half is ";
	// // update_slices(slices, 10);
	// step->set_resolution(4);
	// print_vect(step->get_slices());

	// std::cout << "Testfnc says ";
	// update_slices(slices, 2);
	// print_vect(slices);

	CV_Sequence track_1(1);
	int ID = track_1.track_ID;
	std::cout << "ID is " << ID << std::endl;

	track_1.add_steps(1);

	auto env = std::make_shared<Envelope>(25, 25, 25, 25, 65535/2);
	print_vect(env->get_slices());
	
	track_1.replace_step(1, env);
	std::cout << "Replaced with env" << std::endl;

	auto first_step = track_1.get_step(0);
	auto second_step = track_1.get_step(1);

	print_vect(first_step->get_slices());
	print_vect(second_step->get_slices());


	// delete step;
	// delete env;
}

void print_vect(std::vector<uint16_t> v)
{
	for (auto i = v.begin(); i != v.end(); ++i){
    	std::cout << *i << ' ';
	}
	std::cout << std::endl;
}

void update_slices(std::vector<uint16_t> &slices, int resolution)
{
	std::vector<uint16_t> temp;
	std::vector<uint16_t>::iterator it;
	temp = slices;

	int curr_size = (int)slices.size();

	// Increase resolution. 
	if (curr_res < resolution){

		uint16_t new_val;
		int change = resolution - curr_res;
		
		for (int i = 0; i < change; i++){
			
			// Basically, add a midpoint between each slice. Start from 1 and end at second to last.
			for (int j = 0; j < curr_size - 1; j++){

				// Grab the new iterator.
				it = slices.begin();

				// Find midpoint (average). Use temp so we can use i. 
				new_val = (temp[j] + temp[j + 1]) / 2;
				// std::cout << "Added " << new_val << std::endl;

				// Insert into slice vector.
				slices.insert(it + j*2 + 1, new_val);

			}
			temp = slices;
			curr_size = slices.size();
		}

		curr_res = resolution;

	}

	// Decrease resolution.
	else if (curr_res > resolution){

		// I forget what this algorithm is called....
		// Remove every 2nd element.
		// TODO: is this the best way?
		int change = curr_res - resolution;

		for (int i = 0; i < change; i++){
			for (int j = curr_size - 1; j >= 2; j -= 2){
				// std::cout << "Removing " << slices[slices.end() - j + 1] << std::endl;
				// std::cout << "Removing" << std::endl;
				slices.erase(slices.end() - j);
			}

			// New round means new data.
			curr_size = slices.size();
		}
		curr_res = resolution;
	}
}