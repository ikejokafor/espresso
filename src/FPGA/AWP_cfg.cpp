#include "AWP_cfg.hpp"


AWP_cfg::AWP_cfg() :
   m_imAddrArr(MAX_QUAD_PER_AWP),
	m_krnl3x3AddrArr(MAX_QUAD_PER_AWP),
	m_krnl3x3BiasAddrArr(MAX_QUAD_PER_AWP) { }


AWP_cfg::AWP_cfg(int FAS_id, int AWP_id) :
	m_imAddrArr(MAX_QUAD_PER_AWP),
	m_krnl3x3AddrArr(MAX_QUAD_PER_AWP),
	m_krnl3x3BiasAddrArr(MAX_QUAD_PER_AWP)
{
   m_FAS_id = FAS_id;
   m_AWP_id = AWP_id;
}


AWP_cfg::~AWP_cfg()
{

}
