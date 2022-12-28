ARG DOCKER_BASE_IMAGE

# common layer
FROM ${DOCKER_BASE_IMAGE}

ARG DOCKER_ENV_SCRIPT

COPY ${DOCKER_ENV_SCRIPT} /script.sh
RUN  /script.sh && rm -rf /script.sh

WORKDIR /ws
ENV QT_LIB ""
