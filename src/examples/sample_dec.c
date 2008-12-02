/*

  Sample Program Code.
  Copyright (C) Renesas Technology Corp., 2007. All rights reserved.

 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sched.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/time.h>

#include <shcodecs/shcodecs_decoder.h>

#define INPUT_BUF_SIZE	(256 * 1024)

/* #define _DEBUG */

/* XXX: extern declarations, why are these not in a public header for those libs? */
extern unsigned long m4iph_sleep_time_get(void);

/***********************************************************/

/* Forward declarations */
static int  open_InputStream(void);
static int  open_OutputStream(void);
static int update_input(SHCodecs_Decoder * decoder, int len);

static int local_init (char *pInputfile, char *pOutputfile);
static int local_close (void);


/* XXX: public function, what is this? */
long m4iph_enc_continue(long output_bits);

/* XXX: global, what is this? not even used, why is this here? */
void *global_context;

/* XXX: local stuff */
static struct option stLong_options[] = {
        { "format", 1, 0, 'f'},
        { "output", 1, 0, 'o'},
        { "input" , 1, 0, 'i'},
        { "width" , 1, 0, 'w'},
        { "height", 1, 0, 'h'},
	    { "perfor", 1, 0, 'S'}
};

/* XXX: sample-dec local vars, removed from ST_STREAM_INFO */
char	input_filename[MAXPATHLEN];	/* Input file name */
char	output_filename[MAXPATHLEN];	/* Output file name */
int		input_fd;		/* Input file descriptor */
int		output_fd;		/* Output file descriptor */
unsigned char	*input_buffer;	/* Pointer to input buffer */
int		si_ipos;	/* Current position in input stream */
size_t		si_isize;	/* Total size of input data */


/* XXX: random statics, probably local */

/* XXX: WTF? this just skips all decode processing, with -S option */
static long performance_flag=0;

/*
 * debug_printf
 *
 */
void debug_printf(__const char *__restrict __format, ...)
{
#ifdef _DEBUG
	printf(__format);
#endif
}

/***********************************************************/

/* local output callback */
static int
local_vpu4_decoded (SHCodecs_Decoder * decoder,
                    unsigned char * y_buf, int y_size,
                    unsigned char * c_buf, int c_size,
                    void * user_data)
{
	ssize_t len;

	if (output_fd != -1) {
		len = write(output_fd, y_buf, y_size);
		write(output_fd, c_buf, c_size);
	}

        return 0;
}

/***********************************************************/

/* XXX: local (main) */
int main(int argc, char **argv)
{
        SHCodecs_Decoder * decoder;
	int iRtn=0, iStream_typ = SHCodecs_Format_H264, iIdx, w, h;
	char c, szIfile[MAXPATHLEN], szOfile[MAXPATHLEN];
	struct sched_param stSchePara;
        int bytes_decoded;

	if (argc == 1) {
		debug_printf("argument error!\n");
		exit(0);
	}
	setvbuf(stdout, NULL, _IONBF, 0);
	szOfile[0] = szIfile[0] = '\0';
	memset(szIfile, 0, sizeof(szIfile));
	memset(szOfile, 0, sizeof(szOfile));
	w = h = -1;
	while (1) {
		c = getopt_long(argc, argv, "f:o:i:w:h:S", stLong_options, &iIdx);
		if (c == -1)
			break;

		switch (c) {
		case 'f':
			if (strncmp(optarg, "mpeg4", 5) == 0)
				iStream_typ = SHCodecs_Format_MPEG4;
			else if (strncmp(optarg, "h264", 4) == 0)
				iStream_typ = SHCodecs_Format_H264;
			else{
				/*int ilen = strlen(optarg);*/
				/*printf("optarg len = %d \n", optarg);*/
				debug_printf("argument: Unknown video format: %s.\n",optarg);
				exit(-1);
			}
			break;
		case 'o':
			if (optarg)
				strncpy(szOfile, optarg, sizeof(szOfile) - 1);
			break;
		case 'i':
			if (optarg)
				strncpy(szIfile, optarg, sizeof(szIfile) - 1);
			break;
		case 'w':
			if (optarg)
				w = strtoul(optarg, NULL, 10);
			break;
		case 'h':
			if (optarg)
				h = strtoul(optarg, NULL, 10);
			break;
		case 'S':
			performance_flag=1;
			break;
		default:
			debug_printf("argument error!\n");
			exit(-2);
		}
	}
	if (w == -1 || h == -1){
		debug_printf("Invalid width and/or height specified.\n");
		exit(-3);
	}
	if ( (strcmp(szIfile, "-") == 0) || (szIfile[0] == '\0') ){
		debug_printf("Invalid input file.\n");
		exit(-4);
	}
	if ( (strcmp(szOfile, "-") == 0) || (szOfile[0] == '\0') ){
		debug_printf("Invalid input file.\n");
		exit(-5);
	}
	if (w < SHCODECS_MIN_FX || w > SHCODECS_MAX_FX || h < SHCODECS_MIN_FY || h > SHCODECS_MAX_FY) {
		debug_printf("Invalid width and/or height specified.\n");
		exit(-6);
	}
	if (optind > argc){
		debug_printf("Too many arguments.\n");
		exit(-7);
	}
	debug_printf("Format: %s\n", iStream_typ == SHCodecs_Format_H264 ? "H.264" : "MPEG4");
	debug_printf("Resolution: %dx%d\n", w, h);
	debug_printf("Input  file: %s\n", szIfile);
	debug_printf("Output file: %s\n", szOfile);
 	stSchePara.sched_priority = sched_get_priority_max(SCHED_FIFO);
	if (sched_setscheduler(0, SCHED_RR, &stSchePara) != 0) {
		perror("sched_setscheduler");
		exit(-8);
	}

	/* Open file descriptors to talk to the VPU and SDR drivers */

        if ((decoder = shcodecs_decoder_init(w, h, iStream_typ)) == NULL) {
                exit (-9);
        }

        local_init(szIfile, szOfile);

	shcodecs_decoder_set_decoded_callback (decoder, local_vpu4_decoded, NULL);

        /* decode main loop */
        do {
		debug_printf ("Calling shcodecs_decode (si_ipos %d, si_isize %d) ...", si_ipos, si_isize);
		bytes_decoded = shcodecs_decode (decoder, input_buffer + si_ipos, si_isize - si_ipos);
		debug_printf (" decoded %d bytes\n", bytes_decoded);
        } while (bytes_decoded > 0 && update_input (decoder, bytes_decoded) == 0);

        local_close ();

        shcodecs_decoder_close(decoder);

	debug_printf("Total sleep  time = %d(msec)\n",(int)m4iph_sleep_time_get());
	
	return iRtn;
}

static int
local_init (char *pInputfile, char *pOutputfile)
{
	struct timeval tv;
	struct timezone tz;

        /* XXX: local setup */
	strcpy(input_filename, pInputfile);
	strcpy(output_filename, pOutputfile);

	/* Open input/output stream */
	input_fd = output_fd = -1;
	if (pInputfile[0] != '\0' && open_InputStream())
		return -50;
	if (pOutputfile[0] != '\0' && open_OutputStream())
		return -51;

	/* Allocate memory for input buffer */
	input_buffer = malloc(INPUT_BUF_SIZE);
	debug_printf("input buffer = %X\n",(int)input_buffer);
	if (input_buffer == NULL) goto err2;

	if (input_fd != -1) {
		/* Fill input buffer */
		if ((si_isize = read(input_fd, input_buffer, INPUT_BUF_SIZE)) <= 0) {
				perror(input_filename);
				return -54;
		}
	}

	si_ipos = 0;

	gettimeofday(&tv, &tz);
	debug_printf("decode start %ld,%ld\n",tv.tv_sec,tv.tv_usec);

	return 0;
err2:
	return -1;
}
	
	
static int
local_close (void)
{
	struct timeval tv;
	struct timezone tz;

	if (output_fd != -1 && output_fd > 2)
		close(output_fd);
	if (input_fd != -1 && input_fd > 2)
		close(input_fd);

	if (input_buffer)
		free(input_buffer);

	gettimeofday(&tv, &tz);
	debug_printf("%ld,%ld\n",tv.tv_sec,tv.tv_usec);
	return 0;
}
 

/*
 * open_InputStream
 *
 */
static int open_InputStream(void)
{
	input_fd = open(input_filename, O_RDONLY);
	if (input_fd == -1) {
		perror(input_filename);
		return -1;
	}
	return 0;
}
 
/*
 * open_OutputStream
 *
 */
static int open_OutputStream(void)
{
	output_fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output_fd == -1) {
		perror(output_filename);
		return -1;
	}
	return 0;
}

/*
 * m4iph_enc_continue
 *
 */
long m4iph_enc_continue(long output_bits)
{
	return 1;
}

/*
 * update_input
 * Increment pointer to the input bitstream after decoding a frame/slice.
 */
static int update_input(SHCodecs_Decoder * decoder, int len)
{
	int current_pos = si_ipos + len;
	int rem = si_isize - current_pos;
	int count;

	if (rem<=0) {
                return -1;
        }
	if (rem < INPUT_BUF_SIZE/2 /* && si_isize == INPUT_BUF_SIZE */) {
		debug_printf ("Refilling buffer: ");
		debug_printf ("Remaining bytes %d, moving from %d to 0\n", rem, current_pos);
		memmove(input_buffer, input_buffer + current_pos, rem);
		debug_printf ("Reading %d bytes at pos %d\n", si_isize - rem, rem);
		si_ipos = 0;
		
		do {
			count = read(input_fd, input_buffer + rem, INPUT_BUF_SIZE - rem);
			if (count > 0)
				rem += count;
		} while (count > 0 && count < 100);
		si_isize = rem;
		debug_printf ("New size of buffer is %d bytes\n", si_isize);
	} else if ((size_t)current_pos < si_isize) {
		si_ipos = current_pos;
	} else {
		return -1;
	}

	return 0;
}

