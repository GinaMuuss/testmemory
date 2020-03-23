FROM ubuntu:latest

# install dependencies and utilites
RUN apt update
RUN apt install -y vim htop gcc make

RUN mkdir /test

# Copy files
COPY makefile /test/.
COPY testmemory.c /test/.

# Compile the test
WORKDIR /test
RUN make

ENTRYPOINT ["/test/testmemory"]
