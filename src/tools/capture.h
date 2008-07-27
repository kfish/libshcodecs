
#ifndef __CAPTURE_H__
#define __CAPTURE_H__

void open_device                     (char * device_name);

void init_device                     (void);

void uninit_device                   (void);

void close_device                    (void);

void start_capturing                 (void);

void stop_capturing                  (void);

void mainloop                        (void);


#endif /* __CAPTURE_H__ */
