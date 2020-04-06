#include "PixelSeqCfg.hpp"


PixelSeqCfg::PixelSeqCfg(int stride)
{
	m_stride = stride;
	m_address = allocate(MAX_NUM_INPUT_COLS * NUM_CE_PER_QUAD * PIX_SEQ_CONFIG_SIZE);
	if(stride == 1)
	{
		stride1_config();
	}
	else
	{
		stride2_config();
	}
}


PixelSeqCfg::~PixelSeqCfg()
{
	deallocate();
}


uint64_t PixelSeqCfg::allocate(int size)
{
	m_size = size;
#ifdef SYSTEMC
	m_address = (uint64_t)malloc(size);
#else

#endif
	return m_address;
}


void PixelSeqCfg::deallocate()
{
#ifdef SYSTEMC
	free((uint64_t*)m_address);
#else

#endif
}


void PixelSeqCfg::serialize()
{

}


void PixelSeqCfg::deserialize()
{

}


void PixelSeqCfg::stride1_config()
{
	uint16_t* bytes = (uint16_t*)m_address;
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
}


void PixelSeqCfg::stride2_config()
{
	uint16_t* bytes = (uint16_t*)m_address;
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
}