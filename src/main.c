#include <stdio.h>
#include "http_post.h"

static int OnData(void* ctx, const char* data, int len) 
{
    fwrite(data, 1, len, stdout);
    return 0;
}

int main(void)
{
    HTTPPost* post = NULL;
    HTTPPost_InitiatePtr(&post);

    const char* host = "httpbin.org";
    const char* port = "80";
    const char* path = "/post";
    const char* json = "{ \"device\": \"sensor01\", \"time\": \"2025-10-30T12:00:00Z\", \"temperature\": \"22.5C\" }";

    HTTPPost_SetCallback(post, NULL, OnData);
    HTTPPost_Send(post, host, port, path, json);

    HTTPPost_DisposePtr(&post);
    return 0;
}
