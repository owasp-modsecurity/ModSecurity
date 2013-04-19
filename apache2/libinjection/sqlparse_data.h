#ifndef _SQLPARSE_DATA_H
#define _SQLPARSE_DATA_H
#include "sqlparse.h"

static const char* operators2[] = {
    "!!",
    "!<",
    "!=",
    "!>",
    "!~",
    "%=",
    "&&",
    "&=",
    "*=",
    "+=",
    "-=",
    "/=",
    ":=",
    "<<",
    "<=",
    "<>",
    "<@",
    ">=",
    ">>",
    "@>",
    "^=",
    "|/",
    "|=",
    "||",
    "~*",
};
static const size_t operators2_sz = 25;

static const keyword_t sql_keywords[] = {
    {"ABS", 'f'},
    {"ACCESSIBLE", 'k'},
    {"ACOS", 'f'},
    {"ADD", 'k'},
    {"ADDDATE", 'f'},
    {"ADDTIME", 'f'},
    {"AES_DECRYPT", 'f'},
    {"AES_ENCRYPT", 'f'},
    {"AGAINST", 'k'},
    {"ALL_USERS", 'k'},
    {"ALTER", 'k'},
    {"ANALYZE", 'k'},
    {"AND", '&'},
    {"AS", 'k'},
    {"ASC", 'k'},
    {"ASCII", 'f'},
    {"ASENSITIVE", 'k'},
    {"ASIN", 'f'},
    {"ATAN", 'f'},
    {"ATAN2", 'f'},
    {"AVG", 'f'},
    {"BEFORE", 'k'},
    {"BEGIN", 'k'},
    {"BENCHMARK", 'f'},
    {"BETWEEN", 'k'},
    {"BIGINT", 'k'},
    {"BIN", 'f'},
    {"BINARY", 'k'},
    {"BINARY_DOUBLE_INFINITY", '1'},
    {"BINARY_DOUBLE_NAN", '1'},
    {"BINARY_FLOAT_INFINITY", '1'},
    {"BINARY_FLOAT_NAN", '1'},
    {"BINBINARY", 'f'},
    {"BIT_AND", 'f'},
    {"BIT_COUNT", 'f'},
    {"BIT_LENGTH", 'f'},
    {"BIT_OR", 'f'},
    {"BIT_XOR", 'f'},
    {"BLOB", 'k'},
    {"BOOLEAN", 'k'},
    {"BOTH", 'k'},
    {"BY", 'n'},
    {"CALL", 'k'},
    {"CASCADE", 'k'},
    {"CASE", 'o'},
    {"CAST", 'f'},
    {"CEIL", 'f'},
    {"CEILING", 'f'},
    {"CHANGE", 'k'},
    {"CHAR", 'f'},
    {"CHARACTER", 'k'},
    {"CHARACTER_LENGTH", 'f'},
    {"CHARSET", 'f'},
    {"CHAR_LENGTH", 'f'},
    {"CHECK", 'k'},
    {"CHR", 'f'},
    {"COALESCE", 'k'},
    {"COERCIBILITY", 'f'},
    {"COLLATE", 'k'},
    {"COLLATION", 'f'},
    {"COLUMN", 'k'},
    {"COMPRESS", 'f'},
    {"CONCAT", 'f'},
    {"CONCAT_WS", 'f'},
    {"CONDITION", 'k'},
    {"CONNECTION_ID", 'f'},
    {"CONSTRAINT", 'k'},
    {"CONTINUE", 'k'},
    {"CONV", 'f'},
    {"CONVERT", 'f'},
    {"CONVERT_TZ", 'f'},
    {"COS", 'f'},
    {"COT", 'f'},
    {"COUNT", 'f'},
    {"CRC32", 'f'},
    {"CREATE", 'k'},
    {"CTXSYS.DRITHSX.SN", 'f'},
    {"CURDATE", 'f'},
    {"CURRENT_DATE", 'k'},
    {"CURRENT_DATECURRENT_TIME", 'f'},
    {"CURRENT_TIME", 'k'},
    {"CURRENT_TIMESTAMP", 'k'},
    {"CURRENT_USER", 'k'},
    {"CURSOR", 'k'},
    {"CURTIME", 'f'},
    {"DATABASE", 'k'},
    {"DATABASES", 'k'},
    {"DATE", 'f'},
    {"DATEDIFF", 'f'},
    {"DATE_ADD", 'f'},
    {"DATE_FORMAT", 'f'},
    {"DATE_SUB", 'f'},
    {"DAY", 'f'},
    {"DAYNAME", 'f'},
    {"DAYOFMONTH", 'f'},
    {"DAYOFWEEK", 'f'},
    {"DAYOFYEAR", 'f'},
    {"DAY_HOUR", 'k'},
    {"DAY_MICROSECOND", 'k'},
    {"DAY_MINUTE", 'k'},
    {"DAY_SECOND", 'k'},
    {"DBMS_PIPE.RECEIVE_MESSAGE", 'f'},
    {"DEC", 'k'},
    {"DECIMAL", 'k'},
    {"DECLARE", 'k'},
    {"DECODE", 'f'},
    {"DEFAULT", 'k'},
    {"DEGREES", 'f'},
    {"DELAY", 'k'},
    {"DELAYED", 'k'},
    {"DELETE", 'k'},
    {"DESC", 'k'},
    {"DESCRIBE", 'k'},
    {"DES_DECRYPT", 'f'},
    {"DES_ENCRYPT", 'f'},
    {"DETERMINISTIC", 'k'},
    {"DISTINCROW", 'k'},
    {"DISTINCT", 'k'},
    {"DIV", 'o'},
    {"DROP", 'k'},
    {"DUAL", 'k'},
    {"EACH", 'k'},
    {"ELSE", 'k'},
    {"ELSEIF", 'k'},
    {"ELT", 'f'},
    {"ENCLOSED", 'k'},
    {"ENCODE", 'f'},
    {"ENCRYPT", 'f'},
    {"ESCAPED", 'k'},
    {"EXEC", 'k'},
    {"EXECUTE", 'k'},
    {"EXISTS", 'k'},
    {"EXIT", 'k'},
    {"EXP", 'f'},
    {"EXPLAIN", 'k'},
    {"EXPORT_SET", 'f'},
    {"EXTRACT", 'f'},
    {"EXTRACTVALUE", 'f'},
    {"EXTRACT_VALUE", 'f'},
    {"FALSE", '1'},
    {"FETCH", 'k'},
    {"FIELD", 'f'},
    {"FIND_IN_SET", 'f'},
    {"FLOOR", 'f'},
    {"FOR", 'n'},
    {"FORCE", 'k'},
    {"FOREIGN", 'k'},
    {"FORMAT", 'f'},
    {"FOUND_ROWS", 'f'},
    {"FROM", 'k'},
    {"FROM_DAYS", 'f'},
    {"FROM_UNIXTIME", 'f'},
    {"FULLTEXT", 'k'},
    {"GENERATE_SERIES", 'f'},
    {"GET_FORMAT", 'f'},
    {"GET_LOCK", 'f'},
    {"GOTO", 'k'},
    {"GRANT", 'k'},
    {"GREATEST", 'f'},
    {"GROUP", 'n'},
    {"GROUP_CONCAT", 'f'},
    {"HAVING", 'k'},
    {"HEX", 'f'},
    {"HIGH_PRIORITY", 'k'},
    {"HOST_NAME", 'f'},
    {"HOUR", 'f'},
    {"HOUR_MICROSECOND", 'k'},
    {"HOUR_MINUTE", 'k'},
    {"HOUR_SECOND", 'k'},
    {"IF", 'k'},
    {"IFF", 'f'},
    {"IFNULL", 'f'},
    {"IGNORE", 'k'},
    {"IIF", 'f'},
    {"IN", 'n'},
    {"INDEX", 'k'},
    {"INET_ATON", 'f'},
    {"INET_NTOA", 'f'},
    {"INFILE", 'k'},
    {"INNER", 'k'},
    {"INOUT", 'k'},
    {"INSENSITIVE", 'k'},
    {"INSERT", 'k'},
    {"INSTR", 'f'},
    {"INT", 'k'},
    {"INT1", 'k'},
    {"INT2", 'k'},
    {"INT3", 'k'},
    {"INT4", 'k'},
    {"INT8", 'k'},
    {"INTEGER", 'k'},
    {"INTERVAL", 'k'},
    {"INTO", 'k'},
    {"IS", 'o'},
    {"ISNULL", 'f'},
    {"IS_FREE_LOCK", 'f'},
    {"IS_MEMBERIS_SRVROLEMEMBER", 'f'},
    {"IS_USED_LOCK", 'f'},
    {"ITERATE", 'k'},
    {"JOIN", 'k'},
    {"KEYS", 'k'},
    {"KILL", 'k'},
    {"LAST_INSERT_ID", 'f'},
    {"LCASE", 'f'},
    {"LEADING", 'k'},
    {"LEAST", 'f'},
    {"LEAVE", 'k'},
    {"LEFT", 'n'},
    {"LENGTH", 'f'},
    {"LIKE", 'o'},
    {"LIMIT", 'k'},
    {"LINEAR", 'k'},
    {"LINES", 'k'},
    {"LN", 'f'},
    {"LOAD", 'k'},
    {"LOAD_FILE", 'f'},
    {"LOCALTIME", 'k'},
    {"LOCALTIMESTAMP", 'k'},
    {"LOCATE", 'f'},
    {"LOCK", 'n'},
    {"LOG", 'f'},
    {"LOG10", 'f'},
    {"LOG2", 'f'},
    {"LONGBLOB", 'k'},
    {"LONGTEXT", 'k'},
    {"LOOP", 'k'},
    {"LOWER", 'f'},
    {"LOW_PRIORITY", 'k'},
    {"LPAD", 'f'},
    {"LTRIM", 'f'},
    {"MAKEDATE", 'f'},
    {"MAKE_SET", 'f'},
    {"MASTER_BIND", 'k'},
    {"MASTER_POS_WAIT", 'f'},
    {"MASTER_SSL_VERIFY_SERVER_CERT", 'k'},
    {"MATCH", 'k'},
    {"MAX", 'f'},
    {"MAXVALUE", 'k'},
    {"MD5", 'f'},
    {"MEDIUMBLOB", 'k'},
    {"MEDIUMINT", 'k'},
    {"MEDIUMTEXT", 'k'},
    {"MERGE", 'k'},
    {"MICROSECOND", 'f'},
    {"MID", 'f'},
    {"MIDDLEINT", 'k'},
    {"MIN", 'f'},
    {"MINUTE", 'f'},
    {"MINUTE_MICROSECOND", 'k'},
    {"MINUTE_SECOND", 'k'},
    {"MOD", 'o'},
    {"MODE", 'n'},
    {"MODIFIES", 'k'},
    {"MONTH", 'f'},
    {"MONTHNAME", 'f'},
    {"NAME_CONST", 'f'},
    {"NOT", 'o'},
    {"NOW", 'f'},
    {"NO_WRITE_TO_BINLOG", 'k'},
    {"NULL", '1'},
    {"NULLIF", 'f'},
    {"NUMERIC", 'k'},
    {"OCT", 'f'},
    {"OCTET_LENGTH", 'f'},
    {"OFFSET", 'k'},
    {"OLD_PASSWORD", 'f'},
    {"ONE_SHOT", 'k'},
    {"OPEN", 'k'},
    {"OPENDATASOURCE", 'f'},
    {"OPENQUERY", 'f'},
    {"OPENROWSET", 'f'},
    {"OPENXML", 'f'},
    {"OPTIMIZE", 'k'},
    {"OPTION", 'k'},
    {"OPTIONALLY", 'k'},
    {"OR", '&'},
    {"ORD", 'f'},
    {"ORDER", 'n'},
    {"OUT", 'k'},
    {"OUTFILE", 'k'},
    {"OWN3D", 'k'},
    {"PARTITION", 'k'},
    {"PASSWORD", 'k'},
    {"PERIOD_ADD", 'f'},
    {"PERIOID_DIFF", 'f'},
    {"PG_ADVISORY_LOCK", 'f'},
    {"PG_SLEEP", 'f'},
    {"PI", 'f'},
    {"POSITION", 'f'},
    {"POW", 'f'},
    {"POWER", 'f'},
    {"PRECISION", 'k'},
    {"PRIMARY", 'k'},
    {"PROCEDURE", 'k'},
    {"PURGE", 'k'},
    {"QUARTER", 'f'},
    {"QUOTE", 'f'},
    {"RADIANS", 'f'},
    {"RAND", 'f'},
    {"RANDOMBLOB", 'f'},
    {"RANGE", 'k'},
    {"READ", 'k'},
    {"READS", 'k'},
    {"READ_WRITE", 'k'},
    {"REAL", 'n'},
    {"REFERENCES", 'k'},
    {"REGEXP", 'o'},
    {"RELEASE", 'k'},
    {"RELEASE_LOCK", 'f'},
    {"RENAME", 'k'},
    {"REPEAT", 'k'},
    {"REPLACE", 'k'},
    {"REQUIRE", 'k'},
    {"RESIGNAL", 'k'},
    {"RESTRICT", 'k'},
    {"RETURN", 'k'},
    {"REVERSE", 'f'},
    {"REVOKE", 'k'},
    {"RIGHT", 'n'},
    {"RLIKE", 'o'},
    {"ROUND", 'f'},
    {"ROW", 'f'},
    {"ROW_COUNT", 'f'},
    {"RPAD", 'f'},
    {"RTRIM", 'f'},
    {"SCHEMA", 'k'},
    {"SCHEMAS", 'k'},
    {"SECOND_MICROSECOND", 'k'},
    {"SEC_TO_TIME", 'f'},
    {"SELECT", 'k'},
    {"SENSITIVE", 'k'},
    {"SEPARATOR", 'k'},
    {"SESSION_USER", 'f'},
    {"SET", 'k'},
    {"SHA", 'f'},
    {"SHA1", 'f'},
    {"SHA2", 'f'},
    {"SHOW", 'n'},
    {"SHUTDOWN", 'k'},
    {"SIGN", 'f'},
    {"SIGNAL", 'k'},
    {"SIMILAR", 'k'},
    {"SIN", 'f'},
    {"SLEEP", 'f'},
    {"SMALLINT", 'k'},
    {"SOUNDEX", 'f'},
    {"SOUNDS", 'o'},
    {"SPACE", 'f'},
    {"SPATIAL", 'k'},
    {"SPECIFIC", 'k'},
    {"SQL", 'k'},
    {"SQLEXCEPTION", 'k'},
    {"SQLSTATE", 'k'},
    {"SQLWARNING", 'k'},
    {"SQL_BIG_RESULT", 'k'},
    {"SQL_CALC_FOUND_ROWS", 'k'},
    {"SQL_SMALL_RESULT", 'k'},
    {"SQRT", 'f'},
    {"SSL", 'k'},
    {"STARTING", 'k'},
    {"STDDEV", 'f'},
    {"STDDEV_POP", 'f'},
    {"STDDEV_SAMP", 'f'},
    {"STRAIGHT_JOIN", 'k'},
    {"STRCMP", 'f'},
    {"STR_TO_DATE", 'f'},
    {"SUBDATE", 'f'},
    {"SUBSTR", 'f'},
    {"SUBSTRING", 'f'},
    {"SUBSTRING_INDEX", 'f'},
    {"SUBTIME", 'f'},
    {"SUM", 'f'},
    {"SYS.STRAGG", 'f'},
    {"SYSCOLUMNS", 'k'},
    {"SYSDATE", 'f'},
    {"SYSOBJECTS", 'k'},
    {"SYSTEM_USER", 'f'},
    {"SYSUSERS", 'k'},
    {"TABLE", 'k'},
    {"TAN", 'f'},
    {"TERMINATED", 'k'},
    {"THEN", 'k'},
    {"TIME", 'k'},
    {"TIMEDIFF", 'f'},
    {"TIMESTAMP", 'f'},
    {"TIMESTAMPADD", 'f'},
    {"TIME_FORMAT", 'f'},
    {"TIME_TO_SEC", 'f'},
    {"TINYBLOB", 'k'},
    {"TINYINT", 'k'},
    {"TINYTEXT", 'k'},
    {"TOP", 'k'},
    {"TO_CHAR", 'f'},
    {"TO_DAYS", 'f'},
    {"TO_SECONDS", 'f'},
    {"TRAILING", 'n'},
    {"TRIGGER", 'k'},
    {"TRIM", 'f'},
    {"TRUE", '1'},
    {"TRUNCATE", 'f'},
    {"UCASE", 'f'},
    {"UNCOMPRESS", 'f'},
    {"UNCOMPRESS_LENGTH", 'f'},
    {"UNDO", 'k'},
    {"UNHEX", 'f'},
    {"UNION", 'U'},
    {"UNIQUE", 'n'},
    {"UNIX_TIMESTAMP", 'f'},
    {"UNI_ON", 'U'},
    {"UNLOCK", 'k'},
    {"UNSIGNED", 'k'},
    {"UPDATE", 'k'},
    {"UPDATEXML", 'f'},
    {"UPPER", 'f'},
    {"USAGE", 'k'},
    {"USE", 'k'},
    {"USING", 'f'},
    {"UTC_DATE", 'k'},
    {"UTC_TIME", 'k'},
    {"UTC_TIMESTAMP", 'k'},
    {"UTL_INADDR.GET_HOST_ADDRESS", 'f'},
    {"UUID", 'f'},
    {"UUID_SHORT", 'f'},
    {"VALUES", 'k'},
    {"VARBINARY", 'k'},
    {"VARCHAR", 'k'},
    {"VARCHARACTER", 'k'},
    {"VARIANCE", 'f'},
    {"VARYING", 'k'},
    {"VAR_POP", 'f'},
    {"VAR_SAMP", 'f'},
    {"VERSION", 'f'},
    {"WAITFOR", 'k'},
    {"WEEK", 'f'},
    {"WEEKDAY", 'f'},
    {"WEEKOFYEAR", 'f'},
    {"WHEN", 'k'},
    {"WHERE", 'k'},
    {"WHILE", 'k'},
    {"WITH", 'k'},
    {"XMLELEMENT", 'f'},
    {"XMLFOREST", 'f'},
    {"XMLFORMAT", 'f'},
    {"XMLTYPE", 'f'},
    {"XOR", 'o'},
    {"XP_EXECRESULTSET", 'k'},
    {"YEAR", 'f'},
    {"YEARWEEK", 'f'},
    {"YEAR_MONTH", 'k'},
    {"ZEROFILL", 'k'},
};
static const size_t sql_keywords_sz = 450;
static const char* multikeywords_start[] = {
    "ALTER",
    "CROSS",
    "FULL",
    "GROUP",
    "IN",
    "IN BOOLEAN",
    "INTERSECT",
    "IS",
    "LEFT",
    "LOCK",
    "NATURAL",
    "NOT",
    "NOT SIMILAR",
    "ORDER",
    "OWN3D",
    "READ",
    "RIGHT",
    "SELECT",
    "SIMILAR",
    "SOUNDS",
    "UNION",
};
static const size_t multikeywords_start_sz = 21;
static const keyword_t multikeywords[] = {
    {"ALTER DOMAIN", 'k'},
    {"ALTER TABLE", 'k'},
    {"CROSS JOIN", 'k'},
    {"FULL OUTER", 'k'},
    {"GROUP BY", 'B'},
    {"IN BOOLEAN", 'n'},
    {"IN BOOLEAN MODE", 'k'},
    {"INTERSECT ALL", 'o'},
    {"IS NOT", 'o'},
    {"LEFT JOIN", 'k'},
    {"LEFT OUTER", 'k'},
    {"LOCK TABLE", 'k'},
    {"LOCK TABLES", 'k'},
    {"NATURAL FULL", 'k'},
    {"NATURAL INNER", 'k'},
    {"NATURAL JOIN", 'k'},
    {"NATURAL LEFT", 'k'},
    {"NATURAL OUTER", 'k'},
    {"NATURAL RIGHT", 'k'},
    {"NOT BETWEEN", 'o'},
    {"NOT IN", 'o'},
    {"NOT LIKE", 'o'},
    {"NOT REGEXP", 'o'},
    {"NOT RLIKE", 'o'},
    {"NOT SIMILAR", 'o'},
    {"NOT SIMILAR TO", 'o'},
    {"ORDER BY", 'B'},
    {"OWN3D BY", 'B'},
    {"READ WRITE", 'k'},
    {"RIGHT JOIN", 'k'},
    {"RIGHT OUTER", 'k'},
    {"SELECT ALL", 'k'},
    {"SIMILAR TO", 'o'},
    {"SOUNDS LIKE", 'o'},
    {"UNION ALL", 'U'},
};
static const size_t multikeywords_sz = 35;

typedef size_t (*pt2Function)(sfilter *sf);
static const pt2Function char_parse_map[] = {
   &parse_white, /* 0 */
   &parse_white, /* 1 */
   &parse_white, /* 2 */
   &parse_white, /* 3 */
   &parse_white, /* 4 */
   &parse_white, /* 5 */
   &parse_white, /* 6 */
   &parse_white, /* 7 */
   &parse_white, /* 8 */
   &parse_white, /* 9 */
   &parse_white, /* 10 */
   &parse_white, /* 11 */
   &parse_white, /* 12 */
   &parse_white, /* 13 */
   &parse_white, /* 14 */
   &parse_white, /* 15 */
   &parse_white, /* 16 */
   &parse_white, /* 17 */
   &parse_white, /* 18 */
   &parse_white, /* 19 */
   &parse_white, /* 20 */
   &parse_white, /* 21 */
   &parse_white, /* 22 */
   &parse_white, /* 23 */
   &parse_white, /* 24 */
   &parse_white, /* 25 */
   &parse_white, /* 26 */
   &parse_white, /* 27 */
   &parse_white, /* 28 */
   &parse_white, /* 29 */
   &parse_white, /* 30 */
   &parse_white, /* 31 */
   &parse_white, /* 32 */
   &parse_operator2, /* 33 */
   &parse_string, /* 34 */
   &parse_eol_comment, /* 35 */
   &parse_white, /* 36 */
   &parse_operator1, /* 37 */
   &parse_operator2, /* 38 */
   &parse_string, /* 39 */
   &parse_char, /* 40 */
   &parse_char, /* 41 */
   &parse_operator2, /* 42 */
   &parse_operator1, /* 43 */
   &parse_char, /* 44 */
   &parse_dash, /* 45 */
   &parse_number, /* 46 */
   &parse_slash, /* 47 */
   &parse_number, /* 48 */
   &parse_number, /* 49 */
   &parse_number, /* 50 */
   &parse_number, /* 51 */
   &parse_number, /* 52 */
   &parse_number, /* 53 */
   &parse_number, /* 54 */
   &parse_number, /* 55 */
   &parse_number, /* 56 */
   &parse_number, /* 57 */
   &parse_char, /* 58 */
   &parse_char, /* 59 */
   &parse_operator2, /* 60 */
   &parse_operator2, /* 61 */
   &parse_operator2, /* 62 */
   &parse_other, /* 63 */
   &parse_var, /* 64 */
   &parse_word, /* 65 */
   &parse_word, /* 66 */
   &parse_word, /* 67 */
   &parse_word, /* 68 */
   &parse_word, /* 69 */
   &parse_word, /* 70 */
   &parse_word, /* 71 */
   &parse_word, /* 72 */
   &parse_word, /* 73 */
   &parse_word, /* 74 */
   &parse_word, /* 75 */
   &parse_word, /* 76 */
   &parse_word, /* 77 */
   &parse_word, /* 78 */
   &parse_word, /* 79 */
   &parse_word, /* 80 */
   &parse_word, /* 81 */
   &parse_word, /* 82 */
   &parse_word, /* 83 */
   &parse_word, /* 84 */
   &parse_word, /* 85 */
   &parse_word, /* 86 */
   &parse_word, /* 87 */
   &parse_word, /* 88 */
   &parse_word, /* 89 */
   &parse_word, /* 90 */
   &parse_other, /* 91 */
   &parse_backslash, /* 92 */
   &parse_other, /* 93 */
   &parse_operator1, /* 94 */
   &parse_word, /* 95 */
   &parse_word, /* 96 */
   &parse_word, /* 97 */
   &parse_word, /* 98 */
   &parse_word, /* 99 */
   &parse_word, /* 100 */
   &parse_word, /* 101 */
   &parse_word, /* 102 */
   &parse_word, /* 103 */
   &parse_word, /* 104 */
   &parse_word, /* 105 */
   &parse_word, /* 106 */
   &parse_word, /* 107 */
   &parse_word, /* 108 */
   &parse_word, /* 109 */
   &parse_word, /* 110 */
   &parse_word, /* 111 */
   &parse_word, /* 112 */
   &parse_word, /* 113 */
   &parse_word, /* 114 */
   &parse_word, /* 115 */
   &parse_word, /* 116 */
   &parse_word, /* 117 */
   &parse_word, /* 118 */
   &parse_word, /* 119 */
   &parse_word, /* 120 */
   &parse_word, /* 121 */
   &parse_word, /* 122 */
   &parse_other, /* 123 */
   &parse_operator2, /* 124 */
   &parse_other, /* 125 */
   &parse_operator1, /* 126 */
   &parse_white, /* 127 */
};

#endif
