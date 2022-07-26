#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define MAXWORD 34
FILE *in;

struct node // структура для представления узлов дерева
{
	int count;
	char *word;
	unsigned char height;
	node* left;
	node* right;
	node(char *w) { count = 1; word = _strdup(w); left = right = 0; height = 1; }
};

node *root = NULL, *cur = NULL;

unsigned char height(node* p)
{
	return p ? p->height : 0;
}

int bfactor(node* p)
{
	return height(p->right) - height(p->left);
}

void fixheight(node* p)
{
	unsigned char hl = height(p->left);
	unsigned char hr = height(p->right);
	p->height = (hl > hr ? hl : hr) + 1;
}

node* rotateright(node* p) // правый поворот вокруг p
{
	node* q = p->left;
	p->left = q->right;
	q->right = p;
	fixheight(p);
	fixheight(q);
	return q;
}

node* rotateleft(node* q) // левый поворот вокруг q
{
	node* p = q->right;
	q->right = p->left;
	p->left = q;
	fixheight(q);
	fixheight(p);
	return p;
}

node* balance(node* p) // балансировка узла p
{
	fixheight(p);
	if (bfactor(p) == 2)
	{
		if (bfactor(p->right) < 0)
			p->right = rotateright(p->right);
		return rotateleft(p);
	}
	if (bfactor(p) == -2)
	{
		if (bfactor(p->left) > 0)
			p->left = rotateleft(p->left);
		return rotateright(p);
	}
	return p; // балансировка не нужна
}

node* insert(node* p, char *word, bool second) // вставка слова word в дерево с корнем p
{
	if (p == NULL)
	{
		if (second == 1)
			return new node(word);
		else
			return 0;
	}
	int cond = strcmp(word, p->word);
	if (cond == 0)
	{
		if (second == 1)
			p->count++;
	}
	else if (cond < 0)
		p->left = insert(p->left, word, second);
	else
		p->right = insert(p->right, word, second);
	return balance(p);
}

void treeprint(node *p)
{
	if (p != NULL)
	{
		treeprint(p->left);
		if (p->count)
			printf("%d - %s\n", p->count, p->word);
		treeprint(p->right);
	}
}

int comment(int c)
{
	if ((c = fgetc(in)) == '/')
	{
		while ((c = fgetc(in)) != '\n')
			;
		return 1;
	}
	else if (c == '*')
	{
		while ((c = fgetc(in)) != EOF)
		{
			if (c == '*')
			{
				if ((c = fgetc(in)) == '/')
				{
					break;
				}
			}
		}
		return 1;
	}
	return 0;
}

void new_word(char *word)
{
	int c = 0, i = 1;
	while ((c = fgetc(in)) != EOF && (isalpha(c) != 0 || isalnum(c) != 0 || c == '_') && i < 34)
	{
		word[i] = c;
		i++;
	}
	word[i] = '\0';
}

bool type_compare(char *word)
{
	if (strncmp(word, "int", 3) == 0) return 1;
	if (strncmp(word, "char", 4) == 0) return 1;
	if (strncmp(word, "struct", 6) == 0) return 1;
	if (strncmp(word, "if", 2) == 0) return 1;
	if (strncmp(word, "while", 5) == 0) return 1;
	if (strncmp(word, "for", 3) == 0) return 1;
	if (strncmp(word, "else", 4) == 0) return 1;
	if (strncmp(word, "void", 4) == 0) return 1;
	if (strncmp(word, "FILE", 4) == 0) return 1;
	if (strncmp(word, "h", 1) == 0) return 1;
	if (strncmp(word, "sizeof", 6) == 0) return 1;
	if (strncmp(word, "break", 5) == 0) return 1;
	if (strncmp(word, "return", 6) == 0) return 1;
	return 0;
}

void second_pass(char *word, int c)
{
	while ((c = fgetc(in)) != EOF)
	{
		if (c == '/')
		{
			comment(c);
		}
		if (c == '"' || c == '\'')
		{
			while ((c = fgetc(in)) != '"' && c != '\n' && c != '\'')
				;
		}
		if (isalpha(c) == 0 && c != '_')
			continue;
		word[0] = c;
		new_word(word);
		if (word[0] != '\0' && type_compare(word) == 0)
			root = insert(root, word, 1);
	}
}

int main()
{
	in = fopen("C:\\Users\\Михаил\\Desktop\\data struct\\LB4\\Tree\\Debug\\input.txt", "rb");
	if (in == NULL)
	{
		printf("error open");
	}
	else
	{
		int c = 0;
		char word[MAXWORD];
		second_pass(word, c);
		treeprint(root);
	}

	getchar();
	return 0;
}