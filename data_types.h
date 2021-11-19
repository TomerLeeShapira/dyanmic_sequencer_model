#ifndef _data_types_h_
#define _data_types_h_

#include <cstdint>
#include <vector>
#include <math.h>
#include <stdio.h>

#define HIGH 65535
#define LOW 0
#define MAX_RES 10


// Change these to change the size of a slice.
typedef uint16_t Slice_t;
typedef std::vector<Slice_t> SliceVector_t;

// A step in the sequence. Represents a function that determines the voltage output.
// f(x) is defined by start value, end value, and the resolution. A start of LOW to 
// an end of HIGH with the max resolution would be a ramp. 
// Note that the point of this is to define the parameters for the FPGA state machine, 
// which actually outputs f(x). 
class Step
{
	
	protected:
		Slice_t _start;
		Slice_t _end;

		// Resolution is the scaling factor for the slices in the step. Number of 
		// slices is 2^resolution. 
		uint8_t _resolution;

		// 16 bit DACs expected. Calibration and scaling handled by FPGA. 
		// suggested: DAC8574-series - Quad 16-Bit I2C DACs running off a 5v Reference
		SliceVector_t _slices;

		// Called on updating resolution. Modifies _slices.
		void update_slices(uint8_t new_res);

	public:

		Step(){
			_start = 0;
			_end = 0;

			// This is dumb.
			// _resolution = 0;
			// update_slices(1);
			_slices.push_back(_start);
			_slices.push_back(_end);
			_resolution = 1;
		}
		Step(Slice_t start, Slice_t end, uint8_t resolution);

		~Step()
		{

		}
		
		// void add_slices();
		// void remove_slices();
		virtual void set_resolution(uint8_t resolution);
		virtual uint8_t get_resolution() { return _resolution;}
		virtual void set_start(Slice_t start) {	_start = start;}
		uint8_t get_start() { return _start;}
		virtual void set_end(Slice_t end) {	_end = end;}
		uint8_t get_end() { return _end;}

		virtual void map_to_slices() = 0;

		// Writes the params to the FPGA state machine. 
		virtual void program();

		// Utility
		SliceVector_t get_slices() { return _slices;}

};


// A gate is a step that is high for a user-defined pulse width.
// Resolution is baked-in.
// Width is a percent.
class Gate : public Step
{
	private:
		uint8_t _width;

	public:
		Gate(uint8_t width) : Step(LOW, LOW, MAX_RES)
		{ 
			if (width >= 100){
				_width = 100;
			}
			else {
				_width = width;
			}
			map_to_slices();
		}
		void map_to_slices();

};


// A trigger is a step that is high for the absolute system minimum pulse width. 
// Resolution is baked-in.
// It is a gate of width 1 percent. 
// TOOD: this is wrong. It needs to be a fixed pulse width. In the current implementation, 
// it varies with sequence speed. Change this to some FPGA reg parameters.
class Trigger : public Gate
{
	public:
		Trigger() : Gate(1){}
};

// Techically just the normal Step class. 
// Adds in general function mapping to slices. 
class CV : public Step
{
	public:
		CV() : Step(LOW, LOW, MAX_RES){}

		CV(Slice_t start, Slice_t end, uint8_t resolution) : Step(start, end, resolution){}

		void set_start(Slice_t start){ _start = start;}
		void set_end(Slice_t end){ _end = end;}
		void set_resolution(uint8_t resolution){ _resolution = resolution;}
		uint8_t get_resolution(){ return _resolution;}

		// TODO: takes in a function pointer and builds a slicevector from it. 
		void map_to_slices();
};



// class LFO
// {
// 	private:
// 		uint8_t _rate;
// 	public:

// };

// Specialized CV type. ADSR envelope. Initialized as AD, ASR, or ADSR. 
// Units for the stages are "percent of step". Min is 0, max is whatever fits. 
class Envelope : public CV
{
	private:
		uint8_t _first_stage;
		uint8_t _second_stage;
		uint8_t _third_stage;
		uint8_t _fourth_stage;
		Slice_t _sustain_level;
		uint8_t _num_stages;

	public:

		// Base class values not used here....Envelope writes directly to the slices.
		Envelope(uint8_t a, uint8_t d);
		Envelope(uint8_t a, uint8_t s, uint8_t r);
		Envelope(uint8_t a, uint8_t d, uint8_t s, uint8_t r, Slice_t level);

		void map_to_slices();
};


// Helpers....

// Numpy-like arange(). Two versions made, one where output size does not matter
// so incrememnt step is set, and one where output size is fixed, so increment step is 
// based on that. 
template<typename T>
std::vector<T> unbound_arange(T start, T stop, T step) {
    
    std::vector<T> values;
    
    for (T value = start; value < stop; value += step){
        values.push_back(value);
    }
    return values;
}
template<typename T>
std::vector<T> bound_arange(T start, T stop, T size) {

	std::vector<T> values;
	if (start > stop){
		T step = (start - stop) / size + 1;
	    
	    T value = start;
		for (T i = 0; i < size; i++, value -= step){
	        values.push_back(value);
	    }
	}
	if (start < stop){
		T step = (stop - start) / size + 1;
		// printf("step is %d\n", step);

		T value = start;
		for (T i = 0; i < size; i++, value += step){
			// printf("value is %d\n", value);
			values.push_back(value);
		}
	}
    return values;
}

// Merge vectors
template<typename T>
std::vector<T> merge(std::initializer_list<std::vector<T>*> vecs)
{
    size_t size = 0;
    for(auto v : vecs) { size += v->size(); }
    std::vector<T> ret;
    ret.reserve(size);
    for(auto v : vecs) { ret.insert(ret.end(), v->begin(), v->end()); }
    return ret;
}


#endif