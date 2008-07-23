/********************************************************************************
*                                                                               *
*  SuperH MEPG-4 Video Encode Library                                           *
*    (User Application Sample source for VPU4)                                  *
*                                                                               *
*    Copyright (C) Renesas Technology Corp., 2002-2005. All rights reserved.    *
*                                                                               *
*    Version@2.0 : avcbeinputuser.c 2005/07/27 13:40  Renesas Technology Corp. *
*                                                                               *
********************************************************************************/
#include <stdio.h>  
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <setjmp.h>

#ifdef VPU4IP /* for SH C compiler */
//#include <machine.h>
#endif /* VPU4IP */
                             
#include "avcbe.h"		/* SuperH MEPG-4&H.264 Video Encode Library Header */
#include "m4iph_vpu4.h"	/* SuperH MEPG-4&H.264 Video Driver Library Header */
#include "avcbencsmp.h" 

extern unsigned long my_yuv_data[];	/* array for input YUV data */   
extern long frame_counter_of_input;	/* the number of input frames for stream-1 */ 

int open_input_image_file(APPLI_INFO *appli_info)
{   
	frame_counter_of_input = 0;
	appli_info->input_yuv_fp = NULL;
	appli_info->input_yuv_fp = fopen(appli_info->input_file_name_buf, "rb" );

	if (appli_info->input_yuv_fp == NULL) {
		printf("ERROR: can't open file %s \n", appli_info->input_file_name_buf);
		return (-1);
	}
// #ifdef MULTI_STREAM		/* In the case of multiple streams */
	// appli_info->input_yuv_fp_2 = NULL;
	// appli_info->input_yuv_fp_2 = fopen(appli_info->input_file_name_buf_2, "rb" );

	// if (appli_info->input_yuv_fp_2 == NULL) {
		// return (-1);
	// }
// #endif /* MULTI_STREAM */

	return (0);
}

int open_output_file(APPLI_INFO *appli_info)
{   
	appli_info->output_file_fp = NULL;
	appli_info->output_file_fp = fopen(appli_info->output_file_name_buf, "wb" );

	if (appli_info->output_file_fp == NULL) {
		return (-1);
	}
// #ifdef MULTI_STREAM		/* In the case of multiple streams */
	// appli_info->output_file_fp_2 = NULL;
	// appli_info->output_file_fp_2 = fopen(appli_info->output_file_name_buf_2, "wb" );
// 
	// if (appli_info->output_file_fp_2 == NULL) {
		// return (-1);
	// }
// #endif /* MULTI_STREAM */

	return (0);
}

/* copy yuv data to the image-capture-field area each frame */
int load_1frame_from_image_file(APPLI_INFO *appli_info, 
		unsigned long *addr_y, unsigned long *addr_c)
{   
	int read_size;
	long hsiz, ysiz, index, index2;
	FILE *input_yuv_fp;
//	unsigned long w_addr_yuv[320*240/8*3];
	unsigned long *w_addr_yuv;	
	unsigned long wnum;
	unsigned char *CbCr_ptr, *Cb_buf_ptr, *Cr_buf_ptr, *ptr;

	if (frame_counter_of_input == appli_info->frame_number_to_encode) {
		return (0);
	}
	input_yuv_fp = appli_info->input_yuv_fp;
	hsiz = appli_info->param.avcbe_xpic_size;
	ysiz = appli_info->param.avcbe_ypic_size;
	frame_counter_of_input++; 

	/* TODO MULTI_STREAM code remoevd */

	if (input_yuv_fp == NULL) {
		return (-1);
	}
	/* Input file data to user memory */
	w_addr_yuv = malloc(hsiz*ysiz*2);
//printf("x=%d,y=%d,addr_y=%X,addr_c=%X,",hsiz,ysiz,addr_y,addr_c);
	if ( w_addr_yuv == NULL ) {
		printf("load_1frame_from_image_file: malloc error.\n");
		exit(-1);
	}
	memset(w_addr_yuv,0,hsiz*ysiz);
	read_size = fread(w_addr_yuv, 1, hsiz*ysiz, input_yuv_fp);
	if (read_size <= 1) {
		free(w_addr_yuv);
		return (1);
	}
	/* write memory data size offset make to multiples of 16 */
	wnum = (((hsiz +15)/16) * 16) * (((ysiz +15)/16) * 16); 
	/* Input file data to user memory */
	CbCr_ptr = malloc(hsiz*ysiz/2);
	Cb_buf_ptr = malloc(hsiz*ysiz/2);
	Cr_buf_ptr = malloc(hsiz*ysiz/2);
	if ( Cb_buf_ptr==NULL || Cr_buf_ptr == NULL ) {
		printf("malloc error \n");
		exit(-1);
	} 
//printf("appli_info->enc_exec_info.yuv_CbCr_format == %d\n",appli_info->enc_exec_info.yuv_CbCr_format);
	if (appli_info->enc_exec_info.yuv_CbCr_format == 1) { /* 1:CbSCrS */
//printf("appli_info->yuv_CbCr_format == 1\n");
		ptr = CbCr_ptr;
		read_size = fread(Cb_buf_ptr, 1, (hsiz*ysiz/4), input_yuv_fp);
		read_size = fread(Cr_buf_ptr, 1, (hsiz*ysiz/4), input_yuv_fp);
		for (index=0; index < (hsiz*ysiz/4); index++) {
			*ptr++ = *(Cb_buf_ptr +index);
			*ptr++ = *(Cr_buf_ptr +index);
		}
	} else if (appli_info->enc_exec_info.yuv_CbCr_format == 2) { /* 2:Cb0,Cr0,Cb1,Cr1,... */
//printf("appli_info->yuv_CbCr_format == 2\n");
		read_size = fread(CbCr_ptr, 1, ((hsiz*ysiz/4)*2), input_yuv_fp);
		if(read_size <= 1){
			return (-1);
		}
	} else if (appli_info->enc_exec_info.yuv_CbCr_format == 3) { /* 3:Cb1C,Cr1C,... */
//printf("appli_info->yuv_CbCr_format == 3\n");
		ptr = CbCr_ptr;
		for (index=0; index < (ysiz/2); index++) {
			read_size = fread(Cb_buf_ptr, (sizeof( unsigned char )), (hsiz/2), input_yuv_fp);
			read_size = fread(Cr_buf_ptr, (sizeof( unsigned char )), (hsiz/2), input_yuv_fp);
			for (index2=0; index2 < (hsiz/2); index2++) {
				*ptr++ = *(Cb_buf_ptr +index2);
				*ptr++ = *(Cr_buf_ptr +index2);
			}
		}
	} else {
//printf("appli_info->yuv_CbCr_format == else\n");
		read_size = fread(CbCr_ptr, 1, ((hsiz*ysiz/4)*2), input_yuv_fp);
		if(read_size <= 1){
			return (-1);
		}
}
	/* Write image data to kernel memory for VPU */
	m4iph_sdr_write((unsigned char *)addr_y, (unsigned char *)w_addr_yuv, wnum);
	m4iph_sdr_write((unsigned char *)addr_c, (unsigned char*)CbCr_ptr, wnum/2);
	free(w_addr_yuv);
	free(Cb_buf_ptr);
	free(Cr_buf_ptr);
	free(CbCr_ptr);
	return (0);
}   

/* close input YUV data file */ 
void close_input_image_file(APPLI_INFO *appli_info)
{
	if (appli_info->input_yuv_fp != NULL) {
		fclose(appli_info->input_yuv_fp);
		appli_info->input_yuv_fp = NULL;
	}

// #ifdef MULTI_STREAM		/* In the case of multiple streams */
	// if (appli_info->input_yuv_fp_2 != NULL) {
		// fclose(appli_info->input_yuv_fp_2);
		// appli_info->input_yuv_fp_2 = NULL;
	// }
// #endif /* MULTI_STREAM */
}
     
/* close output file */ 
void close_output_file(APPLI_INFO *appli_info)
{
	if (appli_info->output_file_fp != NULL) {
		fclose(appli_info->output_file_fp);
		appli_info->output_file_fp = NULL;
	}

// #ifdef MULTI_STREAM		/* In the case of multiple streams */
	// if (appli_info->output_file_fp_2 != NULL) {
		// fclose(appli_info->output_file_fp_2);
		// appli_info->output_file_fp_2= NULL;
	// }
// #endif /* MULTI_STREAM */
}
     
int clip_image_data_for_H263(APPLI_INFO *appli_info, avcbe_stream_info *context, unsigned long *addr_y, unsigned long *addr_c)
{
	long width, height, index;
	unsigned char *YCbCr_ptr;
	int return_value=0;
	unsigned long *addr_y_2, *addr_c_2;

	width = appli_info->param.avcbe_xpic_size;
	height = appli_info->param.avcbe_ypic_size;
printf("clip_image: width=%d, height=%d,addr_y=%X,addr_c=%X\n",width,height,addr_y,addr_c);
	addr_y_2 = malloc(width*height);
	addr_c_2 = malloc(width*height/2);
	m4iph_sdr_read((unsigned char *)addr_y, (unsigned char *)addr_y_2, width*height);
	/* Clip Y-data */
	YCbCr_ptr = (unsigned char *)addr_y_2;
	for (index=0; index < (width * height); index++) {
		if ((*YCbCr_ptr) < 1) {
			*YCbCr_ptr = 1;
			return_value = 1; /* clipped */ 
		} else if ((*YCbCr_ptr) > 254) {
			*YCbCr_ptr = 254;
			return_value = 1; /* clipped */
		}
		YCbCr_ptr++;
	}
	m4iph_sdr_write((unsigned char *)addr_y, (unsigned char *)addr_y_2, width*height);
	/* Clip Cb-data, Cr-data */
	m4iph_sdr_read((unsigned char *)addr_c, (unsigned char *)addr_c_2, width*height/2);
	YCbCr_ptr = (unsigned char *)addr_c_2;
	for (index=0; index < ((width * height)/2); index++) {
		if ((*YCbCr_ptr) < 1) {
			*YCbCr_ptr = 1;
			return_value = 1; /* clipped */ 
		} else if ((*YCbCr_ptr) > 254) {
			*YCbCr_ptr = 254;
			return_value = 1; /* clipped */
		}
		YCbCr_ptr++;
	}
	m4iph_sdr_write((unsigned char *)addr_c, (unsigned char *)addr_c_2, width*height/2);
	free(addr_y_2);
	free(addr_c_2);
	return (return_value);
}