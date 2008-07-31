/*****************************************************************************
*                                                                            *
*  SuperH MEPG-4 Video Encode Library                                        *
*    (User Application Sample source for VPU4)                               *
*                                                                            *
*    Copyright (C) Renesas Technology Corp., 2005. All rights reserved.      *
*                                                                            *
*    Version@2.0 : avcbencsmp_mpeg4.c					     *
*				 2005/07/27 14:30 Renesas Technology Corp.   *
*                                                                            *
*****************************************************************************/

#include <stdio.h>  
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <setjmp.h>
#include <sys/time.h>
                  
#include "avcbe.h"	/* SuperH MEPG-4&H.264 Video Encode Library Header */
#include "m4iph_vpu4.h"	/* SuperH MEPG-4&H.264 Video Driver Library Header */  
#include "avcbencsmp.h"	/* User Application Sample Header */  

#include "avcbe_inner.h"
#include "QuantMatrix.h"

#include "capture.h"

#define DEBUG

#define ENC_WIDTH 640
#define ENC_HEIGHT 480

#ifdef CAPT_INPUT
#if 0
#include	"cpu_sh73380.h"
#include	"lcd.h"
#endif
#if 1
#include <pthread.h>
#endif
#endif /* CAPT_INPUT */
extern char *dummy_nal_buf;
#define USE_BVOP

/*** Image-capture-field area ***/
#define DEVICE_VPU "/dev/vpu"
#define DEVICE_SDR "/dev/sdr"
void *global_context=NULL;
long encode_time;

void encode_time_init(void)
{
	encode_time = 0;
}

unsigned long encode_time_get(void)
{
	return encode_time;
}

/*** Stream-output buffer to receive an encoding result in every one frame ***/

//static unsigned long sdr_read_my_stream_buff[MY_STREAM_BUFF_SIZE/4];		/* 4 bytes alignmen */   

/*** Other work-field area ***/
// #ifdef MULTI_STREAM             /* In the case of multiple streams */
// #define MY_MB_WORK_AREA_SIZE       78000   /* QCIF size DataPartioning=ON, Bitrate=64000/256000 : (6500 + 21500) + (9500 + 40000 + more) */
// #else   /* In the case of the number of the streams to encode is one */
// #define MY_MB_WORK_AREA_SIZE       28000   /* QCIF size DataPartioning=ON, Bitrate=64000 : 6500 + 21500 */
// #endif /* MULTI_STREAM */

//#define MY_MB_WORK_AREA_SIZE	76800  /*TODO hardcoded, remove later */
//static unsigned long my_mb_work_area[MY_MB_WORK_AREA_SIZE/4]; /* 4 bytes alignmen */
//#define my_sdr_mb_work_area sdr_base+(MY_MB_WORK_AREA_SIZE*2) /* 4 bytes alignmen */

extern avcbe_stream_info *my_context;
extern TAVCBE_FMEM LDEC_ARRY[];
extern TAVCBE_FMEM CAPTF_ARRY[];

unsigned long *my_frame_memory_capt[19];

unsigned long *my_frame_memory_ldec1;
unsigned long *my_frame_memory_ldec2;
unsigned long *my_frame_memory_ldec3;
unsigned long *my_stream_buff;
unsigned long *my_end_code_buff;
unsigned long *my_stream_buff_bak;
unsigned long *my_end_code_buff_bak;
extern unsigned long *my_work_area;	/* 4 bytes alignment */


extern long frame_counter_of_input;	/* the number of input frames for stream-1 */

int open_input_image_file(APPLI_INFO *);
int open_output_file(APPLI_INFO *);
void disp_context_info(void *context);

#ifdef CAPT_INPUT
/*------------------------------------------------------------------------------*/
/*   For input camera															*/
/*------------------------------------------------------------------------------*/
#define SCENE_CHANGE_THR	150000
#define ComplexValueMax		600

unsigned long  ComplexValue;
#if 0
extern unsigned long  ComplexValue;
extern	void vio_init( unsigned long Y_save_addr, unsigned long CrCb_save_addr, 
						unsigned long RGB_save_addr );
extern	long camera_capture( void );
#endif
unsigned long  ComplexValueSave[ComplexValueMax];
unsigned long  ComplexDiffSave[ComplexValueMax];
unsigned long swap_long(unsigned long ul_val);
unsigned long  cnt;
long  tmp;

#pragma section	_VRAM_BUFF
	unsigned short	VRAM_Buff[ENC_WIDTH][ENC_HEIGHT];	/* 16bit RGB */
#pragma section
#pragma section	_VRAM_BUFF1
	unsigned short	VRAM_Buff1[ENC_WIDTH][ENC_HEIGHT];	/* Y */
#pragma section
#pragma section	_VRAM_BUFF2

	unsigned short	VRAM_Buff2[ENC_WIDTH][ENC_HEIGHT];	/* Cb,Cr */
#pragma section
#pragma section	_VRAM_BUFF3
	unsigned long  VRAM_Buff3[ENC_WIDTH][ENC_HEIGHT];  /* 18bit RGB */
#pragma section

void	cnvs_data( void );


typedef struct {
    int                         video_buffer_size_max;
    volatile int                video_buffer_size_current;
    unsigned char       **video_ringbuffer;
    volatile int        video_head;
    volatile int        video_tail;
    volatile int        video_cnt;
    pthread_t           video_grabber_thread;
    pthread_mutex_t             video_buffer_mutex;

} common;

common common_buf;

extern void * video_grabber(void *);

#endif /* CAPT_INPUT */

extern int GetFromCtrlFTop(const char *control_filepath, ENC_EXEC_INFO *enc_exec_info,
						   long *stream_type);

/* Top of the user application sample source to encode */ 
/*int mpeg4_enc(void) */
extern APPLI_INFO ainfo;  /* User Application Data */

u_int32_t sdr_base;

unsigned long m4iph_vpu4_reg_base=0xfe900000;
#define KERNEL_MEMORY_FOR_VPU_BOTTOM 0xadffffff

unsigned long *kernel_memory_for_vpu_top;
void get_new_stream_buf(avcbe_stream_info *context, char *previous_stream_buff,
		long output_size, char **next_stream_buff, long *stream_buff_size);

void file_name_copy(void)
{
		strcpy(ainfo.file_path_buf_1, ainfo.enc_exec_info.buf_input_yuv_file_with_path);/* 入力ディレクトリ */
		strcpy(ainfo.file_path_buf_2, ainfo.enc_exec_info.buf_output_directry);/* 出力ディレクトリ */
		strcpy(ainfo.input_file_name_buf, ainfo.file_path_buf_1);
		strcat(ainfo.input_file_name_buf, "\\");
		strcpy(ainfo.output_file_name_buf, ainfo.file_path_buf_2);
		strcat(ainfo.output_file_name_buf, "\\");
		strcpy(ainfo.local_decode_file_name_buf, ainfo.file_path_buf_2);
		strcat(ainfo.local_decode_file_name_buf, "\\");
		strcpy(ainfo.log_file_name_buf, ainfo.file_path_buf_2);
//		strcat(ainfo.log_file_name_buf, "\\");	// <--- NG
		strcpy(ainfo.capt_file_name_buf, ainfo.file_path_buf_2);
		strcat(ainfo.capt_file_name_buf, "\\");
		strcpy(ainfo.rate_log_file_name_buf, ainfo.file_path_buf_2);
		strcat(ainfo.rate_log_file_name_buf, "\\");

		printf("ainfo.input_file_name_buf = %s \n", ainfo.input_file_name_buf);
		printf("ainfo.output_file_name_buf = %s \n", ainfo.output_file_name_buf);
}

int main(int argc, char *argv[])
{        
	int encode_return_code, loop_index, success_count, i;
	char message_buf[256];
	int return_code, case_no=0;
	long stream_type, width_height, max_frame;	/* 041201 */

	if (argc == 2) { /* 第1引数=コントロールファイル */
		strcpy(ainfo.ctrl_file_name_buf, argv[1]);
		return_code = GetFromCtrlFTop((const char *)ainfo.ctrl_file_name_buf, &(ainfo.enc_exec_info), &stream_type);
		if(return_code < 0){
			printf("Can't Open to Ctrolfile : %s\n", ainfo.ctrl_file_name_buf); /* 041217 */
			return(-1);
		}
#if 1
		strcpy(ainfo.file_path_buf_1, ainfo.enc_exec_info.buf_input_yuv_file_with_path);/* 入力ディレクトリ */
		strcpy(ainfo.file_path_buf_2, ainfo.enc_exec_info.buf_output_directry);/* 出力ディレクトリ */
		strcpy(ainfo.input_file_name_buf, ainfo.file_path_buf_1);
		strcat(ainfo.input_file_name_buf, "/");
		strcat(ainfo.input_file_name_buf, ainfo.enc_exec_info.buf_input_yuv_file);
		strcpy(ainfo.output_file_name_buf, ainfo.file_path_buf_2);
		strcat(ainfo.output_file_name_buf, "/");
		strcat(ainfo.output_file_name_buf, ainfo.enc_exec_info.buf_output_stream_file);
#if 1
		strcpy(ainfo.local_decode_file_name_buf, ainfo.file_path_buf_2);
		strcat(ainfo.local_decode_file_name_buf, "/");
		strcpy(ainfo.log_file_name_buf, ainfo.file_path_buf_2);
//		strcat(ainfo.log_file_name_buf, "/");	// <--- NG
		strcpy(ainfo.capt_file_name_buf, ainfo.file_path_buf_2);
		strcat(ainfo.capt_file_name_buf, "/");
		strcpy(ainfo.rate_log_file_name_buf, ainfo.file_path_buf_2);
		strcat(ainfo.rate_log_file_name_buf, "/");
		printf("!!!!!!!! ainfo.rate_log_file_name_buf = %s !!!!!!!!\n", ainfo.rate_log_file_name_buf);
		printf("ainfo.input_file_name_buf = %s \n", ainfo.input_file_name_buf);
		printf("ainfo.output_file_name_buf = %s \n", ainfo.output_file_name_buf);
#endif

#else
		file_name_copy();
#endif
	} else{  
		DisplayMessage("usage argv[0] case_number input_base_dir output_base_dir", 1);
		return -1;
	}                 

	ainfo.ceu = sh_ceu_open ("/dev/video0", ENC_WIDTH, ENC_HEIGHT);

	/* stream buffer 0 clear */
//	memset(sdr_read_my_stream_buff,0,sizeof(sdr_read_my_stream_buff));
	m4iph_vpu_open();
	m4iph_sdr_open();
	m4iph_sleep_time_init();
	encode_time_init();
	vpu4_clock_on();
	width_height = ainfo.enc_exec_info.xpic * ainfo.enc_exec_info.ypic;
	width_height += (width_height/2);

        printf ("width_height: %d\n", width_height);

	max_frame = 2;
	sdr_base = m4iph_sdr_malloc(width_height * (max_frame+3), 32);
	if (sdr_base == NULL) exit (1);
	kernel_memory_for_vpu_top = (unsigned long *)sdr_base;
	for (i=0; i<max_frame; i++ ) {
		my_frame_memory_capt[i] = (unsigned long *)(sdr_base + width_height*i);
		printf("my_frame_memory_capt[%d]=%x\n",i,my_frame_memory_capt[i]);
	}
	my_frame_memory_ldec1 = (unsigned long *)(sdr_base + width_height * i);
	i++;
	my_frame_memory_ldec2 = (unsigned long *)(sdr_base + width_height * i);
	i++;
	my_frame_memory_ldec3 = (unsigned long *)(sdr_base + width_height * i);
	my_stream_buff_bak = malloc(MY_STREAM_BUFF_SIZE + 31);
	my_stream_buff = ALIGN(my_stream_buff_bak, 32);
	my_end_code_buff_bak = malloc(MY_END_CODE_BUFF_SIZE + 31);
	my_end_code_buff = ALIGN(my_end_code_buff_bak, 32);
	success_count = 0;

	sh_ceu_start_capturing (ainfo.ceu);

	for (loop_index=0; loop_index < 1; loop_index++) {  
		/* encode on each case */
		if ( stream_type == AVCBE_H264 ) {
			encode_return_code = encode_1file_h264(ainfo.case_no, &ainfo, AVCBE_H264);   
		} else {
			encode_return_code = encode_1file_mpeg4(ainfo.case_no, &ainfo, stream_type);
		}
		if (encode_return_code < 0) {	/* encode error */
			sprintf(message_buf, "Encode Error  code=%d ", encode_return_code);
			DisplayMessage(message_buf, 1);
		} else {						/* encode success */ 
			sprintf(message_buf, "Encode Success ");
			DisplayMessage(message_buf, 1);	
			success_count++;
		}
	} 

	sh_ceu_stop_capturing (ainfo.ceu);

	m4iph_sdr_free(sdr_base,  width_height * (max_frame+3));
	m4iph_sdr_free(ainfo.vpu4_param.m4iph_temporary_buff_address, MY_STREAM_BUFF_SIZE);
	m4iph_sdr_close();
	m4iph_vpu_close();
	free(my_stream_buff_bak);
	free(my_end_code_buff_bak);
	free(my_work_area);
	free(dummy_nal_buf);
printf("Total encode time = %d(msec)\n",encode_time_get());
printf("Total sleep  time = %d(msec)\n",m4iph_sleep_time_get());
	/* TODO vpu4_reg_close(); */

	sh_ceu_close (ainfo.ceu);
}

/*---------------------------------------------------------------------*/
/*    encode on each case (for MPEG-4 or H.263) 					   */   
/*---------------------------------------------------------------------*/
int encode_1file_mpeg4(long case_no, APPLI_INFO *appli_info, long stream_type)
{  
	long return_code;  
	TAVCBE_STREAM_BUFF my_end_code_buff_info;
	long my_size=0;

	appli_info->error_return_function = 0;	/* add at Version2 */
	appli_info->error_return_code = 0;		/* add at Version2 */

	if(stream_type == AVCBE_MPEG4){
		DisplayMessage("MPEG-4 Encode Start! ", 1);
	}else{
		DisplayMessage("H.263 Encode Start! ", 1);
	}

	/*--- set the parameters of VPU4 (one of the user application's own functions) ---*/
	set_VPU4_param(case_no, &(appli_info->vpu4_param));
	
	/*--- The MPEG-4&H.264 Encoder Library API (common to MPEG-4&H.264 Decoder) 
	 * (required-1)@initialize VPU4 ---*/
	/* needs be called only once */
	return_code = m4iph_vpu4_init(&(appli_info->vpu4_param));
	if (return_code < 0) {	/* error */ 
		if (return_code == -1) {
			DisplayMessage(" encode_1file_mpeg4:m4iph_vpu4_init PARAMETER ERROR! ", 1);  
		}
		appli_info->error_return_function = -1;
		appli_info->error_return_code = return_code;
		return (-1);
	}

	/*--- The MPEG-4 Encoder Library API(required-2)@start encoding ---*/
	/* needs be called only once */
	avcbe_start_encoding();  /* initializes the encoder */

	/* Initialize Function Of Encoder(avcbe_set_default_param, avcbe_init_encode, 
	 * avcbe_init_memory) */
	return_code = init_for_encoder_mpeg4(case_no, appli_info, stream_type, &my_context);
	if(return_code != 0){
		return (-14);
	}

	/* encode process function for mpeg-4/H.263 (call avcbe_encode_picture func.) */
	return_code = encode_picture_for_mpeg4(case_no, appli_info, stream_type, my_context);
	if(return_code != 0){
		return (-15);
	}

	/*--- The MPEG-4&H.264 Encoder Library API (required-9)@ends encoding ---*/
	my_end_code_buff_info.buff_top = (unsigned char *)&my_end_code_buff[0];
	my_end_code_buff_info.buff_size = MY_END_CODE_BUFF_SIZE;

	/* return value is byte unit */
	return_code = avcbe_put_end_code(my_context, &my_end_code_buff_info, AVCBE_VOSE); 
	if (return_code <= 0) {
		if (return_code == -4) {
			DisplayMessage(" encode_1file_mpeg4:avcbe_close_encode OUTPUT BUFFER SIZE SHORT ERROR! ", 1);  
		}
		appli_info->error_return_function = -16;
		appli_info->error_return_code = return_code;

		return (-16);
	}else{
//		m4iph_sdr_read((unsigned char *)&my_end_code_buff[0], 	(unsigned char *)&sdr_read_my_stream_buff[0], 	MY_END_CODE_BUFF_SIZE);
//		cat_output_stream(appli_info, my_context, (char *)&sdr_read_my_stream_buff[0], 	MY_END_CODE_BUFF_SIZE);
//		my_size = fwrite((unsigned char *)&sdr_read_my_stream_buff[0], MY_END_CODE_BUFF_SIZE, 1, appli_info->output_file_fp);
cat_output_stream(appli_info, my_context, (char *)&my_end_code_buff[0], return_code);
my_size = fwrite((unsigned char *)&my_end_code_buff[0], return_code, 1, appli_info->output_file_fp);
	}
 
	return (0);
}

/*--------------------------------------------------------------*/
/* init for encoder						*/ 
/*--------------------------------------------------------------*/
long init_for_encoder_mpeg4(long case_no, APPLI_INFO *appli_info, long stream_type, avcbe_stream_info **context)
{
	long return_code=0;
	unsigned long nrefframe, nldecfmem, addr_temp;
	unsigned long *addr_y, *addr_c, *ptr;
	TAVCBE_WORKAREA WORK_ARRY[2];
	long area_width, area_height, i;

	/*--- The MPEG-4&H.264 Encoder Library API(required-3)@set default
	 *  values for the parameters  ---*/
	/* can be called only once or for the number of streams */
	return_code = avcbe_set_default_param(stream_type, AVCBE_RATE_NORMAL,
		   	&(appli_info->param), (void *)&(appli_info->other_options_mpeg4));
	if (return_code != 0) {	/* error */
		DisplayMessage(" encode_1file_mpeg4:avcbe_set_default_param ERROR! ", 1); 
		printf(" encode_1file_mpeg4:avcbe_set_default_param ERROR! \n"); 
		appli_info->error_return_function = -2;
		appli_info->error_return_code = return_code;
		return (-2);
	} 
	
	/*--- set parameters for use in encoding (one of the user application's 
	 * own functions) ---*/
	return_code = select_inputfile_set_param(case_no, appli_info); 
	/* add case_no at Version2 */
	if (return_code == -1) {	/* error */
		printf(" encode_1file_mpeg4:select_inputfile_set_param ERROR! \n");
		appli_info->error_return_function = -3;
		appli_info->error_return_code = return_code;
		return (-3);
	} 

	frame_counter_of_input = 0;
#ifndef CAPT_INPUT
#if 0
	/*--- open input YUV data file (one of the user application's own
	 *  functions) ---*/
	return_code = open_input_image_file(appli_info);
	if (return_code != 0) {	/* error */ 
		DisplayMessage(" encode_1file:open_input_image_file ERROR! ", 1);
		return (-6);
	} 
#endif
#endif
	/*--- open output file (one of the user application's own functions) ---*/
	return_code = open_output_file(appli_info);
	if (return_code != 0) {	/* error */ 
		DisplayMessage("  encode_1file:open_output_file ERROR! ", 1);
		return (-6);
	} 

	/* Capt Image Memory Size Check */
	if(WIDTH_HEIGHT_1_5 < (appli_info->param.avcbe_xpic_size * appli_info->param.avcbe_ypic_size * 3/2)){
		while(1);
	}

	DisplayMessage(" 1 calling avcbe_init_encode ", 1);

	/*--- The MPEG-4&H.264 Encoder Library API(required-4)@initialize the 
	 * variables ---*/	
	WORK_ARRY[0].area_size = MY_WORK_AREA_SIZE;
	WORK_ARRY[0].area_top = (unsigned char *)my_work_area;
#ifndef VPU4IP 
	WORK_ARRY[1].area_size = MY_MB_WORK_AREA_SIZE; 
	WORK_ARRY[1].area_top = (unsigned char *)my_mb_work_area;
#else
	WORK_ARRY[1].area_size = 0;
	WORK_ARRY[1].area_top = (unsigned char *)NULL;
#endif /* VPU4IP */

#ifndef VPU4IP 
	return_code = avcbe_init_encode(&(appli_info->param), 
				&(appli_info->paramR), &(appli_info->other_options_mpeg4),
				// RSM (avcbe_buf_continue_userproc_ptr)NULL, &WORK_ARRY[0], 
				(avcbe_buf_continue_userproc_ptr)get_new_stream_buf, &WORK_ARRY[0], 
				&WORK_ARRY[1], context);
#else
	return_code = avcbe_init_encode(&(appli_info->param), 
			&(appli_info->paramR), &(appli_info->other_options_mpeg4),
			// RSM (avcbe_buf_continue_userproc_ptr)NULL, &WORK_ARRY[0], NULL, 
			(avcbe_buf_continue_userproc_ptr)get_new_stream_buf, &WORK_ARRY[0], NULL, 
			context);
#endif /* VPU4IP */
	if (return_code < 0) {	/* error */ 
		if (return_code == -1) {
			DisplayMessage(
					" encode_1file_mpeg4:avcbe_init_encode PARAMETER ERROR! ",
				   	1);  
		} else if (return_code == -2) {
			DisplayMessage(
					" encode_1file_mpeg4:avcbe_init_encode PARAMETER CHANGED! (Maybe Error) ",
				   	1);  
		} else if (return_code == -3) {
			DisplayMessage(
					" encode_1file_mpeg4:avcbe_init_encode SEQUENCE ERROR! ",
				   	1);
		} else if (return_code == -4) {
			DisplayMessage(
					" encode_1file_mpeg4:avcbe_init_encode WORK-AREA SIZE SHORT ERROR! ", 
					1);
		}
		appli_info->error_return_function = -6;
		appli_info->error_return_code = return_code;
		return (-6);
	}
	return_code = GetFromCtrlFtoEncParamAfterInitEncode(appli_info->ctrl_file_name_buf, *context, &(appli_info->other_API_enc_param), &(appli_info->paramR));
printf("GetFromCtrlFtoEncParamAfterInitEncode=%d\n",return_code);
if(appli_info->other_options_mpeg4.avcbe_quant_type==1){/* add @061121 */
printf("avcbe_set_quant_type1()\n");
return_code = SetQuantMatrix(*context, QMAT_MPEG_TYPE_ANIME1_INTRA, QMAT_MPEG_TYPE_ANIME1_NONINTRA);
}

	nrefframe = 1;
	nldecfmem = 2;
	/* Local-decode-image Y */
	LDEC_ARRY[0].Y_fmemp = (unsigned char *)&my_frame_memory_ldec1[0];
	LDEC_ARRY[1].Y_fmemp = (unsigned char *)&my_frame_memory_ldec2[0];
	LDEC_ARRY[2].Y_fmemp = (unsigned char *)&my_frame_memory_ldec3[0];
	/* make it multiples of 16 */
	area_width = ((appli_info->param.avcbe_xpic_size +15)/16) *16; 
	area_height = ((appli_info->param.avcbe_ypic_size +15)/16) *16;
	/* Local-decode-image C */
	LDEC_ARRY[0].C_fmemp = (unsigned char *)(LDEC_ARRY[0].Y_fmemp + (area_width * area_height));
	LDEC_ARRY[1].C_fmemp = (unsigned char *)(LDEC_ARRY[1].Y_fmemp + (area_width * area_height));
	LDEC_ARRY[2].C_fmemp = (unsigned char *)(LDEC_ARRY[2].Y_fmemp + (area_width * area_height));

	/* Start address of the image-capture-field area must be arranged 
	 * in 32 bytes alignment. And, this area must be arranged in 
	 * non-cacheable space.*/
	addr_temp = (unsigned long)my_frame_memory_capt[0];
	addr_y = (unsigned long *)addr_temp;
	addr_c = (unsigned long *)(addr_temp +(area_width * area_height));
	/* Capture-image */
	CAPTF_ARRY[0].Y_fmemp = (unsigned char *)addr_y;
	CAPTF_ARRY[0].C_fmemp = (unsigned char *)addr_c;
#ifdef USE_BVOP /* 050106 */
	if (appli_info->other_options_mpeg4.avcbe_b_vop_num > 0) {
for(i=0;i<appli_info->other_options_mpeg4.avcbe_b_vop_num+1;i++){
		addr_temp = (unsigned long)my_frame_memory_capt[i];
		addr_y = (unsigned long *)addr_temp;
		addr_c = (unsigned long *)(addr_temp +(area_width * area_height));
		CAPTF_ARRY[i].Y_fmemp = (unsigned char *)addr_y;
		CAPTF_ARRY[i].C_fmemp = (unsigned char *)addr_c;
printf("addr_y=%x,addr_c=%x\n",addr_y,addr_c);
}
		nldecfmem = appli_info->other_options_mpeg4.avcbe_b_vop_num; /* LDEC_ARRY[2]AB-VOP[JfR[ho 050106 */
printf("b_vop_num=%d\n",nldecfmem);
//		m4vse_output_local_image_of_b_vop = AVCBE_ON; /* 050302 */
	}
#endif /* USE_BVOP */
	/*--- The MPEG-4&H.264 Encoder Library API(required-5)@specify the 
	 * address in the image-work-field area ---*/ 
	return_code = avcbe_init_memory(*context, nrefframe, nldecfmem, LDEC_ARRY, area_width, area_height);
printf("avcbe_init_memory=%d\n",return_code);

	if (return_code != 0) {
		if (return_code == -1) {
			DisplayMessage(
				" encode_1file_mpeg4:avcbe_init_memory PARAMETER ERROR! ", 1);  
		} else if (return_code == -3) {
			DisplayMessage(
				" encode_1file_mpeg4:avcbe_init_memory SEQUENCE ERROR! ", 1);
		} else if (return_code == -4) {
			DisplayMessage(
				" encode_1file_mpeg4:avcbe_init_encode WORK-AREA SIZE SHORT ERROR! ",
				   	1);
		}
		appli_info->error_return_function = -7;
		appli_info->error_return_code = return_code;
		return (-7);
	}

	return (0);
}

/*------------------------------------------------------------------------*/
/* Encode process function for MPEG-4/H.263                               */
/*------------------------------------------------------------------------*/
long encode_picture_for_mpeg4(long case_no, APPLI_INFO *appli_info, long stream_type, avcbe_stream_info *context)
{
	unsigned long ldec, ref1, ref2;
	long stream_bits, streamsize_per_frame, streamsize_total;
	long return_code;
	long frm;
	char messeage_buf[256];
	unsigned long *addr_y, *addr_c;
	TAVCBE_FMEM captfmem;
	long area_width, area_height;
	avcbe_frame_stat frame_stat;
	TAVCBE_STREAM_BUFF my_stream_buff_info;
	long my_size=0;
	struct timeval tv, tv1;
	struct timezone tz;
	long tm, ret;
#ifdef USE_BVOP /* 050106 */
	long index;
	unsigned long *addr_y_tbl[17], *addr_c_tbl[17]; 
	AVCBE_FRAME_CHECK frame_check_array[17];
#endif /* USE_BVOP */

#ifdef CAPT_INPUT
	long intra_judge;
	long ercd;
#if 1	
	int i;
#endif
#endif /* CAPT_INPUT */

	addr_y = (unsigned long *)CAPTF_ARRY[0].Y_fmemp;
	addr_c = (unsigned long *)CAPTF_ARRY[0].C_fmemp;

	area_width = ((appli_info->param.avcbe_xpic_size +15)/16) *16; 
	/* make it multiples of 16 */
	area_height = ((appli_info->param.avcbe_ypic_size +15)/16) *16;
#ifdef USE_BVOP /* 050106 */
	for(index = 0; index < (long)(appli_info->other_options_mpeg4.avcbe_b_vop_num +1); index++){	
		addr_y_tbl[index] = (unsigned long *)CAPTF_ARRY[index].Y_fmemp; 
		addr_c_tbl[index] = (unsigned long *)CAPTF_ARRY[index].C_fmemp;
	}        
#endif /* USE_BVOP */
	/* Index number of the image-work-field area (0 to N-1) 
	 * (for avcbe_set_image_pointer function) */
	ldec = 0;	
	ref1 = ref2 = 0;
	frm = 0;		/* Frame number to be encoded (for avcbe_encode_picture function) */  

	appli_info->frame_counter = 0;
	appli_info->frame_skip_num = 0;

	appli_info->output_write_size = 0;
	streamsize_total = 0;

	appli_info->set_intra = AVCBE_ANY_VOP;				/* Forced intra-mode flag */
	appli_info->output_type = AVCBE_OUTPUT_NONE;	    /* Header insertion flag */

#ifdef CAPT_INPUT
#if 0
	port_and_power_setting();

	IIC_Init();
	Lcd_Init();

	Lcd_Set_DisplayOn();
	vio_init( (unsigned long)&VRAM_Buff1[0][0],(unsigned long)&VRAM_Buff2[0][0],
		   		(unsigned long)&VRAM_Buff3[0][0] );
#endif
#if 1
	common_buf.video_buffer_size_max = 8;
	common_buf.video_ringbuffer = (unsigned char**)malloc(common_buf.video_buffer_size_max*sizeof(unsigned char*));
	if(!common_buf.video_ringbuffer) {
		DisplayMessage("cannot allocate video buffer\n", 1);	
		return -1;	
	}
    for (i = 0; i < common_buf.video_buffer_size_max; i++)
	    common_buf.video_ringbuffer[i] = NULL;

    common_buf.video_head = 0;
    common_buf.video_tail = 0;
    common_buf.video_cnt = 0;

	common_buf.video_buffer_size_current = 0;

	pthread_mutex_init(&common_buf.video_buffer_mutex, NULL);
	pthread_create(&common_buf.video_grabber_thread, NULL, video_grabber, &common_buf);
	
#endif
	cnt = 1;
	ComplexValue = 0;

	/* Capture-image */
//	CAPTF_ARRY[0].Y_fmemp = (unsigned char *)&VRAM_Buff1[0][0];
//	CAPTF_ARRY[0].C_fmemp = (unsigned char *)&VRAM_Buff2[0][0];

#endif /* CAPT_INPUT */

	captfmem.Y_fmemp = (unsigned char *)CAPTF_ARRY[0].Y_fmemp;
	captfmem.C_fmemp = (unsigned char *)CAPTF_ARRY[0].C_fmemp;
printf("appli_info->frame_number_to_encode=%d\n",appli_info->enc_exec_info.frame_number_to_encode);
appli_info->frame_number_to_encode = appli_info->enc_exec_info.frame_number_to_encode;
	while (1) {	/*---- Repeating by frame numbers -------------------------*/
printf("while---");
		if(appli_info->frame_number_to_encode == appli_info->frame_counter){
			break;
		}
#ifdef USE_BVOP /* 050106 */
		if (appli_info->other_options_mpeg4.avcbe_b_vop_num > 0) {
			return_code = avcbe_get_buffer_check(context, &frame_check_array[0]);
			if (return_code < 0) {	/* error */ 
				if (return_code == -1) {
					DisplayMessage(" encode_1file:avcbe_get_buffer_check PARAMETER ERROR! ", 1);  
				}
				return (-14);
			}
			for (index=0; index < (long)(appli_info->other_options_mpeg4.avcbe_b_vop_num+1); index++) { /* 050121 */
//			for (index=0; index < 3; index++) { /* 050121 */
				if (frame_check_array[index].avcbe_status == AVCBE_UNLOCK) {
					addr_y = addr_y_tbl[index];
					addr_c = addr_c_tbl[index];
					captfmem.Y_fmemp = (unsigned char *)addr_y; /* 050210 captfmem add */
					captfmem.C_fmemp = (unsigned char *)addr_c; /* 050210 captfmem add */
printf("BVOP index=%d\n",index);
					break;
				}
			}
		}
#endif /* USE_BVOP */
#ifdef CAPT_INPUT
	
#else

		printf ("encode_picture_for_mpeg4: capture_image ...\n");

		/* return_code = load_1frame_from_image_file(appli_info, addr_y, addr_c);*/
		return_code = capture_image (appli_info, addr_y, addr_c);
   		if (return_code < 0) {	/* error */ 
   			/*DisplayMessage(" encode_1file_mpeg4:load_1frame_from_image_file ERROR! ", 1);*/
   			DisplayMessage(" encode_1file_mpeg4:capture_image ERROR! ", 1);
			appli_info->error_return_function = -8;
			appli_info->error_return_code = return_code;
			return (-8);
		} 
#endif /* CAPT_INPUT */
		/*--- The MPEG-4 Encoder Library API(required-7)@specify the *
		 * address in the capture-image-field area ---*/
		if(stream_type != AVCBE_MPEG4){
			return_code = clip_image_data_for_H263(appli_info, context, addr_y, addr_c);
printf("H.263...clip_image_sata=%d\n",return_code);
		}
		return_code = avcbe_set_image_pointer(context, &captfmem, ldec, ref1, ref2);
		if (return_code != 0) {
			if (return_code == -1) {
				DisplayMessage(" encode_1file_mpeg4:avcbe_set_image_pointer PARAMETER ERROR! ", 1);  
			}
			appli_info->error_return_function = -9;
			appli_info->error_return_code = return_code;
			return (-9);
		}

		/*--- The MPEG-4 Encoder Library API (required-8)@encode each 
		 * screen of display data ---*/
		my_stream_buff_info.buff_top = 	(unsigned char *)&my_stream_buff[0];
		my_stream_buff_info.buff_size = MY_STREAM_BUFF_SIZE; // TODO: buf size
memset(&my_stream_buff[0], 0, MY_STREAM_BUFF_SIZE);
#ifdef CAPT_INPUT
		vpu4_clock_on();
		return_code = avcbe_encode_picture(context, frm, intra_judge, 
				appli_info->output_type, &my_stream_buff_info, NULL);
		vpu4_clock_off();
#else
#ifdef DEBUG
		printf ("encode_picture_for_mpeg4: avcbe_encode_picture (%x, %ld, %ld, %ld, {%ld, %x})\n",
			context, frm, appli_info->set_intra, appli_info->output_type,
			my_stream_buff_info.buff_size, my_stream_buff_info.buff_top);
#endif
		vpu4_clock_on();
gettimeofday(&tv, &tz);
//printf("enc_pic0=%ld,",tv.tv_usec);
		return_code = avcbe_encode_picture(context, frm, appli_info->set_intra,	appli_info->output_type, &my_stream_buff_info, NULL);
gettimeofday(&tv1, &tz);

#ifdef DEBUG
		printf ("encode_picture_for_mpeg4: avcbe_encode_picture returned %dn", return_code);
#endif
		

//printf("ret=%d,enc_pic1=%ld,",return_code,tv1.tv_usec);
tm = (tv1.tv_usec-tv.tv_usec)/1000;
if ( tm<0 ) {
	tm = 1000-(tv.tv_usec-tv1.tv_usec)/1000;
}
encode_time += tm;
printf("Total encode time = %dmsec(%dmsec),",tm,encode_time_get());
printf("Total sleep  time = %d(msec)\n",m4iph_sleep_time_get());
		vpu4_clock_off();
#endif /* CAPT_INPUT */

		if (return_code < 0) {	/* error */ 
			if (return_code == AVCBE_ENCODE_ERROR) { /* -1 */
				DisplayMessage(	" encode_1file_mpeg4:avcbe_encode_picture PARAMETER ERROR! ", 1);  

			} else if (return_code == AVCBE_NOT_IN_ORDER_ERROR) { /* -3 */
				DisplayMessage(	" encode_1file_mpeg4:avcbe_encode_picture SEQUENCE ERROR! ", 1);

			} else if (return_code == AVCBE_OUTPUT_BUFFER_SHORT_ERROR) { /* -4 */
				DisplayMessage(	" encode_1file_mpeg4:avcbe_encode_picture OUTPUT BUFFER SIZE SHORT ERROR! ", 1);

			} else if (return_code == AVCBE_VPU_ERROR_AFTER_ENCODING) { /* -6 */
				DisplayMessage(" encode_1file_mpeg4:avcbe_encode_picture VPU4 ERROR! ", 1);

			} else if (return_code == AVCBE_TEMPORARY_BUFFER_SHORT_ERROR) { /* -8 */
				DisplayMessage(	" encode_1file_mpeg4:avcbe_encode_picture TEMPORARY_BUFFER SIZE SHORT ERROR! ",	1);

			} else if (return_code == AVCBE_WORK_AREA_SHORT_ERROR) { /* -99 */
				DisplayMessage(" encode_1file_mpeg4:avcbe_encode_picture OTHER WORK SIZE SHORT ERROR! ", 1);

			} else if (return_code == AVCBE_QUANT_MATRIX_NOT_SPECIFIED_ERROR) { /* -11 */
				DisplayMessage(" encode_1file_mpeg4:avcbe_encode_picture QUANT_MATRIX_NOT_SPECIFIED ERROR! ", 1);
			}
			appli_info->error_return_function = -11;
			appli_info->error_return_code = return_code;
			return (-11);
		} else if (return_code == AVCBE_ENCODE_SUCCESS) { /* 0 */
			sprintf(messeage_buf," encode_1file_mpeg4:avcbe_encode_picture SUCCESS  frm=%d,seq_no=%d ", frm, appli_info->frame_counter);
			DisplayMessage(messeage_buf, 1);
			/* the second parameter 'ldec' value must be changed when *
			 * the avcbe_set_image_pointer function is called on next time. */
		} else if (return_code == AVCBE_FRAME_SKIPPED) { /* 1 */
			sprintf(messeage_buf, " encode_1file_mpeg4:avcbe_encode_picture THIS FRAME SKIPPED(Not Encoded)  frm=%d,seq_no=%d ", frm, appli_info->frame_counter);
			DisplayMessage(messeage_buf, 1);
			/* the second parameter 'ldec' value must NOT be changed *
			 * when the avcbe_set_image_pointer function is called on next time. */
			appli_info->frame_skip_num++;
		} else if (return_code == AVCBE_EMPTY_VOP_OUTPUTTED) { /* 2 */
			sprintf(messeage_buf, " encode_1file_mpeg4:avcbe_encode_picture EMPTY VOP CREATED  frm=%d,seq_no=%d ", frm, appli_info->frame_counter);
			DisplayMessage(messeage_buf, 1); 
			/* the second parameter 'ldec' value must NOT be changed *
			 * when the avcbe_set_image_pointer function is called on next time. */
	
		} else if (return_code == AVCBE_B_VOP_OUTPUTTED) { /* 4 */
			sprintf(messeage_buf, " encode_1file_mpeg4:avcbe_encode_picture B-VOP OUTPUTTED  frm=%d,seq_no=%d ", frm, appli_info->frame_counter);
			DisplayMessage(messeage_buf, 1);
			/* the second parameter 'ldec' value is ignored when B-VOP is used */
		}else{
			sprintf(messeage_buf, " encode_1file_mpeg4:avcbe_encode_picture UNKNOWN RETURN CODE=%d  frm=%d,seq_no=%d ",return_code, frm, appli_info->frame_counter);
			DisplayMessage(messeage_buf, 1); 
		}
		avcbe_get_last_frame_stat(context, &frame_stat);
		if((return_code == AVCBE_ENCODE_SUCCESS) || (return_code == AVCBE_B_VOP_OUTPUTTED) || (return_code == AVCBE_EMPTY_VOP_OUTPUTTED)){
			if (return_code == AVCBE_ENCODE_SUCCESS) { /* add at Version2 */
				if (ldec == 0) {
					ldec = 1;
					ref1 = 0;
				} else {
					ldec = 0;
					ref1 = 1;
				}
			}

			appli_info->frm = frm;
			stream_bits = frame_stat.avcbe_frame_n_bits;
			/* get the size of bitstream (byte unit) */
			streamsize_per_frame = (stream_bits/8);	
//			m4iph_sdr_read((unsigned char *)&my_stream_buff[0],(unsigned char *)&sdr_read_my_stream_buff[0], streamsize_per_frame);
			cat_output_stream(appli_info, context, (char *)&my_stream_buff[0], streamsize_per_frame); 
			my_size = fwrite((unsigned char *)&my_stream_buff[0], 1, streamsize_per_frame, appli_info->output_file_fp);
//printf("streamsize_per_frame=%d,fwrite = %dbyte\n",streamsize_per_frame,my_size);
//cat_output_stream(appli_info, context, (char *)&sdr_read_my_stream_buff[0], streamsize_per_frame); 
//my_size = fwrite((unsigned char *)&sdr_read_my_stream_buff[0], streamsize_per_frame, 1, appli_info->output_file_fp);
		}
		frm += appli_info->frame_no_increment;
		appli_info->frame_counter++;
//printf("appli_info->frame_counter=%d\n",appli_info->frame_counter);
	} /* while */	

#ifdef CAPT_INPUT
#if 1
	pthread_join(common_buf.video_grabber_thread, NULL);
	pthread_mutex_destroy(&common_buf.video_buffer_mutex);

    if (common_buf.video_ringbuffer) {
	    int i;
	    for (i = 0; i < common_buf.video_buffer_size_current; i++) {
		        free(common_buf.video_ringbuffer[i]);
			    }
	    free(common_buf.video_ringbuffer);
	    }

#endif	
#endif	
	/*------ End of repeating by frame numbers ------------*/

	return (0);
}

void get_new_stream_buf(avcbe_stream_info *context, char *previous_stream_buff,
		long output_size, char **next_stream_buff, long *stream_buff_size)
{
	printf(" get_new_stream_buf: p_s_b = %p, output_size = %X s_b_s=0x%X \n",
			previous_stream_buff, output_size, *stream_buff_size);
	*next_stream_buff = (unsigned char *)&my_end_code_buff[1];
	*stream_buff_size = MY_STREAM_BUFF_SIZE;

}


