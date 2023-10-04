#ifndef ACCEPT_OFFER
#define ACCEPT_OFFER

#define ACCEPT_OFFER_URL "https://some-website-api.com/AcceptOffer"

char* makeRequestBody(char* offerId);

short acceptOffer(short* tokenFlagPtr, char* siteToken, char* offerId);

#endif


