# global args
ARG user=user
ARG homedir=/home/${user}
ARG nprocs=4

FROM ubuntu:latest

# to avoid user interaction with tzdata asking for timezone
ARG DEBIAN_FRONTEND=noninteractive

#
ARG user
ARG homedir
ARG nprocs

# environment
# colorize the terminal
ENV TERM=xterm-256color
# openmpi
ENV OMPI_ALLOW_RUN_AS_ROOT=1
ENV OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1
ENV OMPI_MCA_rmaps_base_oversubscribe=1

# get the latest
RUN apt dist-upgrade -y

# update the package repository
RUN apt update -y

# install multipurpose packages
RUN apt install -y cmake-curses-gui
RUN apt install -y git
RUN apt install -y git-core
RUN apt install -y bash-completion
RUN apt install -y make
RUN apt install -y vim
RUN apt install -y sudo

# install mito dependencies
RUN apt install -y libgtest-dev
RUN apt install -y libbenchmark-dev
RUN apt install -y libopenmpi-dev
RUN apt install -y libmetis-dev
RUN apt install -y libvtk9-dev
RUN apt install -y petsc-dev
RUN apt install -y valgrind
RUN apt install -y libpython3-dev
RUN apt install -y python3-pytest

# install pyre dependencies
RUN apt install -y pybind11-dev

# install pyre
WORKDIR ${homedir}
RUN git clone https://github.com/pyre/pyre \
    && cd pyre \
    && git checkout tensor \
    && mkdir build \
    && cd build \
    && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local/pyre .. \
    && make -j ${nprocs} \
    && make install \
    && cd .. \
    && rm -rf build

# setup {user} user
ENV USER=${user}
ENV HOME=${homedir}
RUN useradd -s /bin/bash --create-home --home-dir $HOME $USER \
    && usermod -aG sudo $USER \
    && chown -R $USER:$USER $HOME
RUN echo "$USER:password" | chpasswd
# copy the bashrc
COPY --chown=${user}:${user} bashrc .bashrc

# default user and work directory when running the container
USER ${user}
WORKDIR ${homedir}
# remove annoying message on how to run a sudo command
RUN touch ~/.sudo_as_admin_successful
# configure git
RUN git config --global pull.ff only

# end of file
