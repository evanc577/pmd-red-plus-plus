FROM docker.io/library/almalinux:latest
RUN dnf groupinstall -y 'Development Tools'
RUN dnf install -y epel-release
RUN dnf install -y libpng-devel  arm-none-eabi-binutils-cs arm-none-eabi-gcc-cs

RUN cd /opt && \
    git clone https://github.com/pret/agbcc && \
    cd agbcc && \
    ./build.sh
