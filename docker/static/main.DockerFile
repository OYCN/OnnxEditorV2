ARG DOCKER_3RD_PREFIX
ARG DOCKER_BASE_IMAGE

FROM ${DOCKER_3RD_PREFIX}:nlohmann-json AS nlohmann-json
FROM ${DOCKER_3RD_PREFIX}:magic_enum AS magic_enum
FROM ${DOCKER_3RD_PREFIX}:gflag AS gflag
FROM ${DOCKER_3RD_PREFIX}:gtest AS gtest
FROM ${DOCKER_3RD_PREFIX}:glog AS glog
FROM ${DOCKER_3RD_PREFIX}:graphviz AS graphviz
FROM ${DOCKER_3RD_PREFIX}:ogdf AS ogdf
FROM ${DOCKER_3RD_PREFIX}:onnx AS onnx
FROM ${DOCKER_3RD_PREFIX}:qt AS qt

FROM ${DOCKER_BASE_IMAGE}

COPY --from=nlohmann-json /nlohmann-json /nlohmann-json
COPY --from=magic_enum /magic_enum /magic_enum
COPY --from=gflag /gflag /gflag
COPY --from=gtest /gtest /gtest
COPY --from=glog /glog /glog
COPY --from=graphviz /graphviz /graphviz
COPY --from=ogdf /ogdf /ogdf
COPY --from=onnx /onnx /onnx
COPY --from=qt /qt /qt

ARG DOCKER_ENV_SCRIPT

COPY ${DOCKER_ENV_SCRIPT} /script.sh
RUN  /script.sh && rm -rf /script.sh

WORKDIR /ws
