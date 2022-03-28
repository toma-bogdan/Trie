/*TOMA Bogdan-Nicolae-313CB*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE (68)

typedef struct trienode
{
    struct trienode *children[ALPHABET_SIZE];
    int leaf;
    void* info;
    
}trienode;

typedef struct{
    char* title;
    char* author;
    int rating;
    int nr_of_pages;

}TBook;

int get_index(char c);
trienode *getNode();
int insert(trienode *root, char *key, void* ae);
void insert_author(trienode *root, char *key, void* ae);
int search(trienode *root, char *key);
int search_book(trienode *root, char *key, FILE * out);
int search_book_pref(trienode *root, char *key, FILE * out);
int search_by_author(trienode *root, char *key1, char *key2, FILE * out);
void show_books(trienode* node, FILE* out);
void show_books_pref(trienode* node, int *s, FILE* out);
int list_author(trienode *root, char *key, FILE * out);
int list_author_pref(trienode *root, char *key, FILE * out);
int search_by_author_pref(trienode *root, char *key1, char *key2, FILE * out);
void show_author_pref(trienode* node, int *s, FILE* out);
void print_authors(trienode* node, int *ok, FILE* out);
int count_children(trienode * node);
int delete_book(trienode **node, char* key, char* author);
int delete_from_author(trienode *root, char *key1, char *key2, FILE * out);
int delete_author(trienode **node, char* key);
void free_books(trienode* curs);
void free_authors(trienode* node);
