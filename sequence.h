#ifndef _sequence_h_
#define _sequence_h_

#include "data_types.h"

class Sequence 
{

	public:
		// Track number in system. Defined externally on creation of a sequence. 
		uint8_t track_number;

		// The actual sequence. 
		std::vector<std::shared_ptr<Step>> steps;

		uint8_t get_num_steps(){ return (uint8_t)(steps.size)();}

		void change_num_steps(uint8_t new_steps, bool preserve_pattern);
		void set_division(uint8_t division);
		void set_clock(uint8_t clock);

		void remove_step(uint8_t position);
		void remove_step(uint8_t start_position, uint8_t end_position);
		
		
		// Abstract stuff....
		
		// Constructor args for adding a new Step object depends on the type of sequence.
		virtual void add_step() = 0;
		virtual void modify_step(uint8_t position) = 0;

}

#endif