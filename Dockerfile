# use the Ubuntu (debian) operating system
# ----------------------------------------
FROM ubuntu:latest

# update software in the image that will
# will be run as a container
# --------------------------------------
RUN apt-get update
RUN apt-get install -y apt-utils
RUN apt-get -y install make

# Ensure locales configured correctly
# -----------------------------------
ENV LC_ALL='en_US.utf8'

# install GCC/g++ compilers for C++
# ---------------------------------
RUN apt install g++ -y

# install GDAL
# ------------
RUN apt-get install gdal-bin -y
RUN apt-get install libgdal-dev -y

# Update C env vars so compiler can find gdal
# -------------------------------------------
ENV CPLUS_INCLUDE_PATH=/usr/include/gdal
ENV C_INCLUDE_PATH=/usr/include/gdal

# Add source files root path of docker container
# ----------------------------------------------
RUN mkdir src/
ADD src/Pansharpen.cpp src/
ADD src/Pansharpen.h src/
ADD src/Main.cpp src/
ADD src/Resample.cpp src/
ADD src/Resample.h src/
ADD src/GeotiffUtil.c src/
ADD src/GeotiffUtil.h src/
ADD makefile /

# compile the C/C++ code
# ----------------------
RUN make
ENTRYPOINT [ "bin/pansharpen" ]
