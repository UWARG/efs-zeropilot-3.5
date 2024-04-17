FROM ubuntu:jammy-20240125

LABEL Description="EFS Linux Build Environment (Ubuntu 22.04)"

ENV HOME /root

SHELL ["/bin/bash", "-c"]

RUN apt-get update && apt-get -y --no-install-recommends install \
    wget \
    xz-utils \
    tar \
    make \
    cmake \
    ca-certificates \
    g++ \
    git

# ARM GNU Toolchain
ARG ARM_PKG_NAME=arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi
RUN wget https://developer.arm.com/-/media/Files/downloads/gnu/12.3.rel1/binrel/arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi.tar.xz -O /tmp/$ARM_PKG_NAME.tar.xz && \
    tar -xvf /tmp/$ARM_PKG_NAME.tar.xz -C /usr/share/ && \
    ln -s /usr/share/$ARM_PKG_NAME/bin/arm-none-eabi-gcc /usr/bin/arm-none-eabi-gcc && \
    ln -s /usr/share/$ARM_PKG_NAME/bin/arm-none-eabi-g++ /usr/bin/arm-none-eabi-g++ && \
    ln -s /usr/share/$ARM_PKG_NAME/bin/arm-none-eabi-gdb /usr/bin/arm-none-eabi-gdb && \
    ln -s /usr/share/$ARM_PKG_NAME/bin/arm-none-eabi-size /usr/bin/arm-none-eabi-size && \
    ln -s /usr/share/$ARM_PKG_NAME/bin/arm-none-eabi-objcopy /usr/bin/arm-none-eabi-objcopy

# clang-tidy and clang-format
RUN wget https://apt.llvm.org/llvm-snapshot.gpg.key -O /etc/apt/keyrings/llvm-snapshot.gpg.key && \
    echo "deb [signed-by=/etc/apt/keyrings/llvm-snapshot.gpg.key] http://apt.llvm.org/jammy/ llvm-toolchain-jammy-18 main" >> /etc/apt/sources.list.d/llvm.list && \
    echo "deb-src [signed-by=/etc/apt/keyrings/llvm-snapshot.gpg.key] http://apt.llvm.org/jammy/ llvm-toolchain-jammy-18 main" >> /etc/apt/sources.list.d/llvm.list && \
    chmod 644 /etc/apt/keyrings/llvm-snapshot.gpg.key && \
    chmod 644 /etc/apt/sources.list.d/llvm.list && \
    apt-get update && \
    apt-get -y --no-install-recommends install clang-tidy-18 && \
    ln -s /usr/bin/clang-tidy-18 /usr/bin/clang-tidy && \
    apt-get -y --no-install-recommends install clang-format-18 && \
    ln -s /usr/bin/clang-format-18 /usr/bin/clang-format

# cppcheck
ARG CPPCHECK_PKG_NAME=cppcheck-2.13.0
RUN wget https://github.com/danmar/cppcheck/archive/2.13.0.tar.gz -O /tmp/$CPPCHECK_PKG_NAME.tar.gz && \
    tar -xvf /tmp/$CPPCHECK_PKG_NAME.tar.gz -C /tmp/ && \
    cd /tmp/$CPPCHECK_PKG_NAME && \
    make FILESDIR=/usr/share/cppcheck && \
    make install FILESDIR=/usr/share/cppcheck

# GoogleTest
RUN git clone https://github.com/google/googletest.git -b v1.14.0 /tmp/googletest && \
    cd /tmp/googletest && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make && \
    make install

# Cleanup
RUN rm -rf /tmp/*