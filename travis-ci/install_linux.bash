#!/bin/bash
set -ev
docker exec -ti ${MY_NAME} bash -c "apt update -q"
docker exec -ti ${MY_NAME} bash -c "apt upgrade -q -y"
docker exec -ti ${MY_NAME} bash -c "apt install -q -y build-essential git cmake libgmp-dev"

#docker exec -ti ${MY_NAME} bash -c "git clone --depth 1 https://github.com/fmtlib/fmt.git"
#docker exec -ti ${MY_NAME} bash -c "cd fmt; mkdir -p build; cd build; cmake ..; make -j; make install"
docker exec -ti ${MY_NAME} bash -c "apt install -q -y libfmt-dev"

docker exec -ti ${MY_NAME} bash -c "git clone --depth 1 https://github.com/google/googletest.git"
docker exec -ti ${MY_NAME} bash -c "cd googletest; mkdir -p build; cd build; cmake .. -DBUILD_SHARED_LIBS=ON -DINSTALL_GTEST=ON -DCMAKE_INSTALL_PREFIX:PATH=/usr; make -j; make install; ldconfig"
