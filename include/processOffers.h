#ifndef PROCESS_OFFERS
#define PROCESS_OFFERS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "util.h"
#include "cJSON.h"

#define START_TIME 17
#define CST_DIFF 5 // change to "6" on November 5, 2023

#define MIN_PRICE 90
#define MAP_SIZE 550
#define DESIRED_LOCATION 69

#ifndef SAMPLE_GETOFFERS_RESPONSE_NONEMPTY
  #define SAMPLE_GETOFFERS_RESPONSE_NONEMPTY "{\"sample\" : \"empty\"}"
#endif

#ifndef SAMPLE_GETOFFERS_RESPONSE_EMPTY
  #define SAMPLE_GETOFFERS_RESPONSE_EMPTY "{\"sample\" : \"not empty\"}"
#endif

unsigned long hash(unsigned char *str);
void hashTableInit(int hashTable[], char* desiredLocations[]);

void processOffers(short* tokenFlagPtr, char* siteToken, char* response, int hashTable[]);

#endif