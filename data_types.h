

#define HIGH 255
#define LOW 0
#define MAX_RES 10


// A step in the sequence. Represents a function that determines the voltage output.
// f(x) is defined by start value, end value, and the resolution. A start of LOW to 
// an end of HIGH with the max resolution would be a ramp. 
// Note that the point of this is to define the parameters for the FPGA state machine, 
// which actually outputs f(x). 
class Step
{
	
	private:
		uint8_t _start;
		uint8_t _end;

		// Resolution is the scaling factor for the slices in the step. Number of 
		// slices is 2^resolution. 
		uint8_t _resolution;
		std::vector<uint8_t> _slices;

	public:

		Step(){
			set_start(0);
			set_end(0);
			set_resolution(1);
			update_slices();
		}
		Step(uint8_t start, uint8_t end, uint8_t resolution);
		void update_slices();
		// void add_slices();
		// void remove_slices();
		virtual void set_resolution(uint8_t resolution);
		virtual uint8_t get_resolution() { return _resolution;}
		virtual void set_start(uint8_t start) {	_start = start;}
		uint8_t get_start() { return _start;}
		virtual void set_end(uint8_t end) {	_end = end;}
		uint8_t get_end() { return _end;}

		// Writes the params to the FPGA state machine. 
		virtual void program();

}


// A gate is a step that is high for a user-defined pulse width.
// Resolution is baked-in.
class Gate : public Step
{
	private:
		uint8_t _width;

	public:
		Gate(uint8_t width) : Step(HIGH, LOW, MAX_RES){ _width = width;}

}


// A trigger is a step that is high for the absolute system minimum pulse width. 
// Resolution is baked-in.
class Trigger : public Step
{
	public:
		Trigger() : Step(HIGH, LOW, MAX_RES);
}

// Techically just the normal Step class. 
class CV : public Step
{
	public:
		CV() : Step(HIGH, LOW, MAX_RES){}

		CV(uint8_t start, uint8_t end, uint8_t resolution) : Step(start, end, resolution){}

		void set_start(uint8_t start){ _start = start;}
		void set_end(uint8_t end){ _end = end;}
		void set_resolution(uint8_t resolution){ _resolution = resolution;}
		uint8_t get_resolution(){ return _resolution;}
}



// class LFO
// {
// 	private:
// 		uint8_t _rate;
// 	public:

// }

// class Envelope
// {
// 	private:
// 		uint8_t _attack_time;
// }

