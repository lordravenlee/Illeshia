#include "global.h"

token_types check_word(char t[]);

int function_depth;
token_types token_type;


static struct
{
	const char* str;
	token_types tok;
} reservedWords[max_reserved] = { {"use",USE},{"system",SYSTEM},{"translate",TRANSLATE},{"haven", HAVEN},
{"s_type", S_TYPE}, {"m_type", M_TYPE},{ "n_type", N_TYPE}, { "io", IO},{"num", NUM}, {"mark", MARK},
{"deci", DECI}, {"decii", DECII}, {"quad", QUAD}, {"strand", STRAND}, {"hollow", HOLLOW},{"check", CHECK},
{"else", ELSE}, {"loop", LOOP}, {"set", SET}, {"construct", CONSTRUCT}, { "bridge", BRIDGE}, {"list", LIST},
{"read", READ}, {"write", WRITE}, {"sofi", SOFI}, {"file", FILER} };

token_types check_word(char t[])
{
	int i;

	for (i = 0; i < max_reserved; i++)
	{
		if (!strcmp(t, reservedWords[i].str))
		{
			printf(" %03d:   %s\t              reserved - %d\n", line_num, t, reservedWords[i].tok);
			return reservedWords[i].tok;
		}
	}
	return check_ident(buffer);
}

token_types check_ident(char s[])
{
	strcpy(ident_string, s);
	printf(" %03d:   %s\t              Identifier\n", line_num, ident_string);
	return IDENT;
}


token_types get_token()
{
	while (get_char(file, buffer, 1000 + 1))
	{
		line_num++;

		switch (token_type)
		{
		case SPACE:             token_type = get_token();                                                                    break;
		case NELI:              token_type = get_token();                                                                    break;
		case WORD:              token_type = check_word(buffer);                                                             break;

		case OPEN_PAREN:        printf(" %03d:   %s\t              Lparen\n", line_num, buffer);                             break;
		case CLOSE_PAREN:       printf(" %03d:   %s\t              Rparen\n", line_num, buffer);                             break;
		case OPERATOR:          printf(" %03d:   %s\t              Operator\n", line_num, buffer);                           break;
		case EQUALIZER:         printf(" %03d:   %s\t              Equalizer\n", line_num, buffer);                          break;
		case NUMBER:            printf(" %03d:   %s\t              Number\n", line_num, buffer);                             break;
		case STRAND:            printf(" %03d:   %s\t              Strand_Literal\n", line_num, buffer);                     break;

		case PERIOD:            printf(" %03d:   %s\t              Period\n", line_num, buffer);                             break;
		case COMMA:             printf(" %03d:   %s\t              Comma\n", line_num, buffer);                              break;
		case SEPERATOR:         printf(" %03d:   %s\t              Separator\n", line_num, buffer);
			if (!strcmp(buffer, ";"))
			{
				return SEMICOLON;
			}
			if (!strcmp(buffer, "{"))
			{
				return OPEN_PAREN;
			}
			if (!strcmp(buffer, "}"))
			{
				return CLOSE_PAREN;
			}
			break;

		case UNKNOWN:                                                                                                        break;
		default:
			break;
		}
	}
	return token_type;
}

token_types get_char(FILE *file, char *const buf, const int max_token)
{
	int length = 0;
	int ch = 0;
	int i = 0;
	int j = 0;
	int value;


	buffer[length++] = ch;
	buffer[length] = 0;

	if (operators(ch))     return OPERATOR;
	if (equalizer(ch))     return EQUALIZER;
	if (number(ch))        return NUMBER;
	if (strand(ch))        return STRAND;

	// check and calculate current function deep
	if (lbracket(ch))      function_depth++;        return LBRACKET;
	if (rbracket(ch))      function_depth--;        return RBRACKET;

	if (lparen(ch))		   return OPEN_PAREN;
	if (rparen(ch))	       return CLOSE_PAREN;
	if (space(ch))         return SPACE;
	if (neli(ch))          return NELI;
	if (period(ch))        return PERIOD;
	if (comma(ch))         return COMMA;

	while ((ch = fgetc(file)) != EOF && length < max_token)
	{
		if (!letter(ch))
		{
			ungetc(ch, file);
			break;
		}
		buffer[length++] = ch;
	}
	buffer[length] = 0;
	return WORD;
}
