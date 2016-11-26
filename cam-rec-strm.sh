#!/bin/bash

while [ 1 ]
do
raspivid -o - -t 0 -hf -vf -w 1080 -h 768 -fps 20 | ./split.o | cvlc -vvv stream:///dev/stdin --sout '#standard{access=http,mux=ts,dst=:8160}' :demux=h264 &
sleep $1
kill $!
done
