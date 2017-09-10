
#### install fastDfs
```
    git clone https://github.com/happyfish100/libfastcommon.git
    cd libfastcommon 
    ./make.sh 
    ./make.sh install 
    
    https://github.com/happyfish100/fastdfs.git
    cd fastdfs
    ./make.sh
    ./make.sh install
    
    cp /etc/fdfs/tracker.conf.sample /etc/fdfs/tracker.conf
    cp /etc/fdfs/storage.conf.sample /etc/fdfs/storage.conf
```

#### start fastDfs
```
/etc/rc.d/init.d/fdfs_trackerd start
Starting fdfs_trackerd (via systemctl):                    [  OK  ]

tail -f /var/log/messages
Sep 10 00:21:43 localhost systemd: fdfs_trackerd.service failed.
Sep 10 00:21:43 localhost systemd: Starting LSB: FastDFS tracker server...
Sep 10 00:21:43 localhost fdfs_trackerd: Starting FastDFS tracker server:
Sep 10 00:21:43 localhost systemd: Started LSB: FastDFS tracker server.
Sep 10 00:21:43 localhost fdfs_trackerd: [2017-09-10 00:21:43] ERROR - file: process_ctrl.c, line: 288, "/home/yuqing/fastdfs" can't be accessed, error info: No such file or directory
Sep 10 00:23:37 localhost systemd: Cannot add dependency job for unit fdfs_storaged.target, ignoring: Unit not found.
Sep 10 00:23:37 localhost systemd: Starting LSB: FastDFS storage server...
Sep 10 00:23:37 localhost fdfs_storaged: Starting FastDFS storage server:
Sep 10 00:23:37 localhost systemd: Started LSB: FastDFS storage server.
Sep 10 00:23:37 localhost fdfs_storaged: [2017-09-10 00:23:37] ERROR - file: process_ctrl.c, line: 288, "/home/yuqing/fastdfs" can't be accessed, error info: No such file or directory
```
###### 以上的错误,需要修改配置文件

```
vim /etc/fds/tracker.conf

# the base path to store data and log files
#### fixed base_path must exists
base_path=/fastdfs_tracker/

vim /etc/fds/storage.conf

# the base path to store data and log files
base_path=/fastdfs_storage

#### fixed store_path0 must exists
store_path0=/fastdfs_storage/

#### change ip, 不能使用 127.0.0.1 
tracker_server=192.168.0.232:22122

```

###### 创建文件夹
```
mkdir -p /fastdfs_tracker/data/
mkdir -p /fastdfs_tracker/logs/

mkdir -p /fastdfs_storage/data/
mkdir -p /fastdfs_storage/logs/
```

```
/etc/rc.d/init.d/fdfs_trackerd restart 
Restarting fdfs_trackerd (via systemctl):                  [  OK  ]

ps -ef | grep fdfs_trackerd 
root     24826     1  0 00:39 ?        00:00:00 /usr/bin/fdfs_trackerd /etc/fdfs/tracker.conf


/etc/rc.d/init.d/fdfs_storage restart 
ps -ef | grep fdfs_storage  
root     25301     1  1 00:55 ?        00:00:01 /usr/bin/fdfs_storaged /etc/fdfs/storage.conf restart

```

###### 日志目录
```
/fastdfs_tracker/logs
/fastdfs_storage/logs
```


#### 说明
    - 先启动tracker
    - 再启动storage
    
    
#### 文件上传测试


```
/usr/bin/fdfs_upload_file /etc/fdfs/client.conf /tmp/5100a5f0a0c9b.png 
group1/M00/00/00/wKgA6Fm0IGSAXwNHAASNAuw4eSQ796.png
```

#### 检查文件是否存在

```
ll /fastdfs_storage/data/00/00/
total 292
-rw-r--r-- 1 root root 298242 Sep 10 01:09 wKgA6Fm0IGSAXwNHAASNAuw4eSQ796.png
```


