FROM ubuntu:jammy-20240125
LABEL Description="EFS Linux Build Environment (Ubuntu 22.04)"

ENV HOME /root

SHELL ["/bin/bash", "-c"]

RUN apt-get update

RUN apt-get -y --no-install-recommends install \
    wget \
    xz-utils \
    tar \
    make \
    sudo

ARG ARM_PKG_LINK=https://developer.arm.com/-/media/Files/downloads/gnu/12.3.rel1/binrel/arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi.tar.xz
ARG ARM_PKG_NAME=arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi
RUN wget --no-check-certificate https://developer.arm.com/-/media/Files/downloads/gnu/12.3.rel1/binrel/arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi.tar.xz -O /tmp/$ARM_PKG_NAME.tar.xz && \
    tar -xvf /tmp/$ARM_PKG_NAME.tar.xz -C /usr/share/ && \
    sudo ln -s /usr/share/$ARM_PKG_NAME/bin/arm-none-eabi-gcc /usr/bin/arm-none-eabi-gcc && \
    sudo ln -s /usr/share/$ARM_PKG_NAME/bin/arm-none-eabi-g++ /usr/bin/arm-none-eabi-g++ && \
    sudo ln -s /usr/share/$ARM_PKG_NAME/bin/arm-none-eabi-gdb /usr/bin/arm-none-eabi-gdb && \
    sudo ln -s /usr/share/$ARM_PKG_NAME/bin/arm-none-eabi-size /usr/bin/arm-none-eabi-size && \
    sudo ln -s /usr/share/$ARM_PKG_NAME/bin/arm-none-eabi-objcopy /usr/bin/arm-none-eabi-objcopy

ARG CMAKE_PKG_LINK=https://github.com/Kitware/CMake/releases/download/v3.28.2/cmake-3.28.2-linux-x86_64.tar.gz
ARG CMAKE_PKG_NAME=cmake-3.28.2-linux-x86_64
RUN wget --no-check-certificate $CMAKE_PKG_LINK -O /tmp/$CMAKE_PKG_NAME.tar.gz && \
    tar -xvf /tmp/$CMAKE_PKG_NAME.tar.gz -C /usr/share/ && \
    cp /usr/share/$CMAKE_PKG_NAME/bin/* /usr/bin/ && \
    cp -r /usr/share/$CMAKE_PKG_NAME/share/* /usr/share/

RUN rm -rf /tmp/*