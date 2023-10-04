#include <stdio.h>
#include "acceptOffer.h"
#include "getOffers.h"
#include "processOffers.h"
#include "util.h"

#define REQUEST_LIMIT 900
#define REQUEST_FAIL_DELAY 60

int main(){

  srand(time(NULL));

  /*INITIATE HASH TABLE FOR PROCESSING OFFERS*/

  int hashTable[MAP_SIZE];

 char* desiredLocations[] = {
    "\"12345678-abcd-12ab-123a-123456789abc\""
  };

  hashTableInit(hashTable, desiredLocations);
  /******************************************/



  char* getOffersURL;


  struct MemoryStruct getOffersBuffer;

  char* getOffersRequestBody;  
  getOffersURL = GET_OFFERS_URL;

  getOffersRequestBody = GET_OFFERS_REQUEST_BODY;
  


  long httpStatus;
  
  short STOP = 0;

  short tokenFlag = 1;
  short* tokenFlagPtr = &tokenFlag;

  float delayTime;
  short tmin, tmax;

  tmin = 0.5; tmax = 3;

  char* fileBuffer;

  char siteToken[600];
  char* siteDate;

  struct curl_slist* getOffersRequestHeaders;

  CURL* curlGetOffers;

  char* response;

  int requestCount = 0;

  FILE* currentLogFile = fopen("data/offerLogs/log|Aug-4-2023.txt", "a");

  printf("PREPARING TO GET THE BEST OFFERS!!!\n");
  printf("---------------------------------\n\n");

  // begin getOffers loop
  while(!STOP) {

    getOffersBuffer.memory = malloc(1);
    getOffersBuffer.size = 0;
    getOffersBuffer.address = 420;

    if (tokenFlag) {
      fileBuffer = retrieveCurrentToken();
      sprintf(siteToken, "site-access-token: %s", fileBuffer);
      tokenFlag = 0;
    }


    // ....
    //   GET OFFERS ROUTINE
    // ....

    siteDate = getFormattedDate();

    getOffersRequestHeaders = offerRequestHeaders(siteDate, siteToken);
    curlGetOffers = preparePostRequest(getOffersURL, getOffersBuffer, getOffersRequestHeaders, getOffersRequestBody);

    response = executeRequest(curlGetOffers, getOffersBuffer);

    if (response == "\0") {
      printf("REQUEST ERROR. DELAYING FOR %d SECONDS\n", REQUEST_FAIL_DELAY);
      delay(REQUEST_FAIL_DELAY);
      printf("OKAY. RESTARTING!\n");
    }

    curl_easy_getinfo(curlGetOffers, CURLINFO_RESPONSE_CODE, &httpStatus);

    //printf("\nRESPONSE CODE: %ld\n", httpStatus);

    if (httpStatus == 403) {

      printf("\nCURRENT ACCESS TOKEN EXPIRED. ATTEMPTING TO RETRIEVE NEW TOKEN...\n\n");

      long tokenStatus = extractNewToken();

      if (tokenStatus == 200) {
        printf("NEW TOKEN RETRIEVAL SUCCESS\n");
        tokenFlag = 1;
      } else {
        // temporary handle
        printf("ERROR: UNABLE TO RETRIEVE NEW TOKEN. \n TERMINATING GIGBOT\n\n");
        return 1;
      }



    } else if (httpStatus == 200) {

      processOffers(tokenFlagPtr, siteToken, response, hashTable);
      // fprintf(currentLogFile, "\n\n\n\n");
      // fputs(response, currentLogFile);

    } else {
      // ....
      //   HANDLES FOR OTHER STATUS CODES
      // ....
      continue;
    }

    requestCount += 1;

    if (requestCount > REQUEST_LIMIT) {
      printf("REQUEST LIMIT REACHED: STOPPING BEFORE <corporation name that shall not be named> GETS ALL SUS\n\n");
      STOP = 1;
    }

    delayTime = randNum(tmin, tmax);
    delay(delayTime);
  }



  // ....
  //   MEMORY CLEAN ROUTINE
  // ....

  curl_slist_free_all(getOffersRequestHeaders);
  curl_easy_cleanup(curlGetOffers);
  fclose(currentLogFile);
  //free(getOffersBuffer.memory);

  return 0;
}