#ifndef GET_OFFERS
#define GET_OFFERS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "util.h"

#define GET_OFFERS_URL "https://some-website-api.com/GetOffers"
#define GET_TOKEN_URL "https://api.some-website-api.com/auth/token"

#define REFRESH_TOKEN "rfrsh|123456789"

#define GET_OFFERS_REQUEST_BODY "{\"info\" : \"stuff\"}"
#define GET_TOKEN_REQUEST_BODY "{\"token\" : \"123456789\"}"


struct curl_slist* offerRequestHeaders(char* siteDate, char* siteAccessToken);

#endif