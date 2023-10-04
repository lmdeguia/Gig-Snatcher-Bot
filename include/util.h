#ifndef MODIFY_REQUEST
#define MODIFY_REQUEST

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "cJSON.h"

#define GET_TOKEN_URL "https://api.some-website-api.com/auth/token"
#define GET_TOKEN_REQUEST_BODY "{\"token\": \"123456789\"}"


struct MemoryStruct{
  char* memory;
  size_t size;
  size_t address;
};

char* extractFileContents(const char* filename);
char* jsonToString(cJSON* json);

size_t WriteMemoryCallback(char *contents, size_t size, size_t nmemb, void *userp);

CURL* preparePostRequest(
  char* url,
  struct MemoryStruct buffer,
  struct curl_slist* requestHeaders, 
  char* requestBody
);

char* executeRequest(CURL* curl, struct MemoryStruct buffer);

// methods to delay each request
float randNum(float a, float b);
void delay(float seconds);

// get site-Date formatted as %Y%m%dT%H%M%SZ (e.g. 20230727T202723Z)
void formatDigit(char arr[], int value);
char* getFormattedDate();

// get new access token
struct curl_slist* getNewToken();
long extractNewToken();
char* retrieveCurrentToken();


#endif