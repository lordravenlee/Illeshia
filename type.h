
#define increment(ch)           ((ch == '+' && ch == '+'))
#define decrement(ch)           ((ch == '-' && ch == '-'))
#define mixture(ch)             (letter(ch) || number(ch))

#define TRUE 1
#define FALSE 0
#define max_reserved 40
#define max_children 3
#define max_token_length 40

static int letter(int ch)
{
	return (((ch >= 'a') && (ch <= 'z')) || (ch >= 'A') && (ch <= 'Z') || (ch == '_'));
}

static int number(int ch)
{
	return ((ch >= '0') && (ch <= '9'));
}

static int operators(int ch)
{
	return ((ch == '+') || (ch == '-') || (ch == '*') || (ch == '/') || (ch == '%') || (ch == '&') || (ch == '<') || (ch == '>'));
}

static int equalizer(int ch)
{
	return ((ch == '=') || (ch == '!'));
}

static int comma(int ch)
{
	return ((ch == ','));
}

static int period(int ch)
{
	return ((ch == '.'));
}

static int space(int ch)
{
	return ((ch == ' '));
}

static int neli(int ch)
{
	return ((ch == '\n'));
}

static int tab(int ch)
{
	return ((ch == '\t'));
}

static int strand(int ch)
{
	return ((ch == '\"'));
}


static int single_quote(int ch)
{
	return ((ch == '\''));
}

static int double_quote(int ch)
{
	return ((ch == '\"'));
}

static int nuller(int ch)
{
	return ((ch == '\0'));
}

static int seperator(int ch)
{
	return ((ch == '#') || (ch == '{') || (ch == '}') || (ch == '[') || (ch == ']') || (ch == '\\')
		|| (ch == '.') || (ch == ',') || (ch == ':') || (ch == ';'));
}

static int lbracket(int ch)
{
	return ((ch == '{'));
}

static int rbracket(int ch)
{
	return ((ch == '}'));
}


static int lparen(int ch)
{
	return ((ch == '('));
}

static int rparen(int ch)
{
	return ((ch == ')'));
}







