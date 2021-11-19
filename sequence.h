#ifndef _sequence_h_
#define _sequence_h_

#include <memory>
#include "data_types.h"


// Abstract class for a sequence. 
// Made up of steps.
class Sequence 
{

	public:
		// Track ID in system. Defined externally on creation of a sequence. 
		uint8_t track_ID;

		// The actual sequence. 
		std::vector<std::shared_ptr<Step>> steps;

		Sequence(){}

		uint8_t get_num_steps(){ return (uint8_t)(steps.size)();}

		void change_num_steps(uint8_t new_steps, bool preserve_pattern);
		void set_division(uint8_t division);
		void set_clock(uint8_t clock);

		void remove_step(uint8_t position);
		void remove_step(uint8_t start_position, uint8_t end_position);

		std::shared_ptr<Step> get_step(uint8_t position);
		
		
		// Abstract stuff....
		
		// Constructor args for adding a new Step object depends on the type of sequence.
		virtual void add_steps(uint8_t num_steps) = 0;
		virtual void modify_step(uint8_t position) = 0;
		virtual void replace_step(uint8_t position, std::shared_ptr<Step> new_step) = 0;

};

#endif