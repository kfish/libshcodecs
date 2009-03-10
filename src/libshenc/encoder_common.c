/*****************************************************************************
*                                                                            *
*  SuperH MEPG-4 Video Encode Library                                        *
*    (User Application Sample source for VPU4)                               *
*                                                                            *
*    Copyright (C) Renesas Technology Corp., 2005. All rights reserved.      *
*                                                                            *
*    Version@2.0 : avcbencsmp_common.c	(for MPEG-4 and H.264)    			 *
*								 2005/07/27 14:50 Renesas Technology Corp.   *
*                                                                            *
*****************************************************************************/

#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "avcbe.h"		/* SuperH MEPG-4&H.264 Video Encode Library Header */
#include "m4iph_vpu4.h"		/* SuperH MEPG-4&H.264 Video Driver Library Header */
#include "sh7722_vpu.h"
#include "encoder_common.h"		/* User Application Sample Header */
#include "m4driverif.h"

/*----------------------------------------------------------------------------------------------*/
/* Example of function created by the user, which is called when VPU4 reads register. */
/* (This function is common to MPEG-4&H.264 Encoder for VPU4.)  */
/*----------------------------------------------------------------------------------------------*/
unsigned long m4iph_sdr_read_vpu4(unsigned char *address,
				  unsigned char *buffer,
				  unsigned long count)
{
	m4iph_sdr_read(address, buffer, count);
	return (count);
}

/*----------------------------------------------------------------------------------------------*/
/* Example of function created by the user, which is called when VPU4 writes register. */
/* (This function is common to MPEG-4&H.264 Encoder for VPU4.)  */
/*----------------------------------------------------------------------------------------------*/
void m4iph_sdr_write_vpu4(unsigned char *address, unsigned char *buffer,
			  unsigned long count)
{
	m4iph_sdr_write(address, buffer, count);
}

/*-----------------------------------------------------------------------------*/
/* Example of function created by the user, which is called when cpb_buffer 
 * became over follow. */
/*-----------------------------------------------------------------------------*/
unsigned long avcbe_insert_filler_data_for_cpb_buffer(unsigned long
						      input_filler_size)
{
#if 0
	if (encoder->output_filler_enable == 1) {
		/* Output Stream Buffer Infomation For Filler Data */
		my_filler_data_buff_info.buff_top =
		    (unsigned char *) &my_filler_data_buff[0];
		my_filler_data_buff_info.buff_size =
		    MY_FILLER_DATA_BUFF_SIZE;
		if (input_filler_size > MY_FILLER_DATA_BUFF_SIZE) {
			input_filler_size = MY_FILLER_DATA_BUFF_SIZE;	/* clip to buffer max size */
		}
		/* Input Filler Data */
		encoder->output_filler_data =
		    avcbe_put_filler_data(&my_filler_data_buff_info,
					  AVCBE_ON, input_filler_size);
	} else {
		encoder->output_filler_data = input_filler_size;
	}
	return (encoder->output_filler_data);
#else
	return input_filler_size;
#endif
}

/*-------------------------------------------------------------------------*/
/*      display characters on terminal                                     */
/*-------------------------------------------------------------------------*/
void DisplayMessage(char *message_text, int line_feed_flag)
{
	printf("%s", message_text);
	if (line_feed_flag == 1) {
		printf("\n");
	}
}

/*-------------------------------------------------------------------------*/
/*      dummy function			                                           */
/*-------------------------------------------------------------------------*/
long m4iph_dec_continue()
{
	return (0);		/* dummy function */
}

/*float logf(float x) // dummy function
{
	float y;
	y = (float) log(x);
	return y;
}*/

long encode_time;

void encode_time_init(void)
{
	encode_time = 0;
}

unsigned long encode_time_get(void)
{
	return encode_time;
}

