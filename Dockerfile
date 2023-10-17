FROM debian:bullseye as build
ARG TARGETARCH
ARG TARGETVARIANT

ENV LANG C.UTF-8
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install --yes --no-install-recommends \
        build-essential cmake \
        libatlas-base-dev libssl-dev

WORKDIR /snowman

COPY ./ ./

RUN make

# -----------------------------------------------------------------------------

FROM scratch

COPY --from=build /snowman/resources/ ./resources/
COPY --from=build /snowman/build/apps/detect /snowman/build/apps/enroll ./
