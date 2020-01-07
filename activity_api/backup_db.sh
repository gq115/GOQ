#!/bin/bash
kdxy=$1 
basepath=$(cd `dirname $0`; pwd)
# echo $kdxy
location=${basepath}"/backup_db"
mkdir -p $location
var1=`date +%Y%m%d%H%M%S`

cp -R -f ${kdxy}"/db" $location"/db"

pth=$location/db_${var1}".tar.gz"
cd $kdxy
tar -czvf $pth  db > null & 
cd $basepath

find $location -mtime +3 -type f | xargs rm -rf #删除三天前的备份

