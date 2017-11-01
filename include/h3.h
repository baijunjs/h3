/*
 * h3.h
 * Copyright (C) 2014 c9s <c9s@c9smba.local>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef H3_H
#define H3_H

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// CRLF string for readibility
//#define CRLF "\r\n"
#define MAX_HEADER_SIZE 20

enum H3_ERROR { 
    H3_ERR_REQUEST_LINE_PARSE_FAIL = -1,
    H3_ERR_INCOMPLETE_HEADER,
    H3_ERR_UNEXPECTED_CHAR,
};

typedef struct _HeaderField HeaderField;
typedef struct _HeaderFieldList HeaderFieldList;

/*
    Host: github.com
    ^     ^
    |     |
    |     Value (ValueLen = 10)
    |
    | FieldName, FieldNameLen = 4
*/
struct _HeaderField {
    const char *FieldName;
    int         FieldNameLen;

    const char *Value;
    int  ValueLen;
};

typedef struct  {
    /**
     * Pointer to start of the request line.
     */
    const char * RequestLineStart;

    /**
     * Pointer to the end of the request line 
     */
    const char * RequestLineEnd;

    /**
     * Pointer to the start of the request method string
     */
    const char * RequestMethod;

    int    RequestMethodLen;

    const char * RequestURI;
    int RequestURILen;

    const char * HTTPVersion;
    int HTTPVersionLen;

	HeaderFieldList *HeadList;

} RequestHeader;


typedef struct  {
	/**
	* Pointer to start of the response line.
	*/
	const char * ResponseLineStart;

	/**
	* Pointer to the end of the response line
	*/
	const char * ResponseLineEnd;

	const char * HTTPVersion;
	int HTTPVersionLen;

	/**
	* Pointer to the end of the http status code
	*/
	const char * ResponseStatusCode;
	int ResponseStatusCodeLen;

	/**
	* Pointer to the end of the http status description
	*/
	const char * ResponseStatusDescription;
	int ResponseStatusDescriptionLen;

	HeaderFieldList *HeadList;

} ResponseHeader;

struct _HeaderFieldList {
    HeaderField ** Fields;
    int cap;
    int len;
};

#ifdef __cplusplus
extern "C"
{
#endif

	const char * h3_request_line_parse(RequestHeader *header, const char *body, int bodyLength);
	const char * h3_response_line_parse(ResponseHeader *header, const char *body, int bodyLength);


	/**
	* Request Header function prototypes
	*/
	RequestHeader* h3_request_header_new();

	/**
	* Response Header function prototypes
	*/
	ResponseHeader* h3_response_header_new();

	void h3_request_header_free(RequestHeader *header);
	void h3_response_header_free(ResponseHeader *header);

	int h3_request_header_parse(RequestHeader *header, const char *body, int bodyLength);
	int h3_response_header_parse(ResponseHeader *header, const char *body, int bodyLength);

	/**
	* Request Header Field List function prorotypes
	*/
	HeaderFieldList * h3_header_field_list_new(int cap);

	void h3_header_field_list_resize(HeaderFieldList *fields, int newcap);

	void h3_header_field_list_append(HeaderFieldList *fields, HeaderField *field);

	HeaderField *h3_header_field_lookup(HeaderFieldList *fields, const char* sFieldName, int ignorecase);

	void h3_header_field_list_free(HeaderFieldList *fields);

	/**
	* Header Field function prototypes
	*/
	HeaderField * h3_header_field_new();

	void h3_header_field_free(HeaderField * field);


#ifdef __cplusplus
}
#endif

#define h3_header_field_list_get(fields, idx) fields->Fields[idx]

#define h3_header_field_list_size(fields) fields->len

#define h3_header_field_list_cap(fields) fields->cap

/**
 * Predefined string and constants
 */
#define H3_DEFAULT_HTTP_VERSION "HTTP/1.0"


#endif /* !H3_H */
