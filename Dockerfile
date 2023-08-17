FROM debian:bullseye-slim

ENV TZ="Asia/Tokyo"

ARG USER_ID="10000"
ARG GROUP_ID="10001"
ARG USER_NAME="user"

RUN groupadd -g "${GROUP_ID}" "${USER_NAME}" && \
  useradd -l -u "${USER_ID}" -m "${USER_NAME}" -g "${USER_NAME}"

WORKDIR /home/user/app

RUN apt update && apt install -y \
  wget \
  build-essential \
  cmake \
  tzdata

COPY ./src /home/user/app/src/
COPY CMakeLists.txt build.sh run.sh ./
