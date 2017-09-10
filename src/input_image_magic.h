#ifndef INPUT_IMAGE_MAGIC_H
#define INPUT_IMAGE_MAGIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_input_is_image_convert(const char* base_path, const char *filename, const int len);

int image_convert_operate(const char *disk_filename,
                    const char *save_disk_filename,
                    const int width, const int height, const int quality);

#endif