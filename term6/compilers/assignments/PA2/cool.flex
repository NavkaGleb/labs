/*
 *  The scanner definition for COOL.
 */

/*
 *  Stuff enclosed in %{ %} in the first section is copied verbatim to the
 *  output, so headers and global definitions are placed here to be visible
 * to the code in the file.  Don't remove anything that was here initially
 */
%{
#include <cool-parse.h>
#include <stringtab.h>
#include <utilities.h>

#include <stdint.h>

/* The compiler assumes these identifiers. */
#define yylval cool_yylval
#define yylex  cool_yylex

/* Max size of string constants */
#define MAX_STR_CONST 1025
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the Cool compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
		YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern int curr_lineno;
extern int verbose_flag;

extern YYSTYPE cool_yylval;

/*
 *  Add Your own definitions here
 */

static uint32_t g_string_buffer_size = 0;
static uint8_t g_string_escaped = 0;

%}

/* Define names for regular expressions here */

/* From cool manual 10.4 */
/* Keywords */
CLASS                           [Cc][Ll][Aa]([Ss]{2})
ELSE                            [Ee][Ll][Ss][Ee]
FALSE                           [f][Aa][Ll][Ss][Ee]
FI                              [Ff][Ii]
IF                              [Ii][Ff]
IN                              [Ii][Nn]
INHERITS                        [Ii][Nn][Hh][Ee][Rr][Ii][Tt][Ss]
ISVOID                          [Ii][Ss][Vv][Oo][Ii][Dd]
LET                             [Ll][Ee][Tt]
LOOP                            [Ll]([Oo]{2})[Pp]
POOL                            [Pp]([Oo]{2})[Ll]
THEN                            [Tt][Hh][Ee][Nn]
WHILE                           [Ww][Hh][Ii][Ll][Ee]
CASE                            [Cc][Aa][Ss][Ee]
ESAC                            [Ee][Ss][Aa][Cc]
NEW                             [Nn][Ee][Ww]
OF                              [Oo][Ff]
NOT                             [Nn][Oo][Tt]
TRUE                            [t][Rr][Uu][Ee]

/* Multi-character operators */
DARROW                          =>
ASSIGN                          <-
LE                              <=

INT_CONST                       [0-9]+
TYPEID                          [A-Z][A-Za-z0-9_]*
OBJECTID                        [a-z][A-Za-z0-9_]*
SPECIAL_SYMBOLS                 [.@~*/+-<=(){}:,;]
STRING_SCOPE                    \"
NULL_CHAR                       \0

WHITESPACE                      [ \f\r\t\v]+
NEW_LINE                        \n

/* Comments */
SINGLE_LINE_COMMENT_BEGIN       "--"
MULTIPLE_LINE_COMMENT_BEGIN     "(*"
MULTIPLE_LINE_COMMENT_END       "*)"

%x MULTIPLE_LINE_COMMENT
%x STRING

%%

{CLASS} {
    return CLASS;
}

{ELSE} {
    return ELSE;
}

{FALSE} {
    cool_yylval.boolean = false;
    return BOOL_CONST;
}

{FI} {
    return FI;
}

{IF} {
    return IF;
}

{IN} {
    return IN;
}

{INHERITS} {
    return INHERITS;
}

{ISVOID} {
    return ISVOID;
}

{LET} {
    return LET;
}

{LOOP} {
    return LOOP;
}

{POOL} {
    return POOL;
}

{THEN} {
    return THEN;
}

{WHILE} {
    return WHILE;
}

{CASE} {
    return CASE;
}

{ESAC} {
    return ESAC;
}

{NEW} {
    return NEW;
}

{OF} {
    return OF;
}

{NOT} {
    return NOT;
}

{TRUE} {
    cool_yylval.boolean = true;
    return BOOL_CONST;
}

{DARROW} {
    return DARROW;
}

{ASSIGN} {
    return ASSIGN;
}

{LE} {
    return LE;
}

{INT_CONST} {
    cool_yylval.symbol = inttable.add_string(yytext);
    return INT_CONST;
}

{TYPEID} {
    cool_yylval.symbol = idtable.add_string(yytext);
    return TYPEID;
}

{OBJECTID} {
    cool_yylval.symbol = idtable.add_string(yytext);
    return OBJECTID;
}

{SPECIAL_SYMBOLS} {
    return yytext[0];
}

{WHITESPACE} {}

{NEW_LINE} {
    ++curr_lineno;
}

{SINGLE_LINE_COMMENT_BEGIN}.* {}

<MULTIPLE_LINE_COMMENT,INITIAL>{MULTIPLE_LINE_COMMENT_BEGIN} {
    BEGIN(MULTIPLE_LINE_COMMENT);
}

<MULTIPLE_LINE_COMMENT><<EOF>> {
    cool_yylval.error_msg = "EOF in string constant comment";

    BEGIN(INITIAL);
    return ERROR;
}

<MULTIPLE_LINE_COMMENT>{MULTIPLE_LINE_COMMENT_END} {
    BEGIN(INITIAL);
}

<MULTIPLE_LINE_COMMENT>{NEW_LINE} {
    ++curr_lineno;
}

<MULTIPLE_LINE_COMMENT>. {}

{MULTIPLE_LINE_COMMENT_END} {
    cool_yylval.error_msg = "Unmatched *)";
    return ERROR;
}

<INITIAL>{STRING_SCOPE} {
    g_string_escaped = 0;
    g_string_buffer_size = 0;
    string_buf_ptr = string_buf;

    BEGIN(STRING);
}

<STRING><<EOF>> {
    cool_yylval.error_msg = "EOF in string constant";

    BEGIN(INITIAL);
    return (ERROR);
}

<STRING>{STRING_SCOPE} {
    if (g_string_escaped == 1) {
        *string_buf_ptr++ = '\"';
        ++g_string_buffer_size;
        g_string_escaped = 0;
    } else {
        *string_buf_ptr++ = '\0';
        ++g_string_buffer_size;

        BEGIN(INITIAL);

        if (g_string_buffer_size > MAX_STR_CONST) {
        cool_yylval.error_msg = "String constant too long";
        return ERROR;
        }

        cool_yylval.symbol = stringtable.add_string(string_buf);
        return STR_CONST;
    }
}

<STRING>{NULL_CHAR}.*{STRING_SCOPE} {
    cool_yylval.error_msg = "String contains null character";

    BEGIN(INITIAL);
    return ERROR;
}

<STRING>.|{NEW_LINE} {
    const char c = yytext[0];

    if (c == '\n') {
        ++curr_lineno;
    }

    if (g_string_escaped == 1) {
        switch (c) {
            case 'n': {
                *string_buf_ptr++ = '\n';
                ++g_string_buffer_size;

                break;
            }
            case 't': {
                *string_buf_ptr++ = '\t';
                ++g_string_buffer_size;

                break;
            }
            case 'b': {
                *string_buf_ptr++ = '\b';
                ++g_string_buffer_size;

                break;
            }
            case 'f': {
            *string_buf_ptr++ = '\f';
                ++g_string_buffer_size;

                break;
            }
            default: {
            *string_buf_ptr++ = c;
                ++g_string_buffer_size;

                break;
            }
        }

        g_string_escaped = 0;
    } else {
        switch (c) {
            case '\\': {
                g_string_escaped = 1;

                break;
            }
            case '\n': {
                cool_yylval.error_msg = "Unterminated string constant";

                BEGIN(INITIAL);
                return ERROR;
            }
            default: {
                *string_buf_ptr++ = c;
                ++g_string_buffer_size;

                break;
            }
        }
    }
}

. {
    cool_yylval.error_msg = yytext;
    return ERROR;
}

%%
