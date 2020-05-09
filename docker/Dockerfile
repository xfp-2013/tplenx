FROM alpine:edge
MAINTAINER shin hung-jae <xfp.730700@gmail.com>

ARG VERSION

LABEL version="${VERSION}"

RUN adduser -G users -s sh -S -D -H -u 1001 app; \
    apk add --no-cache alpine-sdk cmake git;

COPY docker/etc /etc
COPY docker/home /home/app
COPY docker/home /root

RUN chown -R app:users /home/app

WORKDIR /app

CMD [ "bash", "--login" ]
