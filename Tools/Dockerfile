FROM ubuntu:jammy-20240125
LABEL Description="EFS Linux Build Environment (Ubuntu 22.04)"

ENV HOME /root

SHELL ["/bin/bash", "-c"]

RUN apt-get update && apt-get -y --no-install-recommends install \
    wget \
    xz-utils \
    tar \
    make \
    cmake

ARG ARM_PKG_LINK=https://developer.arm.com/-/media/Files/downloads/gnu/12.3.rel1/binrel/arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi.tar.xz
ARG ARM_PKG_NAME=arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi
RUN wget --no-check-certificate https://developer.arm.com/-/media/Files/downloads/gnu/12.3.rel1/binrel/arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi.tar.xz -O /tmp/$ARM_PKG_NAME.tar.xz && \
    tar -xvf /tmp/$ARM_PKG_NAME.tar.xz -C /usr/share/ && \
    ln -s /usr/share/$ARM_PKG_NAME/bin/arm-none-eabi-gcc /usr/bin/arm-none-eabi-gcc && \
    ln -s /usr/share/$ARM_PKG_NAME/bin/arm-none-eabi-g++ /usr/bin/arm-none-eabi-g++ && \
    ln -s /usr/share/$ARM_PKG_NAME/bin/arm-none-eabi-gdb /usr/bin/arm-none-eabi-gdb && \
    ln -s /usr/share/$ARM_PKG_NAME/bin/arm-none-eabi-size /usr/bin/arm-none-eabi-size && \
    ln -s /usr/share/$ARM_PKG_NAME/bin/arm-none-eabi-objcopy /usr/bin/arm-none-eabi-objcopy

RUN rm -rf /tmp/*