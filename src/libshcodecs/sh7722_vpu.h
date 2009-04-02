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

/*****************************************************************************
*                                                                            *
*  sh7722_vpu.h                                                              *
*                                                                            *
*****************************************************************************/

/**************************/
/* VPU registers          */
/**************************/
#define	VP4_CTRL		0xFE900000
#define	VP4_VOL_CTRL		0xFE900004
#define	VP4_IMAGE_SIZE		0xFE900008
#define	VP4_MB_NUM		0xFE90000C
#define	VP4_DWY_ADDR		0xFE900010
#define	VP4_DWC_ADDR		0xFE900014
#define	VP4_D2WY_ADDR		0xFE900018
#define	VP4_D2WC_ADDR		0xFE90001C
#define	VP4_DP1_ADDR		0xFE900020
#define	VP4_DP2_ADDR		0xFE900024
#define	VP4_STRS_ADDR		0xFE900028
#define	VP4_STRE_ADDR		0xFE90002C
#define	VP4_VOP_CTRL		0xFE900030
#define	VP4_VOP_TIME		0xFE900034
#define	VP4_263_CTRL		0xFE900038
#define	VP4_264_CTRL		0xFE90003C
#define	VP4_VLC_CTRL		0xFE900040
#define	VP4_ENDIAN		0xFE900044
#define	VP4_CMD			0xFE900048
#define	VP4_ME_TH1		0xFE90004C
#define	VP4_ME_TH2		0xFE900050
#define	VP4_ME_CTRL		0xFE90005C
#define	VP4_MBRF_CTRL		0xFE900060
#define	VP4_MC_CTRL		0xFE900064
#define	VP4_PRED_CTRL		0xFE900068
#define	VP4_SLC_SIZE		0xFE90006C
#define	VP4_VOP_MINBIT		0xFE900070
#define	VP4_MB_MAXBIT		0xFE900074
#define	VP4_MB_TBIT		0xFE900078
#define	VP4_RCQNT		0xFE90007C
#define	VP4_RCRP		0xFE900080
#define	VP4_RCDJ		0xFE900084
#define	VP4_RCWQ		0xFE900088
#define	VP4_EC_REF		0xFE9000A4
#define	VP4_STATUS		0xFE900100
#define	VP4_IRQ_ENB		0xFE900104
#define	VP4_IRQ_STA		0xFE900108
#define	VP4_VOP_BIT		0xFE90010C
#define	VP4_SLC_MB		0xFE900114
#define	VP4_QSUM		0xFE900118
#define	VP4_DEC_ERR		0xFE90011C
#define	VP4_ERR_AREA		0xFE900120
#define	VP4_NEXT_CODE		0xFE900124
#define	VP4_MB_ATTR		0xFE900128
#define	VP4_ERR_DET		0xFE900134
#define	VP4_CLK_STOP		0xFE900138
#define	VP4_MB_SADR		0xFE900140
#define	VP4_MAT_RAM		0xFE901000
#define	VP4_NC_RAM		0xFE902000
#define	WT			0xFE9020CC
#define	VP4_CPY_ADDR		0xFE902264
#define	VP4_CPC_ADDR		0xFE902268
#define	VP4_R0Y_ADDR		0xFE90226C
#define	VP4_R0C_ADDR		0xFE902270
#define	VP4_R1Y_ADDR		0xFE902274
#define	VP4_R1C_ADDR		0xFE902278
#define	VP4_R2Y_ADDR		0xFE90227C
#define	VP4_R2C_ADDR		0xFE902280
#define	VP4_R3Y_ADDR		0xFE902284
#define	VP4_R3C_ADDR		0xFE902288
#define	VP4_R4Y_ADDR		0xFE90228C
#define	VP4_R4C_ADDR		0xFE902290
#define	VP4_R5Y_ADDR		0xFE902294
#define	VP4_R5C_ADDR		0xFE902298
#define	VP4_R6Y_ADDR		0xFE90229C
#define	VP4_R6C_ADDR		0xFE9022A0
#define	VP4_R7Y_ADDR		0xFE9022A4
#define	VP4_R7C_ADDR		0xFE9022A8
#define	VP4_R8Y_ADDR		0xFE9022AC
#define	VP4_R8C_ADDR		0xFE9022B0
#define	VP4_R9Y_ADDR		0xFE9022B4
#define	VP4_R9C_ADDR		0xFE9022B8
#define	VP4_RAY_ADDR		0xFE9022BC
#define	VP4_RAC_ADDR		0xFE9022C0
#define	VP4_RBY_ADDR		0xFE9022C4
#define	VP4_RBC_ADDR		0xFE9022C8
#define	VP4_RCY_ADDR		0xFE9022CC
#define	VP4_RCC_ADDR		0xFE9022D0
#define	VP4_RDY_ADDR		0xFE9022D4
#define	VP4_RDC_ADDR		0xFE9022D8
#define	VP4_REY_ADDR		0xFE9022DC
#define	VP4_REC_ADDR		0xFE9022E0
#define	VP4_RFY_ADDR		0xFE9022E4
#define	VP4_RFC_ADDR		0xFE9022E8

/***********************************/
/* VPU (Video Processing Unit) IPR */
/***********************************/
#define VPU_BASE		0xfe900000
#define VP4_IRQ_STA_EE		0x00000002	/* Err processing completion */
#define VP4_IRQ_STA_EDT		0x00000100	/* Error DeTect */

/**************************/
/*    LOW POWER MODE      */
/**************************/
#define	STBCR			0xA4150020
#define	MSTPCR0			0xA4150030
#define	MSTPCR1			0xA4150034
#define	MSTPCR2			0xA4150038
#define	BAR			0xA4150040

/*****************************************************************************/
/* ioctl command define                                                      */
/*****************************************************************************/
#define VPU4CTRL_SLEEPON 	0x00000100
#define VPU4CTRL_CLKON 		0x00000101
#define VPU4CTRL_CLKOFF 	0x00000102
#define VPU4CTRL_GET_SDR_BASE	0x00000103
