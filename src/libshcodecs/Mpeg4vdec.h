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

#ifndef _M4VSDECPUBLIC_H_
#define _M4VSDECPUBLIC_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct t_m4vsdhead {
	/** Visual Object Sequence & Video Object **/
	long profile_and_level_indication;                      /*(8bit)*/
	long is_visual_object_identifier;                       /*(1bit)*/
	long visual_object_verid;                               /*(4bit)*/
	long visual_object_priority;                            /*(3bit)*/
	long visual_object_type;                                /*(4bit)*/

	/** Video Object Layer **/
	long random_accessible_vol;                             /*(1bit)*/
	long video_object_type_indication;                      /*(8bit)*/
	long is_object_layer_identifier;                        /*(1bit)*/
	long video_object_layer_verid;                          /*(4bit)*/
	long video_object_layer_priority;                       /*(3bit)*/
	long aspect_ratio_info;                                 /*(1bit)*/
	long par_width;                                         /*(8bit)*/
	long par_height;                                        /*(8bit)*/
	long vol_control_parameters;                            /*(1bit)*/
	long chroma_format;                                     /*(2bit)*/
	long low_delay;                                         /*(1bit)*/
	long vbv_parameters;                                    /*(1bit)*/
	long first_half_bit_rate;                               /*(15bit)*/
	long latter_half_bit_rate;                              /*(15bit)*/
	long first_half_vbv_buffer_size;                        /*(15bit)*/
	long latter_half_vbv_buffer_size;                       /*(3bit)*/
	long first_half_vbv_occupancy;                          /*(11bit)*/
	long latter_half_vbv_occupancy;                         /*(15bit)*/
	long video_object_layer_shape;                          /*(2bit)*/
	long vop_time_increment_resolution;                     /*(16bit)*/
	long fixed_vop_rate;                                    /*(1bit)*/
	long fixed_vop_time_increment;                          /*(1-16bit)*/
	long video_object_layer_width;                          /*(13bit)*/
	long video_object_layer_height;                         /*(13bit)*/
	long resync_marker_disable;                             /*(1bit)*/
	long data_partitioned;                                  /*(1bit)*/
	long reversible_vlc;                                    /*(1bit)*/
} M4VSDHEAD;

typedef struct t_m4vsdvolparam {
	long interlaced;
	long obmc_disable;
	long spright_enable;
	long not_8bit;
	long quant_type;
	long complexity_estimation_disable;
	long estimation_method;
	long vop_complex_skip_I;
	long vop_complex_skip_P;
	long vop_complex_skip_B;
	long scalability;
	long hierarchy_type;
	long ref_layer_id;
	long enhancement_type;
	char matrix_intra[64];
	char matrix_inter[64];
} M4VSDVOLPARAM;

typedef long (* t_m4vsd_fread)( unsigned char *, long );
extern t_m4vsd_fread m4vsd_fread;

typedef long (* M4VSD_CONTINUE_FUNC)(const void *context, char **next_stream_buff, unsigned long *stream_buff_size);

#define M4VSD_TYPE_MPEG4 0
/*---------------max number of frame memories */
#define M4VSD_MAX_FM 32
/*---------------size of work area for m4vsd_set_workarea() */
#define M4VSD_WORKAREA_SIZE 600

/*---------------return from decode_picture() */
#define M4VSD_PIC_NOERROR_I                     (0)
#define M4VSD_PIC_NOERROR_P                     (1)
#define M4VSD_PIC_NOERROR_B                     (2)
#define M4VSD_PIC_NOTCODED_VOP                  (4)
#define M4VSD_PIC_TIMEOUT                       (-2)
#define M4VSD_PIC_ABORT                         (-3)
#define M4VSD_PIC_EOS                           (-4)
#define M4VSD_PIC_LARGE                         (-5)
#define M4VSD_PIC_FEWERMB                       (-6)
#define M4VSD_PIC_FMTERROR                      (-8)
#define M4VSD_PIC_OPTERROR                      (-9)
#define M4VSD_MB_OVRRUN                         (-10)
#define M4VSD_STUFFING_ERR                      (-11)
#define M4VSD_MARKER_BIT_ERR                    (-12)
#define M4VSD_ILLEGAL_DATA                      (-14)
#define M4VSD_ILLEGAL_CODE                      (-15)
#define M4VSD_ILLEGAL_MTYPE                     (-16)
#define M4VSD_VO_TYPE_ISNT_VIDEO                (-17)
#define M4VSD_PARAM_ERROR                       (-18)
#define M4VSD_BUFFER_OVERRUN                    (-19)
#define M4VSD_TIME_OUT                          (-20)
#define M4VSD_PIC_ERROR                         (-22)
#define M4VSD_VPU_ERROR_00                      (-40)
#define M4VSD_VPU_ERROR_01                      (-41)
#define M4VSD_VPU_ERROR_02                      (-42)
#define M4VSD_VPU_ERROR_03                      (-43)
#define M4VSD_VPU_ERROR_04                      (-44)
#define M4VSD_VPU_ERROR_05                      (-45)
#define M4VSD_VPU_ERROR_06                      (-46)
#define M4VSD_VPU_ERROR_07                      (-47)
#define M4VSD_VPU_ERROR_08                      (-48)
#define M4VSD_VPU_ERROR_09                      (-49)
#define M4VSD_OTHER_ERROR                       (-128)

/*---------------public proto types */
long m4vsd_init( void * workarea, long workarea_size, unsigned long nfmem, 
			  TAVCBD_FMEM fmema[],
			  long wx, long wy,
			  long *dp1_addr, long *dp2_addr,
			  void **context);
long m4vsd_init_code( long *context, unsigned char *buffer, long buffer_size,
                      unsigned char *usrdata, long usrdata_size );
long m4vsd_init_sequence( long *context, long initialize_all, long error_cncl_flg );
long m4vsd_sc_parse( long *context, long sc_timeout_nbit,
		     long vol_autolock, long vol_parse_only );
long m4vsd_init_memory( long *context, unsigned long nfmem, TAVCBD_FMEM *fmemp,
			long wx, long wy,
			long *dp1_addr, long *dp2_addr,
			long fmclrflg );

void m4vsd_init_picture( long *context );
long m4vsd_close_sequence( long *context );
long m4vsd_set_header(long *context, M4VSDHEAD *m4vsdh);
long m4vsd_get_header( long *context, M4VSDHEAD *m4vsdh );
void m4vsd_get_maxMBxy( long *context, char *mbx, char *mby );
long m4vsd_get_P_source_format(long *context, short *xs, short *ys );
/* allow minus value for ref, dec and disp */
long m4vsd_set_image_pointer( long *context, long ref, long dec, long disp,
				unsigned char *stream_buff,
				unsigned long stream_size );
long m4vsd_decode_picture( long *context, long max_read_bit );
void m4vsd_set_resume_err( long *context, long err_mode_flg, long conceal_mode );
long m4vsd_set_filter_mode( void *context, long filter_mode, long filter_select, TAVCBD_FMEM *filtered_fmem );
long m4vsd_get_TRd( long *context );
long m4vsd_get_vop_time_inc( long *context );
long m4vsd_get_modulo_time_base( long *context );
long m4vsd_init_usrdata( long *context );
long m4vsd_get_usrdata_size( long *context );

unsigned long m4vsd_get_version( void );
void m4vsd_get_time_code( long hour, long min, long sec );
long m4vsd_get_time_resolution(long *context);
long m4vsd_search_I_VOP(void *context, unsigned long max_search_bit);
long m4vsd_search_vop_header( long *context, unsigned char *stream, long search_max );
long m4vsd_set_workarea( unsigned char *workp,
                         unsigned long max_stream_num,
                         unsigned long work_size );
long m4vsd_stream_num( unsigned long stream_ID );

long m4vsd_get_sc_interval( long *context, unsigned char *stream, long search_max,
				long *header_type );
long m4vsd_get_ref_select_code( long *context, long *ref_layer_id );
long m4vsd_set_vol_param( void *context, M4VSDHEAD *m4vsdh, M4VSDVOLPARAM *m4vsdv );
long m4vsd_get_vol_param( void *context, M4VSDVOLPARAM *m4vsdv );
long m4vsd_get_last_frame_stat( void *context, TAVCBD_LAST_FRAME_STATUS *status );
long m4vsd_get_decoded_frame( void *context, long mode );
long m4vsd_get_context_size( long x, long y );
long m4vsd_set_continue_func( void *context, M4VSD_CONTINUE_FUNC func );
void m4vsd_gfid_mode( void *context, int on );
long m4vsd_temporal_init_sequence( void *contextB, void *contextE, long context_size, unsigned long nfmem, 
			  TAVCBD_FMEM fmema[],
			  long wx, long wy,
			  long *dp1_addr, long *dp2_addr );
long m4vsd_temporal_set_stream_pointer( void *contextB, void *contextE,
				       unsigned char *stream_buffB, unsigned char *stream_buffE,
				       unsigned long stream_sizeB, unsigned long stream_sizeE, TAVCBD_DEC_CONTINUE_FUNC func );
long m4vsd_temporal_decode_picture( void *contextB, void *contextE, long max_read_bits );
long m4vsd_temporal_get_last_frame_stat( void *contextB, void *contextE,
					TAVCBD_LAST_FRAME_STATUS *status );
long m4vsd_temporal_get_decoded_frame( void *contextB, void *contextE, long mode );

/*---------------return to *header_type */
#define M4VSD_BITSET(a) (1 << (a))
#define M4VSD_VIDEO_OBJECT_LAYER_S M4VSD_BITSET(0)
#define M4VSD_VIDEO_OBJECT_S M4VSD_BITSET(1)
#define M4VSD_IDR M4VSD_BITSET(2)
#define M4VSD_VISUAL_OBJECT_SEQ_S M4VSD_BITSET(3)
#define M4VSD_VISUAL_OBJECT_SEQ_E M4VSD_BITSET(4)
#define M4VSD_USER_DATA_S M4VSD_BITSET(5)
#define M4VSD_GROUP_OF_VOP_S M4VSD_BITSET(6)
#define M4VSD_VISUAL_OBJECT_S M4VSD_BITSET(7)
#define M4VSD_VOP_S M4VSD_BITSET(8)
#define M4VSD_VIDEOPACKET_S M4VSD_BITSET(9)
#define M4VSD_NONE_HEADER M4VSD_BITSET(10)
#define M4VSD_ENH_LAYER  M4VSD_BITSET(15) /* selected enhancement layer of MPEG-4 temporal-scalability */


#ifdef __cplusplus
}
#endif

#endif
