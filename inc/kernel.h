#pragma once


// System Includes
#include <stdint.h>
#include <iostream>
#include <fstream>


// Project Includes
#include "util.hpp"
#include "fixedPoint.hpp"


// User defined Types
typedef uint64_t kuid;


///Kernel class representing a NxMxD structure
class kernel
{
public:
	//Constructor
	kernel(int channels, int height, int width, int fxPtLen, int numFracBits);
	
	//Deconstructor
	~kernel();

	//Return a unique ID associated with this kernel
	kuid get_uid();

	//Get the length for a given dimension of this kernel
	int get_length(int dim);

	//Set the coefficient for a given element
	void set_weight(int c, int y, int x, fixedPoint_t data);

	//Get the coefficient of a given element
	fixedPoint_t get_weight(int c, int y, int x);

	//Return a kernel object that is a subset slice of this kernel
	kernel* get_slice(int slice_start, int slice_end);

	//Diagnostic printing
	void print(std::ofstream *od);
private:
	//unique ID of this kernel
	kuid m_UID;

	//tracks the length of each dimension
	int m_channels;
	int m_height;
	int m_width;

	//pointer to coefficient data
	fixedPoint_t*** m_Data;
	
	/** total number of bits for kernel value */
	int m_fxPtLen;
	
	/** total number of fractional bits for kernel value */
	int m_numFracBits;
};