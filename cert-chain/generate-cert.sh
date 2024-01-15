#! /usr/bin/bash

set -e

# 创建root-ca并自签名
# openssl req -x509 -newkey rsa:4096 -nodes -keyout rootca.key -out rootca.crt -days 3650  -subj "/C=CN/ST=shanghai/L=shanghai/O=example/OU=it/CN=rootca"

# 查看创建的证书
# openssl x509 -in rootca.crt -text -noout

# openssl req -new -newkey rsa:4096 -nodes -keyout server0.key -out server0.csr -subj "/C=CN/ST=shanghai/L=shanghai/O=example/OU=it/CN=server0"

# openssl x509 -req -in server0.csr -CA rootca.crt -CAkey rootca.key -CAcreateserial -out server0.crt -days 3650

# openssl x509 -in server0.crt -text -noout

# cat server0.crt rootca.crt > bundle0.crt

# openssl req -new -newkey rsa:4096 -nodes -keyout ca1.key -out ca1.csr -subj "/C=CN/ST=shanghai/L=shanghai/O=example/OU=it/CN=ca1"

# openssl x509 -req -in ca1.csr -CA rootca.crt -CAkey rootca.key -CAcreateserial -out ca1.crt -days 3650

# openssl x509 -in ca1.crt -text -noout

# openssl req -new -newkey rsa:4096 -nodes -keyout server1.key -out server1.csr -subj "/C=CN/ST=shanghai/L=shanghai/O=example/OU=it/CN=server1"

# openssl x509 -req -in server1.csr -CA ca1.crt -CAkey ca1.key -CAcreateserial -out server1.crt -days 3650

# cat server1.crt ca1.crt rootca.crt > bundle1.crt

openssl req -new -newkey rsa:4096 -nodes -keyout ca2.key -out ca2.csr -subj "/C=CN/ST=shanghai/L=shanghai/O=example/OU=it/CN=ca2"

openssl x509 -req -in ca2.csr -CA ca1.crt -CAkey ca1.key -CAcreateserial -out ca2.crt -days 3650

openssl req -new -newkey rsa:4096 -nodes -keyout server2.key -out server2.csr -subj "/C=CN/ST=shanghai/L=shanghai/O=example/OU=it/CN=server2"

openssl x509 -req -in server2.csr -CA ca2.crt -CAkey ca2.key -CAcreateserial -out server2.crt -days 3650

cat server2.crt ca2.crt ca1.crt rootca.crt > bundle2.crt