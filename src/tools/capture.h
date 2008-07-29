
#ifndef __CAPTURE_H__
#define __CAPTURE_H__

typedef void * sh_ceu;
typedef void (*sh_process_callback)  (const void * frame_data, size_t length, void * user_data);

sh_ceu * sh_ceu_open (const char * device_name, int width, int height);

void sh_ceu_close (sh_ceu * ceu);

void sh_ceu_start_capturing (sh_ceu * ceu);

void sh_ceu_stop_capturing (sh_ceu * ceu);

void sh_ceu_capture_frame (sh_ceu * ceu, sh_process_callback cb, void * user_data);


#endif /* __CAPTURE_H__ */
