#include <iostream>
#include <vector>
#include <math.h>

void update_slices(std::vector<int> &slices, int resolution);
void print_vect(std::vector<int> v);

int main()
{

	std::vector<int> slices {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

	std::cout << "Init is ";
	print_vect(slices);

	std::cout << "Double res is ";
	update_slices(slices, 20);
	print_vect(slices);

	std::cout << "Double res is ";
	update_slices(slices, 40);
	print_vect(slices);

	std::cout << "Reduce by half is ";
	update_slices(slices, 10);
	print_vect(slices);
}

void print_vect(std::vector<int> v)
{
	for (auto i = v.begin(); i != v.end(); ++i){
    	std::cout << *i << ' ';
	}
	std::cout << std::endl;
}

void update_slices(std::vector<int> &slices, int resolution)
{
	std::vector<int> temp;
	std::vector<int>::iterator it;
	temp = slices;

	int curr_size = (int)slices.size();

	// Increase resolution. 
	if (curr_size < resolution){

		int new_val;
		// Basically, add a midpoint between each slice. Start from 1 and end at second to last.
		for (int i = 1; i < curr_size - 1; i++){

			// Grab the new iterator.
			it = slices.begin();

			// Find midpoint (average). Use temp so we can use i. 
			new_val = (temp[i] + temp[i + 1]) / 2;

			// Insert into slice vector.
			slices.insert(it + i*2, new_val);

		}

	}

	// Decrease resolution.
	else if (curr_size > resolution){

		// I forget what this algorithm is called....
		// Remove every 2nd element.
		// TODO: is this the best way?
		for (int i = curr_size - 1; i >= 2; i -= 2){
			slices.erase(slices.end() - i + 1);
		}
	}
}