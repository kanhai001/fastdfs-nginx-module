
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "input_image_magic.h"


int main() {

    char filename[] = "00/00/wKgA6Fm0IGSAXwNHAASNAuw4eSQ796.png_120x120x20.jpg";
    int len = strlen(filename);

    char base_path[] = "/fastdfs_storage/data";
    int rst = check_input_is_image_convert(base_path,filename,len);
    if(rst) {
        printf("xxxx convert--\n");
    }
}
