FROM ubuntu:latest
LABEL authors="ashutosh.solanki"

RUN apt-get update && apt-get install -y procps
RUN apt-get install -y vim build-essential git cmake net-tools gdb clang valgrind
RUN apt-get install -y libboost-program-options-dev
RUN apt install -y graphviz
RUN apt install -y doxygen
RUN apt install -y libxerces-c-dev

WORKDIR /work