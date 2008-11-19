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

/* XXX: extern declarations, why are these not in a public header for those libs? */
extern void m4iph_sleep_time_init(void);
extern unsigned long m4iph_sleep_time_get(void);
extern int vpu4_clock_on(void);
extern int vpu4_clock_off(void);

/***********************************************************/

/* XXX: Forward declarations, ignore for now */
static int  open_InputStream(void);
static int  open_OutputStream(void);
void UserDisp(__const char *__restrict __format, ...);
static void decode_time_Initialize(void);
static unsigned long decode_Get_time(void);
static int increment_Input(SHCodecs_Decoder * decoder, int len);

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
char	szInput_fileName[MAXPATHLEN];	/* Input file name */
char	szOutput_fileName[MAXPATHLEN];	/* Output file name */
int		iInput_fd;		/* Input file descriptor */
int		iOutput_fd;		/* Output file descriptor */
unsigned char	*pbInput_BufferMemory;	/* Pointer to input buffer */
int		si_ipos;	/* Current position in input stream */
size_t		si_isize;	/* Total size of input data */


/* XXX: random statics, probably local */

/* XXX: local */
static long decode_time;
/* XXX: WTF? this just skips all decode processing, with -S option */
static long performance_flag=0;

/***********************************************************/

/* local output callback, should be static */
static int
local_vpu4_decoded (SHCodecs_Decoder * decoder,
                    unsigned char * y_buf, int y_size,
                    unsigned char * c_buf, int c_size,
                    void * user_data)
{
  ssize_t len;
	if (iOutput_fd != -1) {
		len = write(iOutput_fd, y_buf, y_size);
		write(iOutput_fd, c_buf, c_size);
	} else {
		// vio_render_frame(yf + ry, luma_size);
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
        int pref_len;

	if (argc == 1) {
		UserDisp("argument error!\n");
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
					UserDisp("argument: Unknown video format: %s.\n",optarg);
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
				UserDisp("argument error!\n");
				exit(-2);
		}
	}
	if (w == -1 || h == -1){
		UserDisp("Invalid width and/or height specified.\n");
		exit(-3);
	}
	if ( (strcmp(szIfile, "-") == 0) || (szIfile[0] == '\0') ){
		UserDisp("Invalid input file.\n");
		exit(-4);
	}
	if ( (strcmp(szOfile, "-") == 0) || (szOfile[0] == '\0') ){
		UserDisp("Invalid input file.\n");
		exit(-5);
	}
	if (w < SHCODECS_MIN_FX || w > SHCODECS_MAX_FX || h < SHCODECS_MIN_FY || h > SHCODECS_MAX_FY) {
		UserDisp("Invalid width and/or height specified.\n");
		exit(-6);
	}
	if (optind > argc){
		UserDisp("Too many arguments.\n");
		exit(-7);
	}
	UserDisp("Format: %s\n", iStream_typ == SHCodecs_Format_H264 ? "H.264" : "MPEG4");
	UserDisp("Resolution: %dx%d\n", w, h);
	UserDisp("Input  file: %s\n", szIfile);
	UserDisp("Output file: %s\n", szOfile);
 	stSchePara.sched_priority = sched_get_priority_max(SCHED_FIFO);
	if (sched_setscheduler(0, SCHED_RR, &stSchePara) != 0) {
		perror("sched_setscheduler");
		exit(-8);
	}

   /*
    * Open file descriptors to talk to the VPU and SDR drivers
    *
    */

        /* XXX: init */
        decoder = shcodecs_decoder_init(w, h, iStream_typ);

        /* XXX: local */
        decode_time_Initialize();

        local_init(szIfile, szOfile);

	shcodecs_decoder_set_decoded_callback (decoder, local_vpu4_decoded, NULL);

        /* XXX: decode -- start main loop */
#if 0
	decoder_Start(decoder);
#else
        do {
	  shcodecs_decode (decoder, pbInput_BufferMemory + si_ipos, si_isize - si_ipos);
          pref_len = shcodecs_decoder_preferred_length (decoder);
        } while (increment_Input (decoder, pref_len) == 0);
#endif

        local_close ();

        /* XXX: shutdown */
        shcodecs_decoder_close(decoder);

        /* XXX: local */
	UserDisp("Total decode time = %d(msec)\n",(int)decode_Get_time());
	UserDisp("Total sleep  time = %d(msec)\n",(int)m4iph_sleep_time_get());
	
	return iRtn;
}

static int
local_init (char *pInputfile, char *pOutputfile)
{
	struct timeval tv;
	struct timezone tz;

        /* XXX: local setup */
	strcpy(szInput_fileName, pInputfile);
	strcpy(szOutput_fileName, pOutputfile);

    //- Open input/output stream .
	iInput_fd = iOutput_fd = -1;
	if (pInputfile[0] != '\0' && open_InputStream())
		return -50;
	if (pOutputfile[0] != '\0' && open_OutputStream())
		return -51;

        /* from stream_Initialize() */
    //- Allocate memory for input buffer.
	pbInput_BufferMemory = malloc(INPUT_BUF_SIZE);
	UserDisp("input buffer = %X\n",(int)pbInput_BufferMemory);
	if (pbInput_BufferMemory == NULL) goto err2;

	if (iInput_fd != -1) {
		//- Fill input buffer .
		if ((si_isize = read(iInput_fd, pbInput_BufferMemory, INPUT_BUF_SIZE)) <= 0) {
				perror(szInput_fileName);
				return -54;
		}
	}

	si_ipos = 0;

	gettimeofday(&tv, &tz);
	UserDisp("decode start %ld,%ld\n",tv.tv_sec,tv.tv_usec);

	return 0;
err2:
	return -1;
}
	
	
static int
local_close (void)
{
	struct timeval tv;
	struct timezone tz;

	if (iOutput_fd != -1 && iOutput_fd > 2)
		close(iOutput_fd);
	if (iInput_fd != -1 && iInput_fd > 2)
		close(iInput_fd);

	if (pbInput_BufferMemory){
		free(pbInput_BufferMemory);
		#if 0
		if (decoder->pbNAL_H264BufferMemory)
			free(decoder->pbNAL_H264BufferMemory);
		#endif
	}

	gettimeofday(&tv, &tz);
	UserDisp("%ld,%ld\n",tv.tv_sec,tv.tv_usec);
	return 0;
}
 

/* XXX: local */

/*
 * open_InputStream
 *
 */
static int open_InputStream(void)
{
	iInput_fd = open(szInput_fileName, O_RDONLY);
	if (iInput_fd == -1) {
		perror(szInput_fileName);
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
	iOutput_fd = open(szOutput_fileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (iOutput_fd == -1) {
		perror(szOutput_fileName);
		return -1;
	}
	return 0;
}

/*
 * UserDisp
 *
 */
void UserDisp(__const char *__restrict __format, ...)
{
#ifdef _DEBUG
	printf(__format);
#endif
}

/*
 * decode_time_Initialize
 *
 */
void decode_time_Initialize(void)
{
	decode_time = 0;
}

/*
 * decode_Get_time
 *
 */
unsigned long decode_Get_time(void)
{
	return decode_time;
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
 * increment_Input
 * Increment pointer to the input bitstream after decoding a frame/slice.
 */
static int increment_Input(SHCodecs_Decoder * decoder, int len)
{
	int current_pos = si_ipos + len;
	int rem = si_isize - current_pos;
	int count;

	if (rem<=0) {
                return -1;
        }
	if (rem < INPUT_BUF_SIZE/2 /*&& si_isize == INPUT_BUF_SIZE*/) {
		// printf("Refilling buffer\n");
		// printf("Remaining bytes %d, moving from %d to 0\n", rem, current_pos);
		memmove(pbInput_BufferMemory, pbInput_BufferMemory + current_pos, rem);
		// printf("Reading %d bytes at pos %d\n", si_isize - rem, rem);
		
		do {
			count = read(iInput_fd, pbInput_BufferMemory + rem, INPUT_BUF_SIZE - rem);
			if (count > 0)
				rem += count;
		} while (count > 0 && count < 100);
		si_isize = rem;
		si_ipos = 0;
		// printf("New size of buffer is %d bytes\n", decoder->si_isize);
	} else if ((size_t)current_pos < si_isize) {
		si_ipos = current_pos;
	} else {
		return -1;
	}
	return 0;
}

