# Base SDK Image
FROM ubuntu:latest AS sdk

# Install dependencies
RUN apt update && apt install -y build-essential git

# Install SDK
RUN git clone https://github.com/home-bru/ps4-payload-sdk ~/ps4-payload-sdk &&\
 cd ~/ps4-payload-sdk &&\
 chmod +x install.sh &&\
 ./install.sh


# Builder Image
FROM sdk AS build

WORKDIR /app
COPY . /app
ENV PS4SDK=/opt/ps4sdk
RUN make

# Exporter Image
FROM scratch
COPY --from=build /app/app.bin /
ENTRYPOINT ["/app.bin"]