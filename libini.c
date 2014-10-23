#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "config.h"
#include "libini.h"

static int validSection(const char *str);
static void parserInit(IniParser *parser);
static int parseSection(IniParser *parser, char *line);
static int parseLine(IniParser *parser, char *line, ssize_t size);
static int parseKeyPair(IniParser *parser, char *line);

static char *trimSpace(char *str)
{
    char *end;
    while (isspace(*str)) str++;

    end = str + strlen(str) - 1; //skip '\n'
    while (isspace(*end) || *end == '\n' || *end == '\t') end--;

    *(++end) = '\0';
    return str;
}

IniParser *parseFile(const char *fileName)
{
    char *linep = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    IniParser *parser;
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("Can not open file %s: %s", fileName, strerror(errno));
        return NULL;
    }

    parser = (IniParser*)malloc(sizeof(IniParser));
    while ((linelen = getline(&linep, &linecap, fp)) > 0) {
        int ret = parseLine(parser, linep, linelen);
        free(linep);
        linep = NULL;
        linecap = 0;
        if (ret) {
            free(parser);
            return NULL;
        }
    }

    return parser;
}

static int parseLine(IniParser *parser, char *line, ssize_t size)
{
    char *str = trimSpace(line);
    if (str == NULL);
    // skip empty and comment line
    if (strlen(str) == 0 || str[0] == ';')
        return 0;

    if (parser->count == 0) {
        // section is empty; next string must be a section header.
        parserInit(parser);
        return parseSection(parser, str);
    }

    if (str[0] == '[')
        return parseSection(parser, str);

    return parseKeyPair(parser, str);
}

static void parserInit(IniParser *parser)
{
    parser->sections = malloc(10*sizeof(IniSection*));
    parser->size = 10;
}

static int parseSection(IniParser *parser, char *line)
{
    // do validate
    if (validSection(line))
        return -1;

    line++;
    line[strlen(line)-1]='\0';

    if (parser->count == parser->size) {
        parser->size *= 2;
        parser->sections = realloc(parser->sections, sizeof(IniSection*)*parser->size);
    }
    IniSection *section = malloc(sizeof(IniSection));
    section->sectionName = strdup(line);
    section->keys = malloc(10*sizeof(KeyPair*));
    section->count = 0;
    section->size = 10;
    parser->sections[parser->count++] = section;
    return 0;
}

static int validSection(const char *line)
{
    int p = 1;
    if (line[0] != '[')
        return -1;
    while (isalnum(line[p])) p++;
    if (line[p] != ']' || p != strlen(line)-1) {
        return -1;
    }
    return 0;
}

static int parseKeyPair(IniParser *parser, char *line)
{
    char *eq = strchr(line, '=');
    if (eq == NULL)
        return -1;
    *eq++ = '\0';
    char *key = trimSpace(line);
    char *val = trimSpace(eq);
    KeyPair *_key = malloc(sizeof(KeyPair));
    _key->key = strdup(key);
    _key->value = strdup(val);
    IniSection *curr_section = parser->sections[parser->count-1];
    if (curr_section->size == 0) {
        curr_section->keys = malloc(10*sizeof(KeyPair*));
        curr_section->count = 10;
    }
    if (curr_section->size == curr_section->count) {
        curr_section->size *= 2;
        curr_section->keys = realloc(curr_section->keys, sizeof(KeyPair*)*curr_section->size);
    }
    curr_section->keys[curr_section->count++] = _key;
    return 0;
}

IniSection *getSection(IniParser *parser, const char *section)
{
    int i;
    IniSection *s = NULL;
    if (parser == NULL)
        return NULL;
    for (i=0; i<parser->count; i++) {
        s = parser->sections[i];
        if (strcmp(s->sectionName, section) == 0)
            return s;
    }
    return NULL;
}

KeyPair *getKey(IniSection *section, const char *key)
{
    int i;
    KeyPair *k = NULL;
    if (section == NULL)
        return NULL;
    for (i=0; i<section->count; i++) {
        k = section->keys[i];
        if (strcmp(k->key, key) == 0)
            return k;
    }
    return NULL;
}

void freeIniParser(IniParser *parser)
{
    int i, j;
    for (i=0; i<parser->count; i++) {
        IniSection *section = parser->sections[i];
        for (j=0; j<section->count; j++) {
            KeyPair *k = section->keys[j];
            free(k->key);
            free(k->value);
            free(k);
        }
        free(section->sectionName);
        free(section->keys);
        free(section);
    }
    free(parser->sections);
    free(parser);
}
