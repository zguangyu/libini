typedef struct KeyPair {
    char *key;
    char *value;
} KeyPair;

typedef struct IniSection {
    char *sectionName;
    KeyPair **keys;
    int size;
    int count;
} IniSection;

typedef struct IniParser {
    IniSection **sections;
    int size;
    int count;
} IniParser;

IniParser *parseFile(const char *fileName);
IniSection *getSection(IniParser *parser, const char *section);
KeyPair *getKey(IniSection *section, const char *key);
void freeIniParser(IniParser *parser);
