/*
 * header_field_list.c
 * Copyright (C) 2014 c9s <c9s@c9smba.local>
 *
 * Distributed under terms of the MIT license.
 */
#include <stdlib.h>
#include "..\include\h3.h"


HeaderFieldList * h3_header_field_list_new(int cap) {
    HeaderFieldList *fields = malloc(sizeof(HeaderFieldList));
    fields->cap = cap;
    fields->len = 0;
    fields->Fields = NULL;

    if (cap > 0) {
        fields->Fields = malloc(sizeof(HeaderField*) * cap);
		memset(fields->Fields, 0, sizeof(HeaderField*) * cap);
    }
    return fields;
}

void h3_header_field_list_resize(HeaderFieldList *fields, int newcap) {
    fields->cap = newcap;
    fields->Fields = realloc(fields->Fields, sizeof(HeaderField*) * newcap);

}


void h3_header_field_list_append(HeaderFieldList *fields, HeaderField *field) {
    if (fields->len + 1 > fields->cap) {
        h3_header_field_list_resize(fields, fields->cap * 2);
    }
    fields->Fields[ fields->len++ ] = field;
}

HeaderField *h3_header_field_lookup(HeaderFieldList *fields, const char* sFieldName, int ignorecase)
{
	int i = 0;
	for (i = 0; i < h3_header_field_list_size(fields); ++i)
	{
		HeaderField *pField = h3_header_field_list_get(fields, i);
		if (1 == ignorecase)
		{
			if (0 == _strnicmp(sFieldName, pField->FieldName, pField->FieldNameLen))
			{
				return pField;
			}
		}
		else
		{
			if (0 == strncmp(sFieldName, pField->FieldName, pField->FieldNameLen))
			{
				return pField;
			}
		}
	}
	return NULL;
}


#define h3_header_field_list_get(fields, idx) fields->Fields[idx]

#define h3_header_field_list_size(fields) fields->len

#define h3_header_field_list_cap(fields) fields->cap

void h3_header_field_list_free(HeaderFieldList *fields) {
	int i;
    if (fields->Fields) {

		for (i = 0; i < fields->len; ++i)
			free(fields->Fields[i]);

        free(fields->Fields);
    }
    free(fields);
}



