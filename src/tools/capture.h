
#ifndef __CAPTURE_H__
#define __CAPTURE_H__

typedef void * sh_veu;
typedef void (*sh_process_callback)  (const void * frame_data, size_t length, void * user_data);

sh_veu * sh_veu_open (const char * device_name, int width, int height);

void sh_veu_close (sh_veu * veu);

void sh_veu_start_capturing (sh_veu * veu);

void sh_veu_stop_capturing (sh_veu * veu);

void sh_veu_capture_frame (sh_veu * veu, sh_process_callback cb, void * user_data);


#endif /* __CAPTURE_H__ */
