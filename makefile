default: run

build:clean
	@gcc -g -o gigbot main.c src/util.c src/getOffers.c src/processOffers.c src/acceptOffer.c cJSON/cJSON.c -l curl -I include -I cJSON

clean:
	@rm gigbot

run: build
	@./gigbot