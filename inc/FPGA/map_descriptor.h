#pragma once

#include <stdint.h>
#include <vector>

typedef uint64_t muid;

///map descriptor class representing a NxMxD structure 
class map_descriptor
{
public : 
	//Constructor
	map_descriptor(int h, int w, int d);
	

	//Deconstructor
	~map_descriptor();

	//Return a unique ID associated with this map
	muid get_uid();

	//Get the length for a given dimension of this map
	int get_length(int dim);

	//Get the length of the entire map
	int get_length();

	//Get the number of bytes per pixel
	int get_bytes_per_pixel();

	//Get the offset wrt parent for this map
	int get_offset(int dim);

	//Return a map_descriptor object that is a sub volume of this map
	map_descriptor* get_volume(int x0, int x1, int y0, int y1, int z0, int z1);
	
	//Get the value of the map at a specific x,y,z coord
	float get_value(int x, int y, int z);

	//Set the value of the map at a specific x,y,z coord
	float set_value(int x, int y, int z, float value);

	//Diagnostic printing
	void print();

	uint16_t* get_data();


private:
	//Constructor for creating volumes
	map_descriptor(map_descriptor* parent, int x0, int x1, int y0, int y1, int z0, int z1);

	//Pointer to parent map_descriptor if exists, nullptr otherwise
	map_descriptor * m_Parent;

	//Keep dimensions of map
	int m_Dims[3];

	//Offsets of this map with respect to parent if it exists, zeros otherwise
	int m_Offsets[3];

	uint16_t* m_Data;

};