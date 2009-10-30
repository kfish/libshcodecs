/*
  Copyright (C) Renesas Technology Corp., 2003-2005. All rights reserved.
 */

#ifndef __SHCODECS_COMMON_H__
#define __SHCODECS_COMMON_H__

/** \file
 *
 * Common types.
 */

/**
 * Video formats which can be decoded by the VPU4
 */
typedef enum {
    SHCodecs_Format_NONE = 0,
    SHCodecs_Format_MPEG4 = 1,
    SHCodecs_Format_H264  = 2
} SHCodecs_Format;

/** Minimum frame width */
#define	SHCODECS_MIN_FX		48

/** Maximum frame width */
#define	SHCODECS_MAX_FX		1280

/** Minimum frame height */
#define	SHCODECS_MIN_FY		48

/** Maximum frame height */
#define	SHCODECS_MAX_FY		720

#endif /* __SHCODECS_COMMON_H__ */
