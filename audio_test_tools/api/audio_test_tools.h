// Copyright (c) 2017, XMOS Ltd, All rights reserved

#ifndef TESTING_H_
#define TESTING_H_

#include <stdint.h>
#include <stdlib.h>
#include "dsp.h"

typedef struct  {
    double re;
    double im;
} dsp_complex_fp;

#define CRC_POLY (0xEB31D82E)
#define WAV_HEADER_BYTES 44

/*
 * Wav file stuff
 */
typedef struct wav_header {
    // RIFF Header
    char riff_header[4];    // Should be "RIFF"
    int wav_size;           // File size - 8 = data_bytes + WAV_HEADER_BYTES - 8
    char wave_header[4];    // Should be "WAVE"

    // Format Subsection
    char fmt_header[4];     // Should be "fmt "
    int fmt_chunk_size;     // Size of the rest of this subchunk
    short audio_format;
    short num_channels;
    int sample_rate;
    int byte_rate;          // sample_rate * num_channels * (bit_depth/8)
    short sample_alignment; // num_channels * (bit_depth/8)
    short bit_depth;        // bits per sample

    // Data Subsection
    char data_header[4];    // Should be "data"
    int data_bytes;         // frame count * num_channels * (bit_depth/8)
} wav_header;

int wav_header_to_struct(wav_header & s, char header[WAV_HEADER_BYTES]);
int wav_form_header(char header[WAV_HEADER_BYTES],
        short audio_format,
        short num_channels,
        int sample_rate,
        short bit_depth,
        int num_frames);

void wav_print_header(wav_header & s);

unsigned wav_get_num_bytes_per_frame(wav_header &s);
int wav_get_num_frames(wav_header &s);
long wav_get_frame_start(wav_header &s, unsigned frame_number);


/*
 * Double precision DTF stuff
 */
void bit_reverse    ( dsp_complex_fp pts[], const uint32_t N );
void forward_fft    ( dsp_complex_fp pts[], const uint32_t N, const double sine[]);
void inverse_fft    ( dsp_complex_fp pts[], const uint32_t N, const double sine[]);
void split_spectrum ( dsp_complex_fp pts[], const uint32_t N );
void merge_spectra  ( dsp_complex_fp pts[], const uint32_t N );

/*
 * Random number generation
 */
int32_t random_int32(unsigned &r);
uint32_t random_uint32(unsigned &r);
int64_t random_int64(unsigned &r);
uint64_t random_uint64(unsigned &r);

/*
 * Type conversion
 */
double int64_to_double(int64_t x, int x_exp);
double uint64_to_double(uint64_t x, int x_exp);
double int32_to_double(int32_t x, int x_exp);
double uint32_to_double(uint32_t x, int x_exp);
int32_t double_to_int32(double d, const int d_exp);
uint32_t double_to_uint32(double d, const int d_exp);

dsp_complex_fp complex_int32_to_double(dsp_complex_t x, int x_exp);

q8_24 uint32_to_q24(uint32_t v, int v_exp);


/*
 * Float/Fixed vector comparision
 */
unsigned bfp_vector_complex(dsp_complex_t * B, int B_exp, dsp_complex_fp * f, size_t start, size_t count);
unsigned bfp_vector_uint32(uint32_t * B, int B_exp, double * f, size_t start, size_t count);
unsigned bfp_vector_int32(int32_t * B, int B_exp, double * f, size_t start, size_t count);

/*
 * Python pretty printers
 */
void print_python_fd(dsp_complex_t * d, size_t length, int d_exp);
void print_python_td(dsp_complex_t * d, size_t length, int d_exp, int print_imag);
void print_python_int(int32_t * d, size_t length, int d_exp);
void print_python_uint(uint32_t * d, size_t length, int d_exp);

/*
 * Frame power calculation
 */
{uint32_t, int} get_fd_frame_power(dsp_complex_t * X, int X_shift, size_t bin_count);
{uint32_t, int} get_td_frame_power(dsp_complex_t * x, int x_shift, size_t frame_length, int imag_channel);

/*
 * Linear to log conversion
 */
q8_24 linear_to_db(uq8_24 l);
uq8_24 db_to_linear(q8_24 db);

#endif /* TESTING_H_ */
