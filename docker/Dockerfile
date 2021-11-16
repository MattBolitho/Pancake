FROM debian:bullseye
ENV CXX=g++
RUN apt update && apt upgrade -y && apt install -y git gcc g++ cmake make
RUN git clone https://github.com/MattBolitho/Pancake.git && cd Pancake && mkdir build && cd build && cmake .. && make
RUN mv Pancake/build/pancake /bin && rm -rf Pancake