#!/bin/sh
#set -x
METHOD="perform perform_oneway perform_with_context perform_oneway_with_context"
TRANSPORT="tcp ssl udp csiv2"

for i in $TRANSPORT
do
  echo
  echo "Testing transport: " $i
  ./$i-server &
  server_pid=$!
  sleep 2
  for j in $METHOD
  do
    echo
    echo "method: " $j
    echo
    cat ref|./$i-client ior $j 300000 0 0 &
    client_pid=$!
    sleep 10
    echo
    echo "processes after 10 seconds:"
    echo
    ps ux|grep ./server
    ps ux|grep ./client
    sleep 60
    echo
    echo "processes after 60 seconds:"
    echo
    ps ux|grep ./server
    ps ux|grep ./client
    wait $client_pid
    echo
    echo "server process after client finish:"
    echo
    ps ux|grep ./server
  done
  killall server
  sleep 1
  killall server
  sleep 1
  killall server
  sleep 1
  killall server
  sleep 1
  killall server
done

