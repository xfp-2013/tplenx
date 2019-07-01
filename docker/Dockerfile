FROM alpine:edge
MAINTAINER shin hung-jae <xfp.730700@gmail.com>

ARG MACHINE
ARG VERSION

LABEL machine="${MACHINE}" \
      version="${VERSION}"

ENV PCP_PORT=9898 \
	PCP_USERID=postgres \
	PCP_PASSWD=postgres \
	PGPOOL_PORT=5432 \
	PGPOOL_BACKENDS='postgres:5432:10' \
	TRUST_NETWORK='0.0.0.0/0' \
	PG_USERID=postgres \
	PG_PASSWD=postgres \
	NUM_INIT_CHILDREN=32 \
	MAX_POOL=4 \
	CHILD_LIFE_TIME=300 \
	CHILD_MAX_CONNECTIONS=0 \
	CONNECTION_LIFE_TIME=0 \
	CLIENT_IDLE_LIMIT=0

RUN apk add --no-cache alpine-sdk cmake git

WORKDIR /app

CMD [ "bash", "--login" ]
