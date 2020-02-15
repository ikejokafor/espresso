#include "pixel_seq_config.hpp"


pixel_seq_config::pixel_seq_config(int stride)
{
	m_stride = stride;
}


uint8_t* pixel_seq_config::get_bytes(int& length)
{
	if (m_stride == 1)
	{
		return stride1_config(length);
	}
	else
	{
		return stride2_config(length);
	}
}


uint8_t* pixel_seq_config::stride1_config(int& length)
{
	length = MAX_NUM_INPUT_COLS * NUM_CE_PER_QUAD * PIX_SEQ_CONFIG_SIZE;
	uint16_t *bytes = new uint16_t[length];
	bytes[0] = uint16_t(3072);
	bytes[1] = uint16_t(2);
	bytes[2] = uint16_t(8704);
	bytes[3] = uint16_t(513);
	bytes[4] = uint16_t(4610);
	uint16_t pix_seq0 = 0;
	uint16_t pix_seq1 = 2;
	uint16_t pix_seq2 = 512;
	uint16_t pix_seq3 = 513;
	uint16_t pix_seq4 = 514;
	uint16_t pix_seq_bits_0_0 = uint16_t(2048);
	uint16_t pix_seq_bits_0_1 = uint16_t(3072);
	uint16_t pix_seq_bits_1_0 = uint16_t(1024);
	uint16_t pix_seq_bits_1_1 = uint16_t(0);
	uint16_t pix_seq_bits_2 = uint16_t(8192);
	uint16_t pix_seq_bits_3 = uint16_t(0);
	uint16_t pix_seq_bits_4 = uint16_t(4096);
	int j = 0;
	int i;
	for (i = WINDOW_3x3_NUM_CYCLES; i < (MAX_NUM_INPUT_COLS * WINDOW_3x3_NUM_CYCLES); i = i + WINDOW_3x3_NUM_CYCLES)
	{
	    if((j % 2) == 0)
		{
			bytes[i] = pix_seq_bits_0_0 | pix_seq0;
			bytes[i + 1] = pix_seq_bits_1_0 | pix_seq1;
			pix_seq0++;
		}
		else
		{
			bytes[i] = pix_seq_bits_0_1 | pix_seq0;
			bytes[i + 1] = pix_seq_bits_1_1 | pix_seq1;
			pix_seq0++;
			pix_seq1 += 2;
		}
		bytes[i + 2] = pix_seq_bits_2 |  pix_seq2;
		bytes[i + 3] = pix_seq_bits_3 |  pix_seq3;
		bytes[i + 4] = pix_seq_bits_4 |  pix_seq4;
		pix_seq2++;
		pix_seq3++;
		pix_seq4++;
		j = (j + 1) % 2;
	}
	while(i < (MAX_NUM_INPUT_COLS * NUM_CE_PER_QUAD))
	{
		bytes[i] = 0;
		i = i + 1;
	}
	return (uint8_t*)bytes;
}


uint8_t* pixel_seq_config::stride2_config(int& length)
{
	length = MAX_NUM_INPUT_COLS * NUM_CE_PER_QUAD * PIX_SEQ_CONFIG_SIZE;
	uint16_t *bytes = new uint16_t[length];
	bytes[0] = uint16_t(3072);
	bytes[1] = uint16_t(2);
	bytes[2] = uint16_t(8704);
	bytes[3] = uint16_t(12801);
	bytes[4] = uint16_t(4610);
	uint16_t pix_seq0 = 0;
	uint16_t pix_seq1 = 2;
	uint16_t pix_seq2 = 512;
	uint16_t pix_seq3 = 513;
	uint16_t pix_seq4 = 514;
	uint16_t pix_seq_bits_0 = uint16_t(3072);
	uint16_t pix_seq_bits_1 = uint16_t(0);
	uint16_t pix_seq_bits_2 = uint16_t(8192);
	uint16_t pix_seq_bits_3 = uint16_t(12288);
	uint16_t pix_seq_bits_4 = uint16_t(4096);
    int j = 0;
	int i;
    for(i = WINDOW_3x3_NUM_CYCLES; i < ((MAX_NUM_INPUT_COLS / 2) * WINDOW_3x3_NUM_CYCLES); i = i + WINDOW_3x3_NUM_CYCLES)
    {
	    bytes[i] = pix_seq_bits_0 | pix_seq0;
	    bytes[i + 1] = pix_seq_bits_1 | pix_seq1;
	    bytes[i + 2] = pix_seq_bits_2 | pix_seq2;
	    bytes[i + 3] = pix_seq_bits_3 | pix_seq3;
	    bytes[i + 4] = pix_seq_bits_4 | pix_seq4;
	    pix_seq0 += 2;
	    pix_seq1 += 2;
	    pix_seq2 += 2;
	    pix_seq3 += 2;
	    pix_seq4 += 2;
        j = (j + 1) % 2;
	}
	while(i < (MAX_NUM_INPUT_COLS * NUM_CE_PER_QUAD))
	{
		bytes[i] = 0;
		i = i + 1;
	}
	return (uint8_t*)bytes;
}