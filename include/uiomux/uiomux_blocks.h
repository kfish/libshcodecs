#ifndef __UIOMUX_BLOCKS_H__
#define __UIOMUX_BLOCKS_H__


/**
 * request access to an IP block, by symbolic name

  _BLOCK_RENESAS_BEU
  _BLOCK_RENESAS_VEU
  _BLOCK_RENESAS_CEU
  _...

 * % or, should these names be independent of the actual hardware, ie.
 * % should this library be arch-independent? obviously that is not
 * % a requirement atm, but would be nice in the long run
 * % then perhaps these should be more configurable
 */
typedef int uiomux_blockmask_t;

#define UIOMUX_BLOCK_NONE (0)

/** BEU */
#define UIOMUX_BLOCK_SH_BEU (1<<0)

/** CEU */
#define UIOMUX_BLOCK_SH_CEU (1<<1)

/** VPU */
#define UIOMUX_BLOCK_SH_VPU (1<<2)

#endif /* __UIOMUX_BLOCKS_H__ */
