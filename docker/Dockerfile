FROM alpine:edge
MAINTAINER shin hung-jae <xfp.730700@gmail.com>

ARG MACHINE
ARG VERSION

LABEL machine="${MACHINE}" \
      version="${VERSION}"

RUN apk add --no-cache alpine-sdk cmake git

COPY docker/etc /etc
COPY docker/home /home/app
COPY docker/home /root

RUN chown -R app:users /home/app

WORKDIR /app

CMD [ "bash", "--login" ]
