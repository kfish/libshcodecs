#include <stdio.h>
#include <string.h>

#include <shcodecs/shcodecs_decoder.h>

#define BUF_SIZE (4*256*1024)

static int vpu4_decoded (SHCodecs_Decoder * decoder,
                         unsigned char * y_buf, int y_size,
                         unsigned char * c_buf, int c_size,
                         void * user_data)
{
        fwrite(y_buf, 1, y_size, stdout);
        fwrite(c_buf, 1, c_size, stdout);

        return 0;
}

int main (int argc, char *argv[])
{
         SHCodecs_Decoder * decoder;
         unsigned char buf[BUF_SIZE];
         size_t n;
	 int nused;

         decoder = shcodecs_decoder_init (320, 240, SHCodecs_Format_MPEG4);
         shcodecs_decoder_set_decoded_callback (decoder, vpu4_decoded, NULL);

         while ((n = fread (buf, 1, BUF_SIZE, stdin)) > 0) {
                 if ((nused = shcodecs_decode (decoder, buf, n)) < 0)
                         break;
                 memmove (buf, &buf[nused], BUF_SIZE-nused);
         }

         shcodecs_decoder_finalize (decoder);
         shcodecs_decoder_close (decoder);

	 return 0;
}
