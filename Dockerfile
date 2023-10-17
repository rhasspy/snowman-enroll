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
RUN mkdir -p dist && \
    cp -R resources/ build/apps/detect build/apps/enroll dist/ && \
    cd dist && \
    tar -czf "snowman_enroll-${TARGETARCH}${TARGETVARIANT}.tar.gz" \
        resources/ detect enroll

# -----------------------------------------------------------------------------

FROM scratch

COPY --from=build /snowman/dist/*.tar.gz ./
