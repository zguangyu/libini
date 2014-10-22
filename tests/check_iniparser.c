#include <check.h>
#include <stdlib.h>
#include "iniparser.h"

START_TEST(test_parse_file)
{
    IniParser *parser;
    char *fileName = "check_iniparser.ini";
    parser = parseFile(fileName);
    ck_assert(parser != NULL);
    IniSection *s = getSection(parser, "test");
    ck_assert(s != NULL);
    KeyPair *k = getKey(s, "space");
    ck_assert(k != NULL);
    ck_assert_str_eq(k->value, "space");
    freeIniParser(parser);
}
END_TEST

Suite * iniparser_suite()
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("IniParser");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_parse_file);
    suite_add_tcase(s, tc_core);

    return s;
}

int main()
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = iniparser_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
