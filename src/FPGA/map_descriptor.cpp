#include "map_descriptor.h"

map_descriptor::map_descriptor(int h, int w, int d)
{
	m_Dims[0] = w;
	m_Dims[1] = h;
	m_Dims[2] = d;

	m_Offsets[0] = 0;
	m_Offsets[1] = 0;
	m_Offsets[2] = 0;
}


map_descriptor::map_descriptor(map_descriptor* parent, int x0, int x1, int y0, int y1, int z0, int z1)
{
	m_Parent = parent;

	m_Dims[0] = (x1 - x0) + 1;
	m_Dims[1] = (y1 - y0) + 1;
	m_Dims[2] = (z1 - z0) + 1;

	m_Offsets[0] = x0;
	m_Offsets[1] = y0;
	m_Offsets[2] = z0;

	for (int x = 0; x < m_Dims[0]; x++) {
		for (int y = 0; y < m_Dims[1]; y++) {
			for (int z = 0; z < m_Dims[2]; z++) {
				set_value(x, y, z, m_Parent->get_value(x + m_Offsets[0], y + m_Offsets[1], z + m_Offsets[2]));
			}
		}
	}
}

map_descriptor::~map_descriptor()
{
}

muid map_descriptor::get_uid()
{
	return muid();
}

int map_descriptor::get_length(int dim)
{

	return (dim>=3) ? 0 : m_Dims[dim];
}

int map_descriptor::get_length()
{
	return get_length(0) * get_length(1) * get_length(2);
}

int map_descriptor::get_bytes_per_pixel()
{
	//Hard coded 2 bytes per pixel for now
	return 2;
}

int map_descriptor::get_offset(int dim)
{
	return (dim >= 3) ? 0 : m_Offsets[dim];
}

map_descriptor * map_descriptor::get_volume(int x0, int x1, int y0, int y1, int z0, int z1)
{
	if (x0 < 0 || x0 >= m_Dims[0] || x1 < x0 || x1 >= m_Dims[0])
		return nullptr;

	if (y0 < 0 || y0 >= m_Dims[1] || y1 < y0 || y1 >= m_Dims[1])
		return nullptr;

	if (z0 < 0 || z0 >= m_Dims[2] || z1 < z0 || z1 >= m_Dims[2])
		return nullptr;

	return new map_descriptor(this, x0, x1, y0, y1, z0, z1);
}

float map_descriptor::get_value(int x, int y, int z)
{
	return 0.0f;
}

float map_descriptor::set_value(int x, int y, int z, float value)
{
	return 0.0f;
}

void map_descriptor::print()
{
}

uint16_t* map_descriptor::get_data()
{
	return m_Data;
}

