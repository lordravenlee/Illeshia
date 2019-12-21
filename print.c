#include "global.h"

void print_token( token_types token_type, const char* token_string )
{ 
  switch (token_type)
  { 
	case CHECK:
	case USE:
	case SYSTEM:
    case ELSE:
    case READ:
    case WRITE:               printf(" reserved word: %s\n",buffer);          break;
 	case SEPERATOR:           printf(" %s\n", buffer);                        break;
	case OPERATOR:            printf(" %s\n", buffer);                        break;
    case NUMBER:              printf(" NUM, val= %s\n",buffer);               break;
    case IDENT:               printf(" IDENT, name= %s\n",buffer);            break;
    case ERROR:               printf(" ERROR: %s\n",buffer);                  break;
    default: /* should never happen */
      printf("Unknown token: %d\n",token_type);
  }
}

char * copy_string(char * s)
{ 
  int n;
  char * t;

  if (s == NULL)
  {
	  return NULL;
  }

  n = (int)strlen(s)+1;
  t = (char *)malloc(n);

  if (t == NULL)
  {
	  printf("Out of memory error at line %d\n", line_num);
  }
  else
  {
	  strcpy(t, s);
  }
  return t;
}

TreeNode * new_statement_node(Statement_Kind kind)
{
	TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));

	int i;

	if (t == NULL)
	{
		printf("Out of memory error at line %d\n", line_num);
	}
	else
	{
		for (i = 0; i < max_children; i++)
		{
			t->child[i] = NULL;
		}

		t->sibling = NULL;
		t->nodekind = StmtK;
		t->kind.stmt = kind;
		t->lineno = line_num;
	}
	return t;
}

TreeNode * new_expression_node(Expression_Kind kind)
{
	TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));

	int i;

	if (t == NULL)
	{
		printf("Out of memory error at line %d\n", line_num);
	}
	else
	{
		for (i = 0; i < max_children; i++)
		{
			t->child[i] = NULL;
		}

		t->sibling = NULL;
		t->nodekind = ExpK;
		t->kind.exp = kind;
		t->lineno = line_num;
		t->type = Hollow;
	}
	return t;
}

static int indentno = 0;
#define INDENT indentno+=2
#define UNINDENT indentno-=2

static void print_spaces(void)
{ 
  int i;

  for (i = 0; i < indentno; i++)
  {
	  printf(" ");
  }
}

void print_tree( TreeNode * tree )
{ 
  int i;

  INDENT;

  while (tree != NULL)
  {
    print_spaces();

    if (tree->nodekind==StmtK)
    { 
		switch (tree->kind.stmt) 
		{
        case checkK:                       printf("Check\n");                                      break;
        case readK:                     printf("Read: %s\n",tree->attr.name);                   break;
        case writeK:                    printf("write: %s\n", tree->attr.name);                 break;
		case predirectK:                printf("predirect: %s\n",tree->attr.name);              break;
		case identK:
			if (tree->type == Function)
			{
				printf(" function_type: %s\n", tree->attr.name);
			}
			if (tree->type == Assign)
			{
				printf("Assign: %s\n", tree->attr.name);
			}
			if (tree->type == Assign_Id)
			{
				printf("Id: %s\n", tree->attr.name);
			}
			break;
        default:
          printf("Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==ExpK)
    { 
		switch (tree->kind.exp) 
		{
        case OpK:
          printf("Op: ");
          print_token(tree->attr.op,"\0");
          break;
        case ConstK:
          printf("Const: %d\n",tree->attr.val);
          break;
        case IdK:
          printf("Id: %s\n",tree->attr.name);
          break;
        default:
          printf("Unknown ExpNode kind\n");
          break;
      }
    }
	else
	{
		printf("Unknown node kind\n");
	}
	for (i = 0; i < max_children; i++)
	{
		print_tree(tree->child[i]);
	}
    tree = tree->sibling;
  }
  UNINDENT;
}
