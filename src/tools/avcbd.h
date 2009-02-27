/*
 *  H.264 analyzer program
 *  Copyright (C) Renesas Technology Corp., 2003-2004. All rights reserved.
 *
 *  $Date: 2006/08/09 14:07:47 $
 */
/**\file avcbd.h
 *\brief Prototype declarations of the API functions.
 */

#ifndef _AVCBD_H_
#define _AVCBD_H_

#ifdef __cplusplus
extern "C" {
#endif

#define AVCBD_MAX_FRAME_NUM (32)

#define AVCBD_TYPE_MPEG4 (0)
#define AVCBD_TYPE_AVC   (1)

#define AVCBD_UNIT_PIC    (0)
#define AVCBD_UNIT_NAL    (1)
#define AVCBD_UNIT_NO_ANNEX_B (2)

#define AVCBD_CNCL_REF_TYPE1  (0)
#define AVCBD_CNCL_REF_TYPE2  (1)
#define AVCBD_CNCL_REF_TYPE3  (2)

#define AVCBD_FILTER_OFF  (0)
#define AVCBD_FILTER_DBL  (1)
#define AVCBD_FILTER_LPL  (2)
#define AVCBD_FILTER_LPH  (3)
#define AVCBD_POST        (0)
#define AVCBD_CONCEAL     (1)

#define AVCBD_NAL_UNSPECIFIED   (0)
#define AVCBD_NAL_NON_IDR_PIC   (1)
#define AVCBD_NAL_SLICE_DP_A    (2)
#define AVCBD_NAL_SLICE_DP_B    (3)
#define AVCBD_NAL_SLICE_DP_C    (4)
#define AVCBD_NAL_IDR_PIC       (5)
#define AVCBD_NAL_SEI           (6)
#define AVCBD_NAL_SPS           (7)
#define AVCBD_NAL_PPS           (8)
#define AVCBD_NAL_AUD           (9)
#define AVCBD_NAL_END_SEQ       (10)
#define AVCBD_NAL_END_STREAM    (11)
#define AVCBD_NAL_FILLER        (12)
#define AVCBD_NAL_RESERVED      (13)

/* Error Code */
#define AVCBD_PARAM_ERROR       (-1)
#define AVCBD_PIC_NOERROR_I     (0)
#define AVCBD_PIC_NOERROR_P     (1)
#define AVCBD_PIC_NOERROR_B     (2)
#define AVCBD_PIC_NOERROR_NOVCL (3)
#define AVCBD_PIC_NOTCODED_VOP  (4)
#define AVCBD_PIC_TIMEOUT       (-2)
#define AVCBD_PIC_ABORT         (-3)
#define AVCBD_PIC_EOS           (-4)
#define AVCBD_PIC_LARGE         (-5)
#define AVCBD_PIC_FEWERMB       (-6)
#define AVCBD_PIC_FMTERROR      (-8)
#define AVCBD_PIC_ERROR         (-9)
#define AVCBD_MB_OVERRUN        (-10)
#define AVCBD_ILLEGAL_DATA      (-14)
#define AVCBD_ILLEGAL_CODE      (-15)
#define AVCBD_ILLEGAL_MTYPE     (-16)
#define AVCBD_BUFFER_OVERRUN    (-19)
#define AVCBD_PIC_EXCESS_FRAME  (-20)
#define AVCBD_PIC_EXCESS_PPS    (-21)
#define AVCBD_TEMPORARY_BUFFER_SHORT_ERROR (-50)
#define AVCBD_VPU_ERROR         (-40)

/* Error Code for VLD */
#define AVCBD_VLD_ERROR_CODE                0x00001000
#define AVCBD_COEFTOKEN_ZERO_LENGTH         (AVCBD_VLD_ERROR_CODE | 0x01)
#define AVCBD_COEFTOKEN_TOKENINFO_OVER      (AVCBD_VLD_ERROR_CODE | 0x02)
#define AVCBD_COEFTOKEN_TOKENTBL_OVER       (AVCBD_VLD_ERROR_CODE | 0x03)
#define AVCBD_TOTALZEROS_PARAM_ERROR        (AVCBD_VLD_ERROR_CODE | 0x11)
#define AVCBD_TOTALZEROS_ZEROLENGTH8        (AVCBD_VLD_ERROR_CODE | 0x12)
#define AVCBD_TOTALZEROS_ZEROLEFT           (AVCBD_VLD_ERROR_CODE | 0x13)
#define AVCBD_TOTALZEROS_ZEROLENGTH         (AVCBD_VLD_ERROR_CODE | 0x14)
#define AVCBD_EXPGOLOMB_MAP4_ERROR          (AVCBD_VLD_ERROR_CODE | 0x21)
#define AVCBD_EXPGOLOMB_MAP_ERROR           (AVCBD_VLD_ERROR_CODE | 0x22)

/* detect parameter */
#define AVCBD_BIT(a) (1 << (a))
#define AVCBD_SPS         AVCBD_BIT(0)	/* seq_parameter_set_rbsp() detect. */
#define AVCBD_PPS         AVCBD_BIT(1)	/* pic_parameter_set_rbsp() detect. */
#define AVCBD_IDR         AVCBD_BIT(2)	/* IDR picture detect. */
#define AVCBD_VUI         AVCBD_BIT(3)	/* vui_parameter() detect. */
#define AVCBD_SEI         AVCBD_BIT(4)	/* sei_rbsp() detect. */
#define AVCBD_MPEG4_USER  AVCBD_BIT(5)	/* user_data() detect. */
#define AVCBD_END_SEQ     AVCBD_BIT(5)	/* end_of_seq_rbsp() detect. next shall be IDR. */
#define AVCBD_END_STREAM  AVCBD_BIT(6)	/* end_of_stream_rbsp() detect. decoding will be terminated. */
#define AVCBD_FILLER      AVCBD_BIT(7)	/* filler_data_rbsp() detect. */
#define AVCBD_VCL         AVCBD_BIT(8)	/* slice data detect. */
#define AVCBD_DELIMITER   AVCBD_BIT(9)	/* access_unit_delimiter_rbsp() detect. */
#define AVCBD_BUFFERING_PERIOD_SEI                AVCBD_BIT(10)
#define AVCBD_PIC_TIMING_SEI                      AVCBD_BIT(11)
#define AVCBD_PAN_SCAN_RECT                       AVCBD_BIT(12)
#define AVCBD_RECOVERY_POINT_SEI                  AVCBD_BIT(13)
#define AVCBD_DEC_REF_PIC_MARKING_REPETITION_SEI  AVCBD_BIT(14)
#define AVCBD_MPEG4_ENH   AVCBD_BIT(15)	/* selected enhancement layer of MPEG-4 temporal-scalability */

/** Structure of the pointer to a pair of frame memories.
 */
	typedef struct {
		unsigned char *Y_fmemp;
				/**< Pointer to the luma frame */
		unsigned char *C_fmemp;
				/**< Pointer to the chroma frame */
	} TAVCBD_FMEM;

/** Structure to store informations about size of frame in pixel unit.
 */
	typedef struct {
		unsigned long width;
		unsigned long height;
		unsigned long crop_offset[4];	/* cropping offset in two pixels unit. order: left, right, top, bottom. */
	} TAVCBD_FRAME_SIZE;

/** Structure to store informations about previously decoded picture.
 */
	typedef struct {
		long error_num;
			/**< error ID code */
		unsigned long error_pos;
				 /**< bit position where error was found */
		unsigned long detect_param;	/* flags to indicate detection of pic_param, seq_param, etc. */
		unsigned long read_slices;
				   /**< number of slice(s) that's been processed */
		unsigned long read_bits;
				 /**< number of bits that's been processed */
		long last_macroblock_pos;
				  /**< last decoded MB position  */
		unsigned long frame_num;
				 /**< frame_num in slice_header() */
		unsigned long poc_top;
			       /**< POC (PictureOrderCnt) */
		unsigned long poc_bottom;
				  /**< reserved (BottomFieldCnt) */
		unsigned long primary_pic_type;
					/**< valid if access_unit_delimiter() is present */
	} TAVCBD_LAST_FRAME_STATUS;


	typedef struct {
	/** Visual Object Sequence & Video Object **/
		long profile_and_level_indication;	/*(8) */
		long is_visual_object_identifier;	/*(1) */
		long visual_object_verid;	/*(4) */
		long visual_object_priority;	/*(3) */
		long visual_object_type;	/*(4) */

	/** Video Object Layer **/
		long random_accessible_vol;	/*(1) */
		long video_object_type_indication;	/*(8) */
		long is_object_layer_identifier;	/*(1) */
		long video_object_layer_verid;	/*(4) */
		long video_object_layer_priority;	/*(3) */
		long aspect_ratio_info;	/*(1) */
		long par_width;	/*(8) */
		long par_height;	/*(8) */
		long vol_control_parameters;	/*(1) */
		long chroma_format;	/*(2) */
		long low_delay;	/*(1) */
		long vbv_parameters;	/*(1) */
		long first_half_bit_rate;	/*(15) */
		long latter_half_bit_rate;	/*(15) */
		long first_half_vbv_buffer_size;	/*(15) */
		long latter_half_vbv_buffer_size;	/*(3) */
		long first_half_vbv_occupancy;	/*(11) */
		long latter_half_vbv_occupancy;	/*(15) */

		long video_object_layer_shape;	/*(2) */
		long vop_time_increment_resolution;	/*(16) */
		long fixed_vop_rate;	/*(1) */
		long fixed_vop_time_increment;	/*(1-16) */

		long video_object_layer_width;	/*(13) */
		long video_object_layer_height;	/*(13) */

		long resync_marker_disable;	/*(1) */
		long data_partitioned;	/*(1) */
		long reversible_vlc;	/*(1) */

	} AVCBD_MPEG4_HEAD;

	typedef struct {
		unsigned char pic_width_in_mbs;
					/**< width */
		unsigned char pic_height_in_map_units;
					       /**< height */
		short pic_width;
			 /**< width in pixel unit */
		short pic_height;
			  /**< height in pixel unit */
		short pic_size_in_mbs;
			       /**< number of mbs in one frame */
	} AVCBD_PIC_SIZE;

#define AVCBD_MAX_POC_CYCLE (32)

/** Sequence parameter set.
 */
	typedef struct {
		unsigned char profile_idc;
				   /**< Baseline maybe 66 */
		unsigned char constraint_set_flag;
					   /**< mixture of set[0-2] + 5bit */
		unsigned char level_idc;
				 /**< Level information */
		short seq_parameter_set_id;
				    /**< In the range of [0,31] */
		unsigned short log2_max_frame_num;
					   /**< modified from log2(x)-4 [4,16] */
		unsigned char pic_order_cnt_type;
					  /**< In the range of [0,2] */

		unsigned short log2_max_pic_order_cnt_lsb;
						   /**< modified from log2(x)-4 [4,16]*/
		unsigned char delta_pic_order_always_zero_flag;
							/**< one bit */
		signed long offset_for_non_ref_pic;
					    /**< signed 32bit value */
		signed long offset_for_top_to_bottom_field;
						    /**< signed 32bit value */
		unsigned char num_ref_frames_in_pic_order_cnt_cycle;
							     /**< [0,255] */
		signed short
		    delta_offset_for_ref_frame[AVCBD_MAX_POC_CYCLE];
									/**< sum of signed 32bit value */
		unsigned char num_ref_frames;
				      /**< total number of ref frame. [0,16] */
		unsigned char gaps_in_frame_num_value_allowed_flag;
							    /**< one bit */
		AVCBD_PIC_SIZE pic_size;
		unsigned char frame_mbs_only_flag;
					   /**< multiplexed with mb_adaptive_frame_field_flag */
		unsigned char direct_8x8_inferrence_frame_cropping_flag;
								 /**< multiplexed flags */
		struct {
			short left_offset;
			short right_offset;
			short top_offset;
			short bottom_offset;
		} frame_crop;
		unsigned char vui_parameters_present_flag;
		/* NAL unit */
		unsigned char nal_ref_idc;
		unsigned char nal_unit_type;
				     /**< nal_unit_type of current NAL */
		unsigned short curr_slice_id;
				      /**< slice # in current picture */
	} AVCBD_SEQ_PARAMETER_SET;

/** Picture parameter set.
 */
	typedef struct {
		short pic_parameter_set_id;
				    /**< [0,255] */
		unsigned char seq_parameter_set_id;
					    /**< [0,31] */
		unsigned char entropy_coding_mode_flag;
						/**< one bit */
		unsigned char pic_order_present_flag;
					      /**< one bit */
		unsigned char num_slice_groups;
					/**< [1,8] in Baselile profile */
		unsigned char slice_group_map_type;
					    /**< [0,6] */
		unsigned char slice_group_change_direction_flag;
							 /**< one bit */
		unsigned short run_length[8];
					/**< [1,maxMBnum] for each elements */
		struct {
			unsigned short top_left;
					 /**< less than bottom_right */
			unsigned short bottom_right;
					     /**< less than maxMBnum */
		} rectangle[8];
		unsigned short slice_group_change_rate;
						/**< [1,maxMBnum] */
		unsigned short pic_size_in_map_units;
					      /**< equal to maxMBnum */
		unsigned char slice_group_id[8];
					   /**< [0,num_slice_groups-1] for each elements */
		unsigned char num_ref_idx_l0_active;
					     /**< [1,32] */
		unsigned char num_ref_idx_l1_active;
					     /**< [1,32] */
		unsigned char weighted_pred_flag_bipred_idc;
						     /**< shall be zero */
		unsigned char pic_init_qp;
				   /**< [0,31] */
		unsigned char pic_init_qs;
				   /**< [0,31] */
		signed char chroma_qp_index_offset;
					    /**< [-12,12]*/
		unsigned char deblocking_filter_control_present_flag;
							      /**< one bit */
		unsigned char constrained_intra_pred_flag;
						   /**< one bit */
		unsigned char redundant_pic_cnt_present_flag;
						      /**< one bit */
	} AVCBD_PIC_PARAMETER_SET;

	typedef struct {
		AVCBD_SEQ_PARAMETER_SET seq_param;
		AVCBD_PIC_PARAMETER_SET pic_param;
	} AVCBD_H264_HEAD;


	typedef long (*TAVCBD_FUNC) (void *);
	typedef long (*TAVCBD_DEC_CONTINUE_FUNC) (const void *context,
						  char **next_stream_buff,
						  unsigned long
						  *stream_buff_size);

	unsigned long avcbd_get_version(void);
	long avcbd_get_workarea_size(int stream_type, long x, long y,
				     long pic_param_num);
	long avcbd_get_context_size(long wx, long wy, long pic_param_num);
	long avcbd_start_decoding(void);
	long avcbd_init_sequence(void *workarea, long workarea_size,
				 unsigned long nfmem, TAVCBD_FMEM fmema[],
				 long wx, long wy, long pic_param_num,
				 long *dp1_addr, long *dp2_addr,
				 long stream_type, void **context);
	long avcbd_set_stream_pointer(void *context,
				      unsigned char *stream_buff,
				      unsigned long stream_size,
				      TAVCBD_DEC_CONTINUE_FUNC func);
	long avcbd_set_decode_mode(void *context, long decode_unit_type);
	long avcbd_get_last_frame_stat(void *context,
				       TAVCBD_LAST_FRAME_STATUS * status);
	long avcbd_get_frame_size(void *context,
				  TAVCBD_FRAME_SIZE * frame_size);
	long avcbd_decode_picture(void *context, long max_read_bits);
	long avcbd_get_decoded_frame(void *context, long mode);
	long avcbd_set_resume_err(void *context, long err_resume_flg,
				  long err_conceal_mode);

/* temporal scalability */
	long avcbd_temporal_init_sequence(void *contextB, void *contextE,
					  long context_size,
					  unsigned long nfmem,
					  TAVCBD_FMEM fmema[], long wx,
					  long wy, long *dp1_addr,
					  long *dp2_addr);
	long avcbd_temporal_set_stream_pointer(void *contextB,
					       void *contextE,
					       unsigned char *stream_buffB,
					       unsigned char *stream_buffE,
					       unsigned long stream_sizeB,
					       unsigned long stream_sizeE,
					       TAVCBD_DEC_CONTINUE_FUNC
					       func);
	long avcbd_temporal_decode_picture(void *contextB, void *contextE,
					   long max_read_bits);
	long avcbd_temporal_get_last_frame_stat(void *contextB,
						void *contextE,
						TAVCBD_LAST_FRAME_STATUS *
						status);
	long avcbd_temporal_get_decoded_frame(void *contextB,
					      void *contextE, long mode);


/* supplimental functions */
	long avcbd_search_start_code(unsigned char *, long, unsigned long);
	long avcbd_extract_nal(void *pSrc, void *pDst, long iSize,
			       unsigned long iMode);
	long avcbd_emulation_prevention_byte_skip(void *context, long on);

	void avcbd_get_time_code(long hour, long min, long sec);
	long avcbd_search_vop_header(void *context, unsigned char *stream,
				     long search_max);
	long avcbd_get_sc_interval(void *context, unsigned char *stream,
				   long search_max, long *header_type);
	long avcbd_get_header(void *context, void *header);
	long avcbd_set_seq_header(void *context, const void *header);
	long avcbd_get_time_resolution(void *context);
	long avcbd_get_TRd(void *context);
	long avcbd_get_vop_time_inc(void *context);
	long avcbd_get_modulo_time_base(void *context);
	long avcbd_init_usrdata(void *context);
	long avcbd_get_usrdata_size(void *context);
	long avcbd_get_ref_select_code(void *context, long *ref_layer_id);
	void avcbd_gfid_mode(void *context, int on);
	long avcbd_set_filter_mode(void *context, long filter_mode,
				   long filter_select,
				   TAVCBD_FMEM * filtered_fmem);
	long avcbd_sc_parse(void *context, long sc_timeout_nbit,
			    long vol_autolock, long vol_parse_only);

#ifdef __cplusplus
}
#endif
#endif				/* _AVCBD_H_ */
