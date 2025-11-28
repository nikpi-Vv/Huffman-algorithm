#ifndef CODEC_H
#define CODEC_H

#include "treelist.h"

int encode_file(const char *input_filename, 
                const char *output_filename, 
                code_table_t *table, 
                unsigned char *frequencies, 
                long input_len);

int decode_file(const char *input_filename, 
                const char *output_filename);

#endif