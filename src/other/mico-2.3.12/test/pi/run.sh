#!/bin/sh

echo ""
echo "Testing oneway-context (3 times)"
echo ""

cd oneway-context
./run|diff -u expected-output -
sleep 1
./run|diff -u expected-output -
sleep 1
./run|diff -u expected-output -
sleep 1

echo ""
echo "Testing oneway-request (3 times)"
echo ""

cd ../oneway-request
./run|diff -u expected-output -
sleep 1
./run|diff -u expected-output -
sleep 1
./run|diff -u expected-output -
sleep 1

echo ""
echo "Testing parameter-passing/object-out (1 time)"
echo ""

cd ../parameter-passing/object-out
./runit

echo ""
echo "Testing poacurrent (1 time)"
echo ""

cd ../../poacurrent
./run

echo ""
echo "Testing requestinfo-validity (3 times)"
echo ""

cd ../requestinfo-validity
./run|diff -u expected-output -
sleep 1
./run|diff -u expected-output -
sleep 1
./run|diff -u expected-output -
sleep 1

echo ""
echo "Testing rethrow-exception (3 times)"
echo ""

cd ../rethrow-exception
./run|diff -u expected-output -
sleep 1
./run|diff -u expected-output -
sleep 1
./run|diff -u expected-output -
sleep 1

echo ""
echo "Testing sending-received-exception (3 times)"
echo ""

cd ../sending-received-exception
./run|diff -u expected-output -
sleep 1
./run|diff -u expected-output -
sleep 1
./run|diff -u expected-output -
sleep 1

echo ""
echo "Testing transient-exception (3 times)"
echo ""

cd ../transient-exception
./run|diff -u expected-output -
sleep 1
./run|diff -u expected-output -
sleep 1
./run|diff -u expected-output -
sleep 1


echo ""
echo "Testing twoway-context (3 times)"
echo ""

cd ../twoway-context
./run|diff -u expected-output -
sleep 1
./run|diff -u expected-output -
sleep 1
./run|diff -u expected-output -
sleep 1

echo ""
echo "Testing twoway-request (3 times)"
echo ""

cd ../twoway-request
./run|diff -u expected-output -
sleep 1
./run|diff -u expected-output -
sleep 1
./run|diff -u expected-output -
sleep 1

echo ""
echo "Testing user-exception (3 times)"
echo ""

cd ../user-exception
./run|diff -u expected-output -
sleep 1
./run|diff -u expected-output -
sleep 1
./run|diff -u expected-output -
sleep 1

echo ""
echo "Testing dii-request (3 times)"
echo ""

cd ../dii-request
./run|diff -u expected-output -
sleep 1
./run|diff -u expected-output -
sleep 1
./run|diff -u expected-output -
sleep 1

