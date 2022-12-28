ARG DOCKER_BASE_IMAGE

# common layer
FROM ${DOCKER_BASE_IMAGE} AS common

ARG DOCKER_ENV_SCRIPT

COPY ${DOCKER_ENV_SCRIPT} /script.sh
RUN  /script.sh && rm -rf /script.sh

# build qt libs
FROM common AS qtlib

COPY 3rd_party/qtbase /qtbase
ENV QT_DIR="/qtbase"
ENV QT_DST="/qtlib"
COPY ./script/build_qt.sh /script.sh
RUN /script.sh && rm -rf /script.sh

# build workspace
FROM common

WORKDIR /ws
COPY --from=qtlib /qtlib /qtlib
ENV QT_LIB /qtlib
ENV STATIC "on"
