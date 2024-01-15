#! /usr/bin/bash

set -e

cat server0.crt > loop$1.crt

for n in $(seq 1 $1)
do
 cat loop$1.crt rootca.crt > loop$1.crt
done