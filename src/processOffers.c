#include "processOffers.h"
#include "acceptOffer.h"

// make sure to accept offers before 5:00 PM on the same day

unsigned long hash(unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % MAP_SIZE;
}



void hashTableInit(int hashTable[], char* desiredLocations[]) {

  for (int i = 0; i < MAP_SIZE; i += 1) {
    hashTable[i] = -666;
  }

  for (int i = 0; i < 21; i += 1) {
    hashTable[hash(desiredLocations[i])] = DESIRED_LOCATION;
  }
}


// calls acceptOffer function on each iteration if conditions are met
void processOffers(short* tokenFlagPtr, char* siteToken, char* response, int hashTable[]) {
  
//   int hashTable[MAP_SIZE];

// // temporary
//  char* desiredLocations[] = {
        /*LIST OF LOCATIONS*/
//   };

//   hashTableInit(hashTable, desiredLocations);


  cJSON* json = cJSON_Parse(response);

  cJSON* offerList = cJSON_GetObjectItem(json, "offerList");
  cJSON* iterator = NULL;

  time_t startTime;
  cJSON* rateInfo;
  struct tm* t;

  char* offerId;

  unsigned short offerCount = 0;

  short acceptFlag;
  char* offerServiceArea;
  float rate;
  // FILE* currentOfferInfo = fopen("data/offerLogs/offerInfo|Aug-4-2023.txt", "a");

  cJSON_ArrayForEach(iterator, offerList) {
    
    offerCount += 1;

    // first check if offer is hidden
    //if (strcmp(jsonToString(cJSON_GetObjectItem(iterator, "hidden")), "true") == 0) continue;


    // next, check if location is a target lcoation (via hashmap/hashset)
    
    offerServiceArea = jsonToString(cJSON_GetObjectItem(iterator, "serviceAreaId"));
    //printf("%d\n", hashTable[hash(offerServiceArea)]);
    // fprintf(currentOfferInfo, "\n\n\n\n");
    // fputs(offerServiceArea, currentOfferInfo);
    if (hashTable[hash(offerServiceArea)] != DESIRED_LOCATION) continue;
    // printf("%s\n", offerServiceArea);



    // next, check if price is >= minimum desired price
    rateInfo = cJSON_GetObjectItem(iterator, "rateInfo");
    rate = strtof(jsonToString(cJSON_GetObjectItem(rateInfo, "priceAmount")), NULL);
    
    if ((rate < MIN_PRICE)) continue;


    // finally check if startTime and endTime satisfies time constraints; maybe just check startTime for now
    startTime = strtoul(jsonToString(cJSON_GetObjectItem(iterator, "startTime")), NULL, 0);
    t = gmtime(&startTime);

      // condition assumes 0 <= (t->tm_hour) <= 23
    if (!(((((t->tm_hour - CST_DIFF) % 24) + 24) % 24) >= START_TIME)) continue; 
    
    
    
    // if all of the above conditions are satisfied, extract the offerid
    offerId = jsonToString(cJSON_GetObjectItem(iterator, "offerId"));
    

    printf("\nOFFER CONDITIONS SATISFIED! ATTEMPTING TO ACCEPT\n");
    acceptFlag = acceptOffer(tokenFlagPtr, siteToken, offerId);

    if (acceptFlag) {

      printf("\n***Accepted Offer*** \n--------------------\nOffer Id: %s\nRate: $%0.2f\nLocationId: %s\nTime: %ld\n", offerId, rate, offerServiceArea, startTime);

    }
  }
}