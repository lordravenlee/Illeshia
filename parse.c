#include "global.h"

static token_types token;
static TreeNode * stmt_sequence(void);
static TreeNode * statement(void);
static TreeNode * close_paren_stmt();
static TreeNode * predirect_stmt(void);
static TreeNode * if_stmt(void);
static TreeNode * ident_stmt(void);
static TreeNode * read_stmt(void);
static TreeNode * write_stmt(void);
static TreeNode * _exp(void);
static TreeNode * simple_exp(void);
static TreeNode * term(void);
static TreeNode * factor(void);
token_types get_token();
static void syntax_error(const char * message);
static void match(token_types expected);


TreeNode * stmt_sequence(void)
{ 
  TreeNode * t = statement();
  TreeNode * p = t;
  
  while ((token_type != EOF) && (token_type != ELSE))
  {
	  TreeNode * q;

	  if (token_type != OPEN_PAREN && token_type != CLOSE_PAREN)
	  {
		  if (token_type != LBRACKET && token_type != RBRACKET)
		  {
			  match(SEMICOLON);
			  printf(" STATEMENT TEST");
		  }
	  }
	  else
	  {
		  if (token_type == OPEN_PAREN)
		  {
			  match(OPEN_PAREN);
			  printf(" ARGUMENT OPEN TEST");
		  }
		  else
		  {
			  match(CLOSE_PAREN);
			  printf(" ARGUMENT CLOSE TEST");
		  }
	  }
	  if (token_type == ELSE)
	  {
		  continue;
	  }

	  q = statement();

	  if (q != NULL)
	  {
		  if (t == NULL)
		  {
			  t = p = q;
		  }
		  else /* now p cannot be NULL either */
		  {
			  p->sibling = q;
			  p = q;
		  }
	  }
  }
  return t;
}

TreeNode * statement(void)
{ 
  TreeNode * t = NULL;

  switch (token_type)
  {
	case PREDIRECT:                         t = predirect_stmt();                     break;
	case IDENT:                             t = ident_stmt();                         break;
    case CHECK :                            t = if_stmt();                            break;
    case READ :                             t = read_stmt();                          break;
    case WRITE :                            t = write_stmt();                         break;
	case CLOSE_PAREN:                       t = close_paren_stmt();                   break;
    default : 
		syntax_error(" unexpected token -> ");       
		print_token(token_type, buffer);
		token_type = get_token();   break;
  } 
  return t;
}

TreeNode * close_paren_stmt()
{
	TreeNode * t = new_statement_node(identK);
	return t;
}

TreeNode * predirect_stmt()
{
	TreeNode * t = new_statement_node(predirectK);

	token_type = get_token();
	
	match(USE || TRANSLATE);
	match(PERIOD);
	match(SYSTEM || HAVEN);
	match(PERIOD);
	match(S_TYPE || M_TYPE || N_TYPE || IO);

	strcpy(token_buffer, buffer);
	t->attr.name = copy_string(token_buffer);
	return t;

	//char str_buf[100];
	//match(USE);
	//match(PERIOD);
	//strcpy(str_buf, buffer);
	//match(SYSTEM);
	//strcat(str_buf, "->");
	//match(SEPERATOR);
	//strcat(str_buf, buffer);
	//match(IDENT);
	//t->attr.name = copyString(str_buf);
	//return t;
}


TreeNode * ident_stmt()
{
	TreeNode * t = new_statement_node(identK);

	char ident_buffer[1000];
	char ass_buf[100];
	strcpy(ass_buf, buffer);

	match(IDENT);

	if (token_type == IDENT)
	{
		strcpy(ident_buffer, ident_string);

		match(IDENT);

		if (token_type == OPEN_PAREN)
		{
			match(OPEN_PAREN);
			t->type = Function;
			t->attr.name = copy_string(ident_buffer);

			if (token_type != CLOSE_PAREN)
			{
				int k = 0;

				do
				{
					if (k)
					{
						match(SEPERATOR);
					}
					char _buf[100];
					strcpy(_buf, buffer);
					match(IDENT);
					strcat(_buf, " ");
					strcat(_buf, buffer);
					match(IDENT);
					TreeNode * p = new_expression_node(IdK);

					if (p != NULL)
					{
						p->attr.name = copy_string(_buf);
						p->type = Function;
						t->child[k] = p;

					}
					k++;
				} while (token_type == SEPERATOR);
			}
			match(CLOSE_PAREN);
			return t;
		}
		else if (token_type == SEMICOLON)
		{
			return t;
		}
		else
		{
			while (token_type != SEMICOLON)
			{
				match(SEPERATOR);
				match(IDENT);
			}
			return t;
		}
	}
	else if (token_type == EQUALIZER)
	{
		t->type = Assign;
		t->attr.name = copy_string(ass_buf);
		match(EQUALIZER);

		TreeNode * p = new_expression_node(IdK);
		if (p != NULL)
		{
			p->attr.name = copy_string(buffer);
			p->type = Assign_Id;
			t->child[0] = p;

		}
		match(IDENT);
		if (token_type == OPERATOR)
		{
			match(OPERATOR);

			if (t != NULL)
			{
				t->child[1] = _exp();
			}
		}
		else
		{
			match(OPEN_PAREN);
			match(IDENT);
			match(COMMA || PERIOD);
			match(IDENT);
			match(CLOSE_PAREN);
		}

	}
	else
	{
		match(NUMBER);
	}
	return t;
}





TreeNode * if_stmt(void)
{
	TreeNode * t = new_statement_node(checkK);

	match(CHECK);
	match(OPEN_PAREN);

	if (t != NULL)
	{
		t->child[0] = _exp();
	}

	match(CLOSE_PAREN);
	match(LBRACKET);

	if (t != NULL)
	{
		t->child[1] = stmt_sequence();
	}
	match(RBRACKET);
	if (token_type == ELSE)
	{
		match(ELSE);
		match(LBRACKET);
		if (t != NULL)
		{
			t->child[2] = stmt_sequence();
		}
		match(RBRACKET);
	}
	return t;
}

TreeNode * read_stmt(void)
{
	TreeNode * t = new_statement_node(readK);

	char str_buf[100];

	strcpy(str_buf, " ");

	match(READ);
	match(OPEN_PAREN);
	match(STRAND);
	int k = 0;
	while (token_type != STRAND)
	{
		k++;
		strcat(str_buf, buffer);
		match(OPERATOR);
		strcat(str_buf, buffer);
		match(IDENT);
	}
	strcat(str_buf, "->");
	match(STRAND);
	for (int i = 0; i < k; i++)
	{
		match(SEPERATOR);
		strcat(str_buf, buffer);
		match(OPERATOR);
		strcat(str_buf, buffer);
		match(IDENT);
	}
	match(CLOSE_PAREN);
	t->attr.name = copy_string(str_buf);
	return t;
}

TreeNode * write_stmt(void)
{
	TreeNode * t = new_statement_node(writeK);
	char str_buf[100];
	strcpy(str_buf, "");
	match(WRITE);
	match(OPEN_PAREN);
	match(STRAND);
	int k = 0;
	while (token_type != STRAND)
	{
		if (token_type == EQUALIZER)
		{
			strcat(str_buf, buffer);
			strcat(str_buf, " ");
			match(EQUALIZER);
			continue;
		}
		else if (token_type == OPERATOR)
		{
			strcat(str_buf, buffer);
			strcat(str_buf, " ");
			match(OPERATOR);
			k++;
		}
		else if (token_type == SEPERATOR)
		{
			strcat(str_buf, buffer);
			strcat(str_buf, " ");
			match(SEPERATOR);
			continue;
		}
		strcat(str_buf, buffer);
		strcat(str_buf, " ");
		match(IDENT);
	}
	match(STRAND);
	for (int i = 0; i < k; i++)
	{
		match(SEPERATOR);
		match(IDENT);
	}
	match(CLOSE_PAREN);
	// if (t!=NULL) t->child[0] = exp();
	t->attr.name = copy_string(str_buf);
	return t;
}


TreeNode * _exp(void)
{
	TreeNode * t = simple_exp();

	if ((token_type == EQUALIZER))
	{
		TreeNode * p = new_expression_node(OpK);

		if (p != NULL)
		{
			p->child[0] = t;
			p->attr.op = token_type;
			t = p;
		}
		match(token_type);

		if (t != NULL)
		{
			t->child[1] = simple_exp();
		}
	}
	return t;
}

TreeNode * simple_exp(void)
{
	TreeNode * t = term();

	while (token_type == OPERATOR)
	{
		TreeNode * p = new_expression_node(OpK);

		if (p != NULL)
		{
			p->child[0] = t;
			p->attr.op = token_type;
			t = p;
			match(token_type);
			t->child[1] = term();
		}
	}
	return t;
}

TreeNode * term(void)
{
  TreeNode * t = factor();

  while (token_type == OPERATOR)
  { 
	TreeNode * p = new_expression_node(OpK);

    if (p!=NULL) 
	{
      p->child[0] = t;
      p->attr.op = token_type;
      t = p;
      match(token_type);
      p->child[1] = factor();
    }
  }
  return t;
}

TreeNode * factor(void)
{ 
  TreeNode * t = NULL;

  switch (token_type)
  {
    case NUMBER :

      t = new_expression_node(ConstK);

	  if ((t != NULL) && (token_type == NUMBER))
	  {
		  t->attr.val = atoi(buffer);
	  }
      match(NUMBER);
      break;
    case IDENT :
      t = new_expression_node(IdK);
	  if ((t != NULL) && (token_type == IDENT))
	  {
		  t->attr.name = copy_string(ident_buffer);
	  }
      match(IDENT);
      break;
    case LPAREN :
      match(LPAREN);
      t = _exp();
      match(RPAREN);
      break;
    default:
      syntax_error("unexpected token -> ");
      print_token(token_type,buffer);
	  token_type = get_token();
      break;
    }
  return t;
}

static void syntax_error(const char * message)
{
	printf("\n >>> ");
	printf(" Syntax error at line %d: %s", line_num, message);
	Error = TRUE;
}

static void match(token_types expected)
{
	if (token_type == expected)
	{
		token_type = get_token();
	}
	else
	{
		syntax_error(" unexpected token -> ");
		print_token(token_type, buffer);
		printf("      ");
	}
}

TreeNode * parse(void)
{
  TreeNode * t;

  token_type = get_token();

  t = stmt_sequence();

  return t;
}
