#include "util.h"

size_t WriteMemoryCallback(char *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  char *ptr = realloc(mem->memory, mem->size + realsize + 1);

  if(ptr == NULL) {
    printf("error: not enough memory\n");
    return 0;
  }

  // printf("%s\n", contents);
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

char* extractFileContents(const char* filename) {
  // https://stackoverflow.com/questions/174531/how-to-read-the-content-of-a-file-to-a-string-in-c
  FILE* file = fopen(filename, "rb");

  char * buffer = 0;
  long length;
  if (file)
  {
    fseek (file, 0, SEEK_END);
    length = ftell (file);
    fseek (file, 0, SEEK_SET);
    buffer = malloc (length);
    if (buffer)
    {
      fread (buffer, 1, length, file);
    }
    fclose (file);
  }
  return buffer;
}

CURL* preparePostRequest(
  char* url,
  struct MemoryStruct buffer,
  struct curl_slist* requestHeaders,  
  char* requestBody
){

  CURL* curl;

  curl = curl_easy_init();

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_1_1);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    //curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&buffer);
    

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, requestHeaders);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestBody);

  }


  return curl;
}

void modifyRequestHeaders(CURL* curl, struct curl_slist* newRequestHeaders) {
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, newRequestHeaders);
}

char* executeRequest(CURL* curl, struct MemoryStruct buffer){
  char* response;
  CURLcode res = curl_easy_perform(curl);
  if (!(res == CURLE_OK)) {
    printf("REQUEST ERROR OCCURRED!\n");
    printf("error: %s\n", curl_easy_strerror(res));
    response = "\0";
    return response;
  } else {
    response = buffer.memory;
  }
  return response;
}


float randNum(float a, float b) {
  return ((b - a) / RAND_MAX)*rand() + a;
}

void delay(float seconds) {
  time_t start = time(NULL);
  float timeElapsed = ((float)(time(NULL) - start));

  while (timeElapsed < seconds - 1){
    timeElapsed = ((float)(time(NULL) - start));
  }  
}

void formatDigit(char arr[], int value) {
  if (value < 10) {
    sprintf(arr, "0%d", value);
  } else {
    sprintf(arr, "%d", value);
  }
  //arr[2] = '\0';
}

char* getFormattedDate() {
  //%Y%m%dT%H%M%SZ

  time_t now = time(NULL);

  struct tm* t = gmtime(&now);

  int _year = t->tm_year + 1900;
  int _month = t->tm_mon + 1;
  int _day = t->tm_mday;
  int _hour = t->tm_hour;
  int _minute = t->tm_min;
  int _second = t->tm_sec;

  char year[5];
  char month[4];
  char day[4];
  char hour[4];
  char minute[4];
  char second[4];

  // NOT THREAD SAFE
  static char formattedTime[40]; 


  sprintf(year, "%d", _year);

  formatDigit(month, _month);
  formatDigit(day, _day);
  formatDigit(hour, _hour);
  formatDigit(minute, _minute);
  formatDigit(second, _second);


  sprintf(formattedTime, "X-Amz-Date: %s%s%sT%s%s%sZ", year, month, day, hour, minute, second);

  return formattedTime;
}

char* retrieveCurrentToken() {
  FILE* readTokenFile = fopen("data/siteToken.txt", "rb");

  // NOT THREAD SAFE
  static char fileBuffer[500];

  char ch; int idx  = 0;

  while(ch != EOF) {
    ch = fgetc(readTokenFile);
    if (ch != '\"') {
      fileBuffer[idx] = ch;
      idx += 1;
    } 
  }
  fileBuffer[idx-1] = '\0';
  fclose(readTokenFile);
  return fileBuffer;
}

char* jsonToString(cJSON* json) {
  return cJSON_Print(json);
}

struct curl_slist* getNewToken(){

  struct curl_slist* requestHeaders = NULL;

  requestHeaders = curl_slist_append(requestHeaders, "User-Agent: Lendel the Handsome");
  requestHeaders = curl_slist_append(requestHeaders, "Accept-Encoding: gzip, deflate");
  requestHeaders = curl_slist_append(requestHeaders, "Accept: */*");
  requestHeaders = curl_slist_append(requestHeaders, "Connection: keep-alive");
  requestHeaders = curl_slist_append(requestHeaders, "auth-domain: api.website.com");
  requestHeaders = curl_slist_append(requestHeaders, "Content-Type: application/json");
    
  return requestHeaders;
}


long extractNewToken() {

  // execute if response for other requests is 403
  char* getTokenURL;
  char* getTokenRequestBody;

  static struct MemoryStruct getNewTokenBuffer;
  getNewTokenBuffer.memory = malloc(1);
  getNewTokenBuffer.size = 0;
  getNewTokenBuffer.address = 69;

  getTokenURL = GET_TOKEN_URL;

  getTokenRequestBody = GET_TOKEN_REQUEST_BODY; 

  struct curl_slist* getTokenRequestHeaders = getNewToken();

  CURL* curlGetNewToken = preparePostRequest(getTokenURL, getNewTokenBuffer, getTokenRequestHeaders, getTokenRequestBody);
  char* response = executeRequest(curlGetNewToken, getNewTokenBuffer);


  long httpStatus;
  curl_easy_getinfo(curlGetNewToken, CURLINFO_RESPONSE_CODE, &httpStatus);


  if (httpStatus == 200) {
    printf("NEW TOKEN ACQUIRED. STORING IN TEXT FILE...");
    
    cJSON* json = cJSON_Parse(response);
    cJSON* accessTokenJson = cJSON_GetObjectItem(json, "access_token");
    char* accessToken = jsonToString(accessTokenJson);
    //char* accessToken = "DEEZNUTS";

  
    FILE* tokenFile = fopen("data/siteToken.txt", "wb");
    fputs(accessToken, tokenFile);

    curl_slist_free_all(getTokenRequestHeaders);
    curl_easy_cleanup(curlGetNewToken);
    free(getNewTokenBuffer.memory);
    fclose(tokenFile);
  }

  return httpStatus;
}