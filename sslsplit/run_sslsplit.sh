#!/bin/sh

sslsplit \
-D -l connect.log \
-j /home/xyi/sslsplit \
-S /home/xyi/sslsplit/logdir \
-k hbase_pri.key \
-c hbase.crt \
ssl 0.0.0.0 10000
