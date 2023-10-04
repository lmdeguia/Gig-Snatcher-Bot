#include "getOffers.h"
#include "processOffers.h"


struct curl_slist* offerRequestHeaders(char* siteDate, char* siteAccessToken){

  struct curl_slist* requestHeaders = NULL;

  requestHeaders = curl_slist_append(requestHeaders, "User-Agent: Lendel the Handsome");
  requestHeaders = curl_slist_append(requestHeaders, "Accept-Encoding: gzip, deflate, br");
  requestHeaders = curl_slist_append(requestHeaders, "Accept: application/json");
  requestHeaders = curl_slist_append(requestHeaders, "Connection: keep-alive");

  requestHeaders = curl_slist_append(requestHeaders, siteAccessToken);
  requestHeaders = curl_slist_append(requestHeaders, "Authorization: SITE-HMAC-SHA256 SignedHeaders=site-access-token;site-date, Signature=aaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbcccccccccccc");
  requestHeaders = curl_slist_append(requestHeaders, siteDate);
  requestHeaders = curl_slist_append(requestHeaders, "site-id: ABCDEFGH-1234-5678-ABC1-123456789123");
  requestHeaders = curl_slist_append(requestHeaders, "Accept-Language: en-US");
  requestHeaders = curl_slist_append(requestHeaders, "Content-Type: application/json");
  requestHeaders = curl_slist_append(requestHeaders, "Cookie: session-id=123-4567891-1234567; session-id-time=1023456789q; session-token=ttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttooooooooooooooooo/kkkkkkkkkkkkkkkk/eeeeeeeeeeeeeeeeee+nnnnnnnnnnn/ttttttttttttttttttttttt+oooooooooooooooo+kk+eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeen; at-main=\"abcd|toooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo_kkkkkkkkkkkkkk-eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeennnnnnnnnnnnn\"; sess-at-main=\"sseeeessssss+eeeeees+ssssssssssssssssssssss=\"; ubid-main=123-4567891-2345678; x-main=\"xxxxxxxxxxxxxxmmmmmmmmmmmmmmmmmmmmmaaaaaaaaaaaaaaiiiiiiiiiiiiiin\"");
  return requestHeaders;
}

char* extractToken(char* response) {
  cJSON* json = cJSON_Parse(response);
  cJSON* accessTokenJson = cJSON_GetObjectItem(json, "access_token");
  char* accessToken = jsonToString(accessTokenJson);
  return accessToken;
}

