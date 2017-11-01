/*
 * response_header.c
 * Copyright (C) 2017 c9s <c9s@c9smba.local>
 *
 * Distributed under terms of the MIT license.
 */
#include <stdio.h>
#include <stdlib.h>
//#include <stdbool.h>
//#include <stdint.h>
#include <ctype.h>
#include <string.h>

#include "..\include\h3.h"
#include "scanner.h"

ResponseHeader* h3_response_header_new() {
	ResponseHeader *h = malloc(sizeof(ResponseHeader));
	h->ResponseLineStart = NULL;
	h->ResponseLineEnd = NULL;
	h->HTTPVersion = NULL;
	h->HTTPVersionLen = 0;
	h->ResponseStatusCode = NULL;
	h->ResponseStatusCodeLen = 0;
    h->HTTPVersion = NULL;;
    h->HTTPVersionLen = 0;
	h->ResponseStatusDescription = NULL;
	h->ResponseStatusDescriptionLen = 0;
	h->HeadList = NULL;
    return h;
}

void h3_response_header_free(ResponseHeader *header) {
	h3_header_field_list_free(header->HeadList);
    free(header);
}


/**
 * This function returns a char pointer, which is the end of the request line.
 *
 * Return NULL if parse failed.
 */
const char * h3_response_line_parse(ResponseHeader *header, const char *body, int bodyLength) {
    // Parse the request-line
    // http://tools.ietf.org/html/rfc2616#section-5.1
    // Request-Line   = Method SP Request-URI SP HTTP-Version CRLF
    const char * p = body;
    header->ResponseLineStart = body;

	while (notend(p) && !isspace((unsigned char)*p)) p++;
	if (strncmp(body, "HTTP", 4))
	{
		return NULL;
	}

	// Skip space and parse HTTP-Version
	header->HTTPVersion = body;
	header->HTTPVersionLen = p - body;
    // Skip space
    // parse Status Code
	while (isspace((unsigned char)*p) && notcrlf(p) && notend(p)) p++;
    header->ResponseStatusCode = p; 
	while (!isspace((unsigned char)*p) && notcrlf(p) && notend(p)) p++;
	header->ResponseStatusCodeLen = p - header->ResponseStatusCode;

	// Skip space
	// parse Status Description
	while (isspace((unsigned char)*p) && notcrlf(p) && notend(p)) p++;
	header->ResponseStatusDescription = p;
	while (!isspace((unsigned char)*p) && notcrlf(p) && notend(p)) p++;
	header->ResponseStatusDescriptionLen = p - header->ResponseStatusDescription;

    return p;
}



/**
 * Parse header body
 */
int h3_response_header_parse(ResponseHeader *header, const char *body, int bodyLength) {
	int i = 0;
	const char *p = h3_response_line_parse(header, body, bodyLength);

    if (p == NULL) {
        return H3_ERR_REQUEST_LINE_PARSE_FAIL;
    }

    // should be ended with CR-LF
    if (end(p)) return -1;

    // skip CR-LF
    iscrlf(p); p+=2;
    if (end(p)) return 0;

	header->HeadList = h3_header_field_list_new(2);
    // Parse Header Fields Here
    do {
		HeaderField *field = h3_header_field_new();
        // HeaderField *field = h3_header_field_new();
        field->FieldName = p; // start of a header field name

        while(notend(p) && *p != ':' ) p++;
        field->FieldNameLen = p - field->FieldName;
        p++; // skip ':'

        // CRLF is not allowed here
        if (end(p) || iscrlf(p)) return -1;


		while (notend(p) && isspace((unsigned char)*p)) p++; // skip space

        // CRLF is not allowed here
        if (end(p) || iscrlf(p)) return -1;

        field->Value = p;
        while(notend(p) && notcrlf(p) ) p++;
        field->ValueLen = p - field->Value;

        iscrlf(p); p+=2;

		h3_header_field_list_append(header->HeadList, field);
#ifdef DEBUG
        printf("==> %.*s ==> %.*s\n", field->FieldNameLen, field->FieldName, field->ValueLen, field->Value );
#endif
        // end of header
        if (iscrlf(p)) return 0;
    } while( notend(p) );

    return 0;
}
