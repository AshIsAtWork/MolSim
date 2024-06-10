FROM ubuntu:latest
LABEL authors="ashutosh.solanki"

RUN apt-get update && apt-get install -y procps
RUN apt-get install -y vim build-essential git cmake net-tools gdb clang valgrind
RUN apt-get install -y libboost-program-options-dev
RUN apt-get install -y python3 python3-pip python3-venv

# Create a virtual environment
RUN python3 -m venv /opt/venv

# Activate the virtual environment and install gprof2dot
RUN /opt/venv/bin/pip install --upgrade pip && \
    /opt/venv/bin/pip install gprof2dot

# Add the virtual environment's bin directory to the PATH
ENV PATH="/opt/venv/bin:$PATH"

RUN apt install -y graphviz
RUN apt install -y doxygen
RUN apt install -y libxerces-c-dev
RUN apt install -y xsdcxx


WORKDIR /work