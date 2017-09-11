## 分支说明

#### 添加图片格式转换

```
    强行修改为100x200,图片质量为90
    http://aaaa.com/M01/01/01/aaaaaaa.jpeg_100x200x90.png
    
    强行修改宽度为100,高度等比原图,图片质量为80
    http://aaaa.com/M01/01/01/aaaaaaa.jpeg_100x0x80.png
    
    强行修改高度为100,宽度等比原图,图片质量为70
    http://aaaa.com/M01/01/01/aaaaaaa.jpeg_0x100x70.png
    
    图片质量只能为1-100
```




#### imageMagic
```
    yum install ImageMagick ImageMagick-devel -y
```


https://www.imagemagick.org/script/magick-core.php

https://www.imagemagick.org/MagickWand/resize.htm

https://github.com/ImageMagick/ImageMagick/issues/29

It looks like the value that the PNG compression reads was changed from wand->image_info->quality to wand->images->quality in this commit





