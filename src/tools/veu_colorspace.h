#ifndef __VEU_COLORSPACE_H__
#define __VEU_COLORSPACE_H__

int sh_veu_open (void);

void sh_veu_close (void);

int sh_veu_rgb565_to_nv12 (unsigned char *rgb565_in,
			unsigned char *y_out,
			unsigned char *c_out,
			unsigned long width, unsigned long height);

#endif /* __VEU_COLORSPACE_H__ */
