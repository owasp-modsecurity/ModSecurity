/*
* ModSecurity for Apache 2.x, http://www.modsecurity.org/
* Copyright (c) 2004-2011 Trustwave Holdings, Inc. (http://www.trustwave.com/)
*
* You may not use this file except in compliance with
* the License.  You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* If any of the files related to licensing are missing or if you have any
* other questions related to licensing please contact Trustwave Holdings, Inc.
* directly using the email address security@modsecurity.org.
*/

//#undef inline
#define	inline inline

#include <stdio.h>
#include <conio.h>
#include "api.h"


char *config_file = NULL;
char *url_file = NULL;
char *event_files[1024];
int event_file_cnt;
char *event_file = NULL;
int event_file_len = 0;
char **event_file_lines;
int event_line_cnt = 0;
int event_file_blocks[256];

#define	EVENT_FILE_MAX_SIZE		(16*1024*1024)

#define	MAX_URLS	4096

char urls[MAX_URLS][4096];
int url_cnt = 0;

void readeventfile(char *name)
{
	if(event_file == NULL)
	{
		event_file = (char *)malloc(EVENT_FILE_MAX_SIZE);
		event_file_lines = (char **)malloc(EVENT_FILE_MAX_SIZE);
	}

	event_file_len = 0;
	event_line_cnt = 0;
	memset(event_file_blocks, -1, sizeof(int) * 256);

	FILE *fr = fopen(name, "rb");

	if(fr == NULL)
		return;

	event_file_len = fread(event_file, 1, EVENT_FILE_MAX_SIZE - 1, fr);

	fclose(fr);

	event_file[event_file_len] = 0;
}

void parseeventfile()
{
	if(event_file_len == 0 || event_file == NULL)
		return;

	char *t = event_file;
	char *e = event_file + event_file_len;
	int nocrlf = 1;

	while(t < e)
	{
		event_file_lines[event_line_cnt++] = t;

		while(t < e && *t != 10 && *t != 13)
			t++;

		char ct = *t;
		*t = 0;
		int i = event_line_cnt - 1;

		int l = strlen(event_file_lines[i]);

		if(l == 14 && event_file_lines[i][0] == '-' && event_file_lines[i][1] == '-' && event_file_lines[i][l-2] == '-' && event_file_lines[i][l-1] == '-')
		{
			char blk =  event_file_lines[i][l-3];

			event_file_blocks[blk] = i;

			if(blk == 'C' || blk == 'G')
			{
				nocrlf = 0;
			}
			else
			{
				nocrlf = 1;
			}
		}
		*t = ct;

		if(nocrlf)
			while(t < e && (*t == 10 || *t == 13))
				*t++ = 0;
		else
			while(t < e && (*t == 10 || *t == 13))
				t++;
	}
}

void parseargs(int argc, char *argv[])
{
	int i = 1;

	event_file_cnt = 0;

	while(i < argc)
	{
		if(argv[i][0] == '-')
		{
			if(argv[i][1] == 'c' && i < argc - 1)
			{
				config_file = argv[i + 1];
				i += 2;
				continue;
			}
			if(argv[i][1] == 'u' && i < argc - 1)
			{
				url_file = argv[i + 1];
				i += 2;
				continue;
			}
			i++;
			continue;
		}
		if(event_file_cnt == 1024)
		{
			fprintf(stderr, "Too many input files! (limit 1024)\n");
			break;
		}

		event_files[event_file_cnt++] = argv[i++];
	}
}

void log(void *obj, int level, char *str)
{
	printf("%s\n", str);
}

unsigned int bodypos = 0;

apr_status_t readbody(request_rec *r, char *buf, unsigned int length, unsigned int *readcnt, int *is_eos)
{
	int j = event_file_blocks['C'];

	if(j < 0)
	{
		*is_eos = 1;
		return APR_SUCCESS;
	}

	j++;

	if(event_file_lines[j][0] == 0)
	{
		*is_eos = 1;
		return APR_SUCCESS;
	}

	unsigned int l = strlen(event_file_lines[j]);
	unsigned int size = length;

	if(bodypos + size > l)
		size = l - bodypos;

	memcpy(buf, &event_file_lines[j][bodypos], size);

	bodypos += size;
	*readcnt = size;

	if(bodypos == l)
	{
		*is_eos = 1;
	}

	return APR_SUCCESS;
}

unsigned int responsepos = 0;

apr_status_t readresponse(request_rec *r, char *buf, unsigned int length, unsigned int *readcnt, int *is_eos)
{
	int j = event_file_blocks['G'];

	if(j < 0)
	{
		*is_eos = 1;
		return APR_SUCCESS;
	}

	j++;

	if(event_file_lines[j][0] == 0)
	{
		*is_eos = 1;
		return APR_SUCCESS;
	}

	unsigned int l = strlen(event_file_lines[j]);
	unsigned int size = length;

	if(responsepos + size > l)
		size = l - responsepos;

	memcpy(buf, &event_file_lines[j][responsepos], size);

	responsepos += size;
	*readcnt = size;

	if(responsepos == l)
		*is_eos = 1;

	return APR_SUCCESS;
}

void main(int argc, char *argv[])
{
	directory_config *config;
	conn_rec *c;
	request_rec *r;

	parseargs(argc, argv);

	if(config_file == NULL || argc < 3)
	{
		printf("Usage:\n");
		printf("standalone.exe -c <config_file> [-u <text_file_with_urls>] <event_file1> [<event_file2> <event_file3> ...]\n");
		return;
	}

	modsecSetLogHook(NULL, log);

	modsecSetReadBody(readbody);
	modsecSetReadResponse(readresponse);

	modsecInit();

	modsecStartConfig();

	config = modsecGetDefaultConfig();

	const char * err = modsecProcessConfig(config, config_file, "c:\\inetpub\\wwwroot");

	if(err != NULL)
	{
		printf("%s\n", err);
	}

	modsecFinalizeConfig();

	modsecInitProcess();

	if(url_file != NULL)
	{
		FILE *fr = fopen(url_file, "rb");
		int i = 0;

		while(fgets(urls[i],4096,fr) != NULL)
		{
			urls[i][4095] = 0;

			int l = strlen(urls[i]) - 1;

			if(l < 8)
				continue;

			while(urls[i][l] == 10 || urls[i][l] == 13)
				l--;

			urls[i++][l + 1] = 0;
		}

		url_cnt = i;
		fclose(fr);
	}

	for(int i = 0; i < event_file_cnt; i++)
	{
		if(url_cnt == 0)
		{
			urls[0][0] = 0;
			url_cnt = 1;
		}

		for(int ui = 0; ui < url_cnt; ui++)
		{
			readeventfile(event_files[i]);
			parseeventfile();

			bodypos = 0;
			responsepos = 0;

			c = modsecNewConnection();

			modsecProcessConnection(c);

			r = modsecNewRequest(c, config);

			int j = event_file_blocks['B'];

			if(j < 0)
				continue;

			j++;

			if(event_file_lines[j][0] == 0)
				continue;

			char *method = event_file_lines[j];
			char *url = strchr(method, 32);
			char *proto = strchr(url + 1, 32);

			if(url == NULL || proto == NULL)
				continue;

			*url++=0;
			*proto++=0;

			if(urls[ui][0] != 0)
			{
				url = urls[ui];
			}

#define	SETMETHOD(m) if(strcmp(method,#m) == 0){ r->method = method; r->method_number = M_##m; }

			r->method = "INVALID";
			r->method_number = M_INVALID;

			SETMETHOD(OPTIONS)
			SETMETHOD(GET)
			SETMETHOD(POST)
			SETMETHOD(PUT)
			SETMETHOD(DELETE)
			SETMETHOD(TRACE)
			SETMETHOD(CONNECT)
			SETMETHOD(MOVE)
			SETMETHOD(COPY)
			SETMETHOD(PROPFIND)
			SETMETHOD(PROPPATCH)
			SETMETHOD(MKCOL)
			SETMETHOD(LOCK)
			SETMETHOD(UNLOCK)

			r->protocol = proto;

			while(event_file_lines[++j][0] != 0)
			{
				char *value = strchr(event_file_lines[j], ':');

				if(value == NULL)
					break;

				*value++ = 0;

				while(*value <=32 && *value != 0)
					value++;

				apr_table_setn(r->headers_in, event_file_lines[j], value);
			}

			r->content_encoding = apr_table_get(r->headers_in, "Content-Encoding");
			r->content_type = apr_table_get(r->headers_in, "Content-Type");
			r->hostname = apr_table_get(r->headers_in, "Host");
			r->path_info = url;
		
			char *query = strchr(url, '?');
			char *rawurl = url;

			if(query != NULL)
			{
				rawurl = (char *)apr_palloc(r->pool, strlen(url) + 1);
				strcpy(rawurl, url);
				*query++ = 0;
				r->args = query;
			}

			const char *lng = apr_table_get(r->headers_in, "Content-Languages");

			if(lng != NULL)
			{
				r->content_languages = apr_array_make(r->pool, 1, sizeof(const char *));

				*(const char **)apr_array_push(r->content_languages) = lng;
			}

			r->request_time = apr_time_now();

			r->parsed_uri.scheme = "http";
			r->parsed_uri.path = r->path_info;
			r->parsed_uri.hostname = (char *)r->hostname;
			r->parsed_uri.is_initialized = 1;
			r->parsed_uri.port = 80;
			r->parsed_uri.port_str = "80";
			r->parsed_uri.query = r->args;
			r->parsed_uri.dns_looked_up = 0;
			r->parsed_uri.dns_resolved = 0;
			r->parsed_uri.password = NULL;
			r->parsed_uri.user = NULL;
			r->parsed_uri.fragment = NULL;

			r->unparsed_uri = rawurl;
			r->uri = r->unparsed_uri;

			r->the_request = (char *)apr_palloc(r->pool, strlen(r->method) + 1 + strlen(r->uri) + 1 + strlen(r->protocol) + 1);

			strcpy(r->the_request, r->method);
			strcat(r->the_request, " ");
			strcat(r->the_request, r->uri);
			strcat(r->the_request, " ");
			strcat(r->the_request, r->protocol);

			apr_table_setn(r->subprocess_env, "UNIQUE_ID", "1");

			modsecProcessRequest(r);
			modsecProcessResponse(r);
			modsecFinishRequest(r);
		}
	}

	modsecTerminate();
	getch();
}
