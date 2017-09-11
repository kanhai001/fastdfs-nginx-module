

#include "logger.h"
#include "input_image_magic.h"

#define CHECK_TO_PNG ".png"
#define CHECK_TO_JPG ".jpg"
#define CHECK_TO_JPEG ".jpeg"
#define CHECK_TO_GIF ".gif"

#define CHECK_FROM_PNG ".png_"
#define CHECK_FROM_JPG ".jpg_"
#define CHECK_FROM_JPEG ".jpeg_"
#define CHECK_FROM_GIF ".gif_"

#include <magick/MagickCore.h>

static char* from_fmt_arr[] = {
    CHECK_FROM_PNG,
    CHECK_FROM_JPG,
    CHECK_FROM_JPEG,
    CHECK_FROM_GIF
};

static char* to_fmt_arr[] = {
    CHECK_TO_PNG,
    CHECK_TO_JPG,
    CHECK_TO_JPEG,
    CHECK_TO_GIF
};

#define PARSE_CONVERT_MAX_LEN  32

#define PARSE_CONVERT_CHAR 'x'

#define IMG_MAX_WIDTH   8
#define IMG_MAX_HEIGHT  8
#define IMG_MAX_QUALITY 3

#define DISK_FILE_NAME_MAX_LEN 256

#define PRIVATE_IS_NUMBER(ch) ((ch >= '0' && ch <= '9'))

#define DATA_NAME "/data/"

#define DATA_LEN 6

// 判断文件是否为图片转换
int check_input_is_image_convert(const char* base_path, const char *filename, const int len){
    int from_idx = -1;
    int to_idx = -1;

    int i = 0 ;

    char *prv_name = NULL;
    char *latest_ext = NULL;
    char disk_filename[DISK_FILE_NAME_MAX_LEN] = {0x00};
    char save_disk_filename[DISK_FILE_NAME_MAX_LEN] = {0x00};
    int from_fmt_size = sizeof(from_fmt_arr)/sizeof(char*);
    for(i = 0 ; i < from_fmt_size ; i++) {
        if((prv_name = strstr(filename,from_fmt_arr[i])) != NULL){
            from_idx = i;
            break;
        }
    }

    if( from_idx == -1 ) {
        // failed
        return 0;
    }

    // logError("file: "__FILE__", line: %d, from idx %d " , __LINE__, from_idx);

    int disk_file_len = prv_name - filename + strlen(from_fmt_arr[from_idx]) - 1;
    strncpy(disk_filename, base_path, strlen(base_path));
    strncat(disk_filename + strlen(base_path), DATA_NAME, 6);
    strncat(disk_filename + strlen(base_path) + 6, filename, disk_file_len);

    strncpy(save_disk_filename, base_path, strlen(base_path));
    strncat(save_disk_filename + strlen(base_path), DATA_NAME, 6);
    strncat(save_disk_filename + strlen(base_path) + 6, filename, len);

//    logError("file: "__FILE__", line: %d, prv_name: %s disk_file_len: %d \r\ndisk_filename:%s\r\nsave disk_filename:%s \r\n",
//        __LINE__,
//        prv_name, disk_file_len, disk_filename,
//        save_disk_filename);

    int to_fmt_size = sizeof(to_fmt_arr)/sizeof(char*);
    // find prv
    char *conv_uri = (prv_name + strlen(from_fmt_arr[from_idx]));
    for(i = 0 ; i < to_fmt_size; i++) {
        if((latest_ext = strstr(conv_uri,to_fmt_arr[i])) != NULL) {
            to_idx = i;
            break;
        }
    }

    if(to_idx == -1 ) {
        // failed
        return 0;
    }

    // logError("file: "__FILE__", line: %d, from idx:%d to:idx:%d %s\r\n", __LINE__, from_idx, to_idx, latest_ext);


    if(strlen(latest_ext) != strlen(to_fmt_arr[to_idx])) {
        // latest_ext havv error character
        return 0;
    }

    int parse_all_len = latest_ext - prv_name;

    char *parse_all_str = (prv_name + strlen(from_fmt_arr[from_idx]));

    if(parse_all_len > PARSE_CONVERT_MAX_LEN) {
        // must len < 32
        return 0;
    }

//    char parse_str[PARSE_CONVERT_MAX_LEN*2] = {0x00};

    // 解析 WidthxHeightxQuality

    int width = -1;
    int height = -1;
    int quality = -1;

    // logError("file: "__FILE__", line: %d parse_all_str: %s latest_ext:%s \r\n",__LINE__, parse_all_str,latest_ext);

    char *width_start = strchr(parse_all_str,PARSE_CONVERT_CHAR);
    int width_len = 0;

    if((width_len = (width_start - parse_all_str)) > IMG_MAX_WIDTH ) {
        // width to longer
        return 0;
    }
    char width_str[IMG_MAX_WIDTH] = {0x00};
    if(width_len > 0) {
        strncpy(width_str,parse_all_str,width_len);
    }

//    logInfo("image width_str:%s \r\n", width_str);
    // printf("2 find start:%s width:%s \r\n", width_start, width_str);

    char *height_start = strchr((width_start+1),PARSE_CONVERT_CHAR);
    int height_len = 0;
    if((height_len = (height_start - (width_start+1))) > IMG_MAX_HEIGHT ) {
        // width to longer
        return 0;
    }

    char height_str[IMG_MAX_HEIGHT] = {0x00};
    if(height_len > 0) {
        strncpy(height_str,(width_start+1),height_len);
    }

//    logInfo("image height_str:%s \r\n", height_str);

    // printf("3 find start:%s height:%s height_len:%d \r\n", height_start, height_str, height_len);

    char *quality_start = latest_ext;
    int quality_len = 0;
    if((quality_len = (quality_start - (height_start+1))) > IMG_MAX_HEIGHT ) {
            // width to longer
        return 0;
    }

    char quality_str[IMG_MAX_HEIGHT] = {0x00};
    if(quality_len > 0) {
        strncpy(quality_str,(height_start+1),quality_len);
    }

//    logInfo("image quality_str:%s \r\n", quality_str);

    // printf("4 find start:%s quality:%s quality_len:%d \r\n", quality_start, quality_str, quality_len);

//    logInfo("image width_len:%d  height_len:%d \r\n", width_len,height_len);

    if (width_len == 0 && height_len == 0) {
        return 0;
    }

    int tx = 0 ;
    if(width_len > 0) {
        for(tx = 0 ; tx < width_len ; tx++) {
            if(!PRIVATE_IS_NUMBER(width_str[tx])){
                return 0;
            }
        }
        width = atoi(width_str);
        if(width < 0) {
            return 0;
        }
    }

    if( height_len > 0 ) {
        for(tx = 0 ; tx < height_len ; tx++) {
            if(!PRIVATE_IS_NUMBER(height_str[tx])){
                return 0;
            }
        }
        height = atoi(height_str);
        if (height < 0) {
            return 0;
        }
    }

    if( quality_len > 0 ) {
        for(tx = 0 ; tx < quality_len ; tx++) {
            if(!PRIVATE_IS_NUMBER(quality_str[tx])){
                return 0;
            }
        }
        quality = atoi(quality_str);
        if ( quality <= 0 || quality > 100 ) {
            return 0;
        }
    }

//    logInfo("image width:%d  height:%d \r\n", width,height);

    if (width <= 0 && height <= 0) {
        return 0;
    }

    return image_convert_operate(disk_filename, save_disk_filename, width, height, quality);
}


int image_convert_operate(const char *disk_filename,
                    const char *save_disk_filename,
                    const int width, const int height, const int quality) {


  ExceptionInfo *exception;

  Image
    *image,
    *images,
    *resize_image,
    *thumbnails;

  ImageInfo *image_info;

  MagickCoreGenesis(NULL, MagickTrue);
  exception=AcquireExceptionInfo();
  image_info=CloneImageInfo((ImageInfo *) NULL);
  (void) strcpy(image_info->filename,disk_filename);
  images=ReadImage(image_info,exception);
  if (exception->severity != UndefinedException)
    CatchException(exception);
  if (images == (Image *) NULL) {
    return 0;
  }

  size_t rows = images->rows;
  size_t columns = images->columns;

  size_t re_width = 0 ;
  size_t re_height = 0 ;
  double re_rate = 1.0;
  if(width <= 0) {
    // 自适应宽度
    re_rate = rows * 100.0 / height;
    re_width = columns * 100.0 / re_rate;
    re_height = height;
  } else if( height <= 0 ) {
    re_width = width;
    // 自适应高度
    re_rate = columns * 100.0 / width ;
    re_height =  rows * 100.0 / re_rate;
  } else {
    re_width = width;
    re_height = height;
  }

//  logInfo("image read width:%d height:%d, rate:%lf after width: %d height:%d quality:%d fielname:%s \r\n",
//        columns,
//        rows,re_rate,
//        re_width,
//        re_height,
//        quality,
//        save_disk_filename);

  thumbnails=NewImageList();
  while ((image=RemoveFirstImageFromList(&images)) != (Image *) NULL)
  {
    resize_image=ResizeImage(image,re_width,re_height,LanczosFilter, 1.0 , exception);
    if (resize_image == (Image *) NULL){
        MagickError(exception->severity,exception->reason,exception->description);
    }
    (void) AppendImageToList(&thumbnails,resize_image);
    DestroyImage(image);
  }
  /*
    Write the image thumbnail.
  */
  (void) strcpy(thumbnails->filename,save_disk_filename);

  if( quality > 0 && quality <= 100 ) {
    // thumbnails->compression = (CompressionType) JPEGCompression;
    thumbnails->quality = quality ;
  }

  WriteImage(image_info,thumbnails);
  /*
    Destroy the image thumbnail and exit.
  */
  thumbnails=DestroyImageList(thumbnails);
  image_info=DestroyImageInfo(image_info);
  exception=DestroyExceptionInfo(exception);
  MagickCoreTerminus();
  return 1;
}