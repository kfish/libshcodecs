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

/*----------------------------------------------------------*/
/*       Output buffer size for EOS                         */
/*----------------------------------------------------------*/
#define END_CODE_BUFF_SIZE 32	/* byte unit */

/*----------------------------------------------------------*/
/*       SPS-stream-output buffer size                      */
/*----------------------------------------------------------*/
#define SPS_STREAM_BUFF_SIZE 256 /* byte unit */	/* this value must be multiple of 32 */

/*----------------------------------------------------------*/
/*       PPS-stream-output buffer size                      */
/*----------------------------------------------------------*/
#define PPS_STREAM_BUFF_SIZE 256 /* byte unit */	/* this value must be multiple of 32 */

/*----------------------------------------------------------*/
/*       SEI-stream-output buffer size                      */
/*----------------------------------------------------------*/
/*#define SEI_STREAM_BUFF_SIZE       175000 *//* Level 1 */
/*#define SEI_STREAM_BUFF_SIZE       2000000 *//* Level 2 */
/*#define SEI_STREAM_BUFF_SIZE       10000000 *//* Level 3 */
#define SEI_STREAM_BUFF_SIZE	100000

/*----------------------------------------------------------*/
/*    Filler-data-stream-output buffer size                 */
/*----------------------------------------------------------*/
#define FILLER_DATA_BUFF_SIZE 256



/*-------------- proto-type declaration ----------------------------*/

void encode_time_init(void);
unsigned long encode_time_get(void);

#endif				/* AVCBENCSMP */
