FROM --platform=linux/amd64 ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y linux-libc-dev

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y \
    build-essential \
    gcc \
    g++ \
    make \
    linux-libc-dev \
    libc6-dev \
    libc6-dev-i386 \
    gdb \
    strace \
    lldb \
    vim \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

SHELL ["/bin/bash", "-c"]

WORKDIR /workspace

CMD ["bash"]