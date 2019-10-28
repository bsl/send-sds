FROM	ubuntu
RUN	apt-get update
RUN	apt-get install -qq -y make gcc libasound2-dev libsndfile1-dev
RUN	mkdir /src
COPY	. /src
WORKDIR /src
RUN	make && install send-sds /usr/local/bin
