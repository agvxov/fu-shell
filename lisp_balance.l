%{
    #include "lisp_balance.h"
    static int paren_stack = 0;

    static char * lb_input;
	static int         len;
	static int      offset;

    void lb_set_input(char * const s) {
        lb_input = s;
        len      = strlen(lb_input);
        offset   = len;
    }

	#define YY_INPUT(buf, result, max_size) {                        \
		int cpi = (offset && offset > max_size) ? max_size : offset; \
		memcpy(buf, lb_input+(len-offset), cpi);                     \
		result = cpi;                                                \
		offset = (cpi > offset) ? 0 : offset - cpi;                  \
	}
%}

%option noyywrap
%option nodefault

%x LITERAL
%%
<INITIAL>{
\(      { ++paren_stack; }
\)      {
            if (paren_stack >= 0) {
                --paren_stack;
            } else {
                return BROKEN;
            }
        }
\"      { BEGIN LITERAL; }
.|\n    { ; }
<<EOF>> {
            if (paren_stack == 0) {
                return FULL;
            } else {
                return PARTIAL;
            }
        }
}

<LITERAL>{
\"      { BEGIN INITIAL; }
.|\n    { ; }
<<EOF>> { return PARTIAL; }
}
%%
