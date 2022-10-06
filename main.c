//PRESS RUN AND TYPE ./main 440.wav
#include <stdint.h>
#include <stdio.h> //read and write files
#include <string.h>
#include <math.h>

typedef int8_t fourcc[4];
//change output format here

typedef int16_t sample_t;
#define SAMPLE_MAX 32767

// Wav file struct

struct riff_hdr {
  fourcc   id;
  uint32_t size;
  fourcc   type;
};

struct fmt_ck {
  fourcc   id;
  uint32_t size;
  uint16_t fmt_tag;
  uint16_t channels;
  uint32_t samples_per_sec;
  uint32_t bytes_per_sec;
  uint16_t block_align;
  uint16_t bits_per_sample;
};

struct data_hdr {
  fourcc   id;
  uint32_t size;
};

struct wav_hdr {
  struct riff_hdr riff;
  struct fmt_ck   fmt;
  struct data_hdr data;
};

#define DURATION 6
#define SAMPLE_RATE 44100
#define NCHANNELS 1
#define NSAMPLES (DURATION*SAMPLE_RATE*NCHANNELS)
/*hard coding the sample 50ms or 0.05sec div by 0.002 based on hz 400.
 In other words 25 periods can fit in 50ms if hz = 400. 
 Since there are about 110 data points in 1 period.
 Therefore 110 data points x 25 periods is 2750 data points for 50ms*/
static sample_t buf[2750];
FILE *fp;
char *filename;
struct wav_hdr hdr = {0};

int main (int argc, char **argv){
  //read in file name from command line
  filename = (char*) malloc(sizeof(char) * 1024);
  if (filename == NULL) {
    printf("Error in malloc\n");
    exit(1);
  }
  char cwd[1024];
  if(getcwd(cwd, sizeof(cwd)) != NULL) {
    strcpy(filename, cwd);
    if(argc < 2) {
      printf("No wave file specified\n");
      return 0;
    }
    strcat(filename, "/");
    strcat(filename, argv[1]);
    printf("%s\n", filename);
  }
  //open file
  fp = fopen(filename, "rb");
  if (fp == NULL) {
    printf("Error opening file\n");
    exit(1);
  }
  int read; //read return value

  //fread the whole header; contrary to the piece by piece of fwrite method
  read = fread(&hdr, sizeof(hdr), 1, fp);
  read = fread(buf, sizeof(buf), 1, fp);
  for (int i = 0; i < 410; i++) {
    if (i % 50 == 0) printf("\n");
    if (i % 25 == 0) printf("\n");
    printf("%d ", buf[i]);
  }
  printf("\n");

  int i;    int numBumps = 0;
  for (i = 0; i < 410; i++) {
    if (buf[i] > 0  && buf[i+1] < 0) {
      printf("%d: concave down\n", i);
      numBumps++;
    } else if (buf[i] < 0 && buf[i+1] > 0) {
      printf("%d concave up\n", i);
      numBumps++;
    } else {};

    if (numBumps == 2)
      break;
  }
  printf("numBumps: %d\n ", numBumps);

  printf("\nFrequency is: ");
  int freq = 44100 / i;
  printf("%d\n", freq);

  fclose(fp);
  free(filename);
  return 0;
}