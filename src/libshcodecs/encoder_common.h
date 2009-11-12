/*
 * libshcodecs: A library for controlling SH-Mobile hardware codecs
 * Copyright (C) 2009 Renesas Technology Corp.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA  02110-1301 USA
 */

#ifndef	AVCBENCSMP_H
#define	AVCBENCSMP_H

#include <stdio.h>

#define CAPTURE_720P

//#define MAX_D1

//#define CAPTURE_VGA 1

#ifdef CAPTURE_720P

/*----------------------------------------------------------*/
/*       Other work-field area size                         */
/*----------------------------------------------------------*/
#define MY_WORK_AREA_SIZE	1024*512

/*----------------------------------------------------------*/
/*       Stream-output buffer size for 1 frame              */
/*----------------------------------------------------------*/
#define MY_STREAM_BUFF_SIZE	1024*256     /* byte unit */	/* this value must be multiple of 32 */

#elif defined(MAX_D1)

/*----------------------------------------------------------*/
/*       Other work-field area size                         */
/*----------------------------------------------------------*/
#define MY_WORK_AREA_SIZE	1024*512

/*----------------------------------------------------------*/
/*       Stream-output buffer size for 1 frame              */
/*----------------------------------------------------------*/
#define MY_STREAM_BUFF_SIZE	1024*256     /* byte unit */	/* this value must be multiple of 32 */

#elif defined(CAPTURE_VGA)
/*----------------------------------------------------------*/
/*       Other work-field area size                         */
/*----------------------------------------------------------*/
#define MY_WORK_AREA_SIZE	(101376*4) /*76800 One stream */	/* QVGA */

/*----------------------------------------------------------*/
/*       Stream-output buffer size for 1 frame              */
/*----------------------------------------------------------*/
#define MY_STREAM_BUFF_SIZE	(400000)     /* byte unit */	/* this value must be multiple of 32 */

#else				/* Not D1, or CAPTURE_VGA */
/*----------------------------------------------------------*/
/*       Other work-field area size                         */
/*----------------------------------------------------------*/
#define MY_WORK_AREA_SIZE	101376 /*76800 One stream */	/* QVGA */

/*----------------------------------------------------------*/
/*       Stream-output buffer size for 1 frame              */
/*----------------------------------------------------------*/
#define MY_STREAM_BUFF_SIZE	160000     /* byte unit */	/* this value must be multiple of 32 */
#endif

/*----------------------------------------------------------*/
/*       Output buffer size for EOS                         */
/*----------------------------------------------------------*/
#define MY_END_CODE_BUFF_SIZE 32	/* byte unit */

/*----------------------------------------------------------*/
/*       SPS-stream-output buffer size                      */
/*----------------------------------------------------------*/
#define MY_SPS_STREAM_BUFF_SIZE 256 /* byte unit */	/* this value must be multiple of 32 */

/*----------------------------------------------------------*/
/*       PPS-stream-output buffer size                      */
/*----------------------------------------------------------*/
#define MY_PPS_STREAM_BUFF_SIZE 256 /* byte unit */	/* this value must be multiple of 32 */

/*----------------------------------------------------------*/
/*       SEI-stream-output buffer size                      */
/*----------------------------------------------------------*/
/*#define MY_SEI_STREAM_BUFF_SIZE       175000 *//* Level 1 */
/*#define MY_SEI_STREAM_BUFF_SIZE       2000000 *//* Level 2 */
/*#define MY_SEI_STREAM_BUFF_SIZE       10000000 *//* Level 3 */
#define MY_SEI_STREAM_BUFF_SIZE	100000

/*----------------------------------------------------------*/
/*    Filler-data-stream-output buffer size                 */
/*----------------------------------------------------------*/
#define MY_FILLER_DATA_BUFF_SIZE 256



/*-------------- proto-type declaration ----------------------------*/

void encode_time_init(void);
unsigned long encode_time_get(void);

#define ALIGN_NBYTES(a, w) \
	(void *)(((unsigned long)(a) + (w) - 1) & ~((w) - 1))

#endif				/* AVCBENCSMP */
