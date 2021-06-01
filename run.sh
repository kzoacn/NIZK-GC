killall test
./test 1 12345 &
sleep 0.1
time ./test 2 12345 &
