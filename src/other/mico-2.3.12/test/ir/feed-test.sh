#!/bin/sh
for i in `ls *.idl`
do
  ir-feed.sh $i
done
