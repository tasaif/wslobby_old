#!/bin/bash

rm a.out
g++ -L ./jsoncpp/src/lib_json -I ./jsoncpp/include *.cpp -lcurlpp -lcurl -ljsoncpp -lboost_system > build_log
