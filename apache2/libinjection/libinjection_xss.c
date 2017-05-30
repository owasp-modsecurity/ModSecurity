#include "libinjection_xss.h"
#include "libinjection_html5.h"

#include <assert.h>
#include <stdio.h>

/*
 * HEY THIS ISN'T DONE
 *  AND MISSING A KEY INGREDIENT!!
 *
 */

typedef enum attribute {
    TYPE_NONE
    , TYPE_BLACK     /* ban always */
    , TYPE_ATTR_URL   /* attribute value takes a URL-like object */
    , TYPE_STYLE
    , TYPE_ATTR_INDIRECT  /* attribute *name* is given in *value* */
} attribute_t;

typedef struct stringtype {
    const char* name;
    attribute_t atype;
} stringtype_t;

/*
 * view-source:
 * data:
 * javascript:
 */
static stringtype_t BLACKATTR[] = {
    { "ACTION", TYPE_ATTR_URL }     /* form */
    , { "ATTRIBUTENAME", TYPE_ATTR_INDIRECT } /* SVG allow indirection of attribute names */
    , { "BY", TYPE_ATTR_URL }         /* SVG */
    , { "BACKGROUND", TYPE_ATTR_URL } /* IE6, O11 */
    , { "DATAFORMATAS", TYPE_BLACK }  /* IE */
    , { "DATASRC", TYPE_BLACK }       /* IE */
    , { "DYNSRC", TYPE_ATTR_URL }     /* Obsolete img attribute */
    , { "FILTER", TYPE_STYLE }        /* Opera, SVG inline style */
    , { "FORMACTION", TYPE_ATTR_URL } /* HTML5 */
    , { "FOLDER", TYPE_ATTR_URL }     /* Only on A tags, IE-only */
    , { "FROM", TYPE_ATTR_URL }       /* SVG */
    , { "HANDLER", TYPE_ATTR_URL }    /* SVG Tiny, Opera */
    , { "HREF", TYPE_ATTR_URL }
    , { "LOWSRC", TYPE_ATTR_URL }     /* Obsolete img attribute */
    , { "POSTER", TYPE_ATTR_URL }     /* Opera 10,11 */
    , { "SRC", TYPE_ATTR_URL }
    , { "STYLE", TYPE_STYLE }
    , { "TO", TYPE_ATTR_URL }         /* SVG */
    , { "VALUES", TYPE_ATTR_URL }     /* SVG */
    , { "XLINK:HREF", TYPE_ATTR_URL }
    , { NULL, TYPE_NONE }
};

/* xmlns */
/* xml-stylesheet > <eval>, <if expr=> */

/*
static const char* BLACKATTR[] = {
    "ATTRIBUTENAME",
    "BACKGROUND",
    "DATAFORMATAS",
    "HREF",
    "SCROLL",
    "SRC",
    "STYLE",
    "SRCDOC",
    NULL
};
*/

static const char* BLACKTAG[] = {
    "APPLET"
    /*    , "AUDIO" */
    , "BASE"
    , "EMBED"
    /*   ,  "FORM" */
    , "FRAME"
    , "FRAMESET"
    , "HANDLER" /* Opera SVG, effectively a script tag */
    , "IFRAME"
    , "IMPORT"
    , "ISINDEX"
    , "LINK"
    , "LISTENER"
    /*    , "MARQUEE" */
    , "META"
    , "NOSCRIPT"
    , "OBJECT"
    , "SCRIPT"
    , "STYLE"
    /*    , "VIDEO" */
    , "VMLFRAME"
    , "XML"
    , NULL
};

static int is_black_tag(const char* s, size_t len);
static attribute_t is_black_attr(const char* s, size_t len);
static int is_black_url(const char* s, size_t len);
static int cstrcasecmp_with_null(const char *a, const char *b, size_t n);

static int cstrcasecmp_with_null(const char *a, const char *b, size_t n)
{
    char cb;

    for (; n > 0; a++, b++, n--) {
        cb = *b;
        if (cb == '\0') continue;

        if (cb >= 'a' && cb <= 'z') {
            cb -= 0x20;
        }
        if (*a != cb) {
            return *a - cb;
        } else if (*a == '\0') {
            return -1;
        }
    }

    return (*a == 0) ? 0 : 1;
}

static int is_black_tag(const char* s, size_t len)
{
    const char** black;

    if (len < 3) {
        return 0;
    }

    black = BLACKTAG;
    while (*black != NULL) {
        if (cstrcasecmp_with_null(*black, s, len) == 0) {
            return 1;
        }
        black += 1;
    }

    /* anything SVG related */
    if ((s[0] == 's' || s[0] == 'S') &&
        (s[1] == 'v' || s[1] == 'V') &&
        (s[2] == 'g' || s[2] == 'G')) {
        return 1;
    }

    /* Anything XSL(t) related */
    if ((s[0] == 'x' || s[0] == 'X') &&
        (s[1] == 's' || s[1] == 'S') &&
        (s[2] == 'l' || s[2] == 'L')) {
        return 1;
    }

    return 0;
}

static attribute_t is_black_attr(const char* s, size_t len)
{
    stringtype_t* black;

    if (len < 2) {
        return TYPE_NONE;
    }

    /* javascript on.* */
    if ((s[0] == 'o' || s[0] == 'O') && (s[1] == 'n' || s[1] == 'N')) {
        return TYPE_BLACK;
    }


    if (len >= 5) {
        /* XMLNS can be used to create arbitrary tags */
        if (cstrcasecmp_with_null("XMLNS", s, 5) == 0 || cstrcasecmp_with_null("XLINK", s, 5) == 0) {
            return TYPE_BLACK;
        }
    }

    black = BLACKATTR;
    while (black->name != NULL) {
        if (cstrcasecmp_with_null(black->name, s, len) == 0) {
            return black->atype;
        }
        black += 1;
    }

    return TYPE_NONE;
}

static int is_black_url(const char* s, size_t len)
{

    static const char* data_url = "DATA";
    static const char* viewsource_url = "VIEW-SOURCE";

    /* obsolete but interesting signal */
    static const char* vbscript_url = "VBSCRIPT";

    /* covers JAVA, JAVASCRIPT, + colon */
    static const char* javascript_url = "JAVA";

    size_t tokenlen;

    /* skip whitespace */
    while (len > 0) {
        /*
         * HEY: this is a signed character.
         *  We are intentionally skipping high-bit characters too
         *  since they are not ascii, and Opera sometimes uses UTF8 whitespace
         */
        if (*s <= 32) {
            ++s;
            --len;
        }
        break;
    }

    tokenlen = strlen(data_url);
    if (len > tokenlen && cstrcasecmp_with_null(data_url, s, tokenlen) == 0) {
        return 1;
    }
    tokenlen = strlen(viewsource_url);
    if (len > tokenlen && cstrcasecmp_with_null(viewsource_url, s, tokenlen) == 0) {
        return 1;
    }

    tokenlen = strlen(javascript_url);
    if (len > tokenlen && cstrcasecmp_with_null(javascript_url, s, tokenlen) == 0) {
        return 1;
    }

    tokenlen = strlen(vbscript_url);
    if (len > tokenlen && cstrcasecmp_with_null(vbscript_url, s, tokenlen) == 0) {
        return 1;
    }
    return 0;
}

int libinjection_is_xss(const char* s, size_t len)
{
    h5_state_t h5;
    attribute_t attr = TYPE_NONE;

    libinjection_h5_init(&h5, s, len, 0);
    while (libinjection_h5_next(&h5)) {
        if (h5.token_type != ATTR_VALUE) {
            attr = TYPE_NONE;
        }

        if (h5.token_type == DOCTYPE) {
            return 1;
        } else if (h5.token_type == TAG_NAME_OPEN) {
            if (is_black_tag(h5.token_start, h5.token_len)) {
                return 1;
            }
        } else if (h5.token_type == ATTR_NAME) {
            attr = is_black_attr(h5.token_start, h5.token_len);
        } else if (h5.token_type == ATTR_VALUE) {
            /*
             * IE6,7,8 parsing works a bit differently so
             * a whole <script> or other black tag might be hiding
             * inside an attribute value under HTML5 parsing
             * See http://html5sec.org/#102
             * to avoid doing a full reparse of the value, just
             * look for "<".  This probably need adjusting to
             * handle escaped characters
             */
            /*
            if (memchr(h5.token_start, '<', h5.token_len) != NULL) {
                return 1;
            }
            */

            switch (attr) {
            case TYPE_NONE:
                break;
            case TYPE_BLACK:
                return 1;
            case TYPE_ATTR_URL:
                if (is_black_url(h5.token_start, h5.token_len)) {
                    return 1;
                }
                break;
            case TYPE_STYLE:
                return 1;
            case TYPE_ATTR_INDIRECT:
                /* an attribute name is specified in a _value_ */
                if (is_black_attr(h5.token_start, h5.token_len)) {
                    return 1;
                }
                break;
/*
            default:
                assert(0);
*/
            }
            attr = TYPE_NONE;
        } else if (h5.token_type == TAG_COMMENT) {
	    /* IE uses a "`" as a tag ending char */
            if (memchr(h5.token_start, '`', h5.token_len) != NULL) {
                return 1;
            }

            /* IE conditional comment */
            if (h5.token_len > 3) {
                if (h5.token_start[0] == '[' &&
                    (h5.token_start[1] == 'i' || h5.token_start[1] == 'I') &&
                    (h5.token_start[2] == 'f' || h5.token_start[2] == 'F')) {
                    return 1;
                }
                if ((h5.token_start[0] == 'x' || h5.token_start[1] == 'X') &&
                    (h5.token_start[1] == 'm' || h5.token_start[1] == 'M') &&
                    (h5.token_start[2] == 'l' || h5.token_start[2] == 'L')) {
                    return 1;
                }
            }

            if (h5.token_len > 5) {
	        /*  IE <?import pseudo-tag */
                if (cstrcasecmp_with_null("IMPORT", h5.token_start, 6) == 0) {
                    return 1;
                }

                /*  XML Entity definition */
                if (cstrcasecmp_with_null("ENTITY", h5.token_start, 6) == 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}
