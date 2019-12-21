#include "global.h"

int length;
int line_num;
int max_token;
int n;
int in;
int Error;


int main(void)
{

	TreeNode * syntax_tree;

	file = fopen("source.txt", "r");

	get_token();

	syntax_tree = parse();

	printf("\nSyntax tree:\n");
	print_tree(syntax_tree);
	

	fclose(file);

	return 0;
}
