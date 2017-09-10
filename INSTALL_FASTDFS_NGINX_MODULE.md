
#### compiler fastdfs-nginx-module to nginx 

    - download nginx src from http://nginx.org
    - download fastdfs-nginx-module
    
```
   ./configure --prefix=/data/nginx --add-module=../fastdfs-nginx-module/src/
   make && make install
```



#### cp mod_fastdfs.conf from current project to /etc/fdfs/mod_fastdfs.conf

```
    cp fastdfs-nginx-module/src/mod_fastdfs.conf /etc/fdfs/mod_fastdfs.conf   
```

#### start nginx 

maybe find in nginx error log 

```
    [2017-09-10 15:12:36] ERROR - file: ini_file_reader.c, line: 631, include file "http.conf" not exists, line: "#include http.conf"
    [2017-09-10 15:12:36] ERROR - file: ../fastdfs-nginx-module/src//common.c, line: 163, load conf file "/etc/fdfs/mod_fastdfs.conf" fail, ret code: 2
```

and maybe find something in /fastdfs_storate/logs/

```
==> nginx_mod.log <==
[2017-09-10 15:20:50] ERROR - file: ../common/fdfs_http_shared.c, line: 153, param "http.mime_types_filename" not exist or is empty
```

#### change mod_fastdfs.conf

```
log_filename=/fastdfs_storage/logs/nginx_mod.log
#include http.conf
```

#### add http.conf

```
vim /etc/fdfs/http.conf
http.default_content_type = application/octet-stream  
http.mime_types_filename=/data/nginx/conf/mime.types
```


```
ls /etc/fdfs/*.conf
client.conf  http.conf  mod_fastdfs.conf  storage.conf  storage_ids.conf  tracker.conf
```

## config nginx.conf

### config the nginx config file such as nginx.conf, add the following lines:

        location /M00 {
            root /fastdfs_storage/data;
            ngx_fastdfs_module;
        }

#### step 6. make a symbol link ${fastdfs_base_path}/data/M00 to ${fastdfs_base_path}/data,
         command line such as:
ln -s /fastdfs_storage/data  /fastdfs_storage/data/M00


#### test request http 
    wget http://192.168.0.232/M00/00/00/wKgA6Fm0IGSAXwNHAASNAuw4eSQ796.png
