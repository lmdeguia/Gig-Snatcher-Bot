#include "acceptOffer.h"
#include "getOffers.h"
#include "util.h"

char* makeRequestBody(char* offerId) {
  // NOT THREAD SAFE
  static char requestBody[1000];
  sprintf(requestBody, "{\"offerId\": %s}", offerId);
  return requestBody;

}

short acceptOffer(short* tokenFlagPtr, char* siteToken, char* offerId) {

  char* acceptOfferURL;
  char* acceptOfferRequestBody;
  short acceptFlag = 0;

  static struct MemoryStruct acceptOfferBuffer;
  acceptOfferBuffer.memory = malloc(1);
  acceptOfferBuffer.size = 0;
  acceptOfferBuffer.address = 69;

  struct curl_slist* acceptOfferRequestHeaders;
  CURL* curlacceptOffer;
  char* response;

  long httpStatus;

  acceptOfferURL = ACCEPT_OFFER_URL;
  acceptOfferRequestBody = makeRequestBody(offerId);

  char* accessToken = siteToken;

  while(!acceptFlag) {

    acceptOfferRequestHeaders = offerRequestHeaders(accessToken, getFormattedDate());
    curlacceptOffer = preparePostRequest(acceptOfferURL, acceptOfferBuffer, acceptOfferRequestHeaders, acceptOfferRequestBody);
    response = executeRequest(curlacceptOffer, acceptOfferBuffer);

    curl_easy_getinfo(curlacceptOffer, CURLINFO_RESPONSE_CODE, &httpStatus);

    if (httpStatus == 200) {
      printf("\n***Accepted Offer*** \n--------------------\n%s\n\n", acceptOfferRequestBody);
      acceptFlag = 1;
    } else if (httpStatus == 403) {
      printf("\nCURRENT ACCESS TOKEN EXPIRED. ATTEMPTING TO RETRIEVE NEW TOKEN...\n\n");

      long tokenStatus = extractNewToken();

      if (tokenStatus == 200) {

        accessToken = retrieveCurrentToken();
        *tokenFlagPtr = 1;

      } else {
        // temporary handle
        printf("ERROR: UNABLE TO RETRIEVE NEW TOKEN. ABORTING \"ACCEPT OFFER\" ROUTINE\n\n");
        return 0;
      }
    } else {
      printf("\nATTEMPTED TO ACCEPT OFFER BUT FAILED. HTTP STATUS %ld\n\n", httpStatus);
      return 0;
    }


  }

  curl_slist_free_all(acceptOfferRequestHeaders);
  curl_easy_cleanup(curlacceptOffer);
  //free(acceptOfferBuffer.memory);
  return acceptFlag;
}


