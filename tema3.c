/*TOMA Bogdan-Nicolae-313CB*/
#include <stdio.h>
#include "trie.h"

TBook * create_book()
{
    TBook * book = malloc(sizeof(TBook));

    book->title = calloc(51, sizeof(char));
    book->author = calloc(41, sizeof(char));

    char * title = strtok(NULL,":");
    strcpy(book->title,title);

    char * author = strtok(NULL,":");
    strcpy(book->author,author);

    char * rating = strtok(NULL,":");
    book->rating = atoi(rating);

    char * nr_of_pages = strtok(NULL,":");
    book->nr_of_pages = atoi(nr_of_pages);

    return book;
}

int main(int argc, char **argv)
{
    char filename1[50], filename2[50];

	strcpy(filename1,argv[1]);
    strcpy(filename2,argv[2]);

	FILE *in = fopen(filename1,"rt");
    if(in == NULL){
        printf("eroare deschidere fisier");
        return -1;
    }
    FILE *out = fopen(filename2,"wr");
    if(out == NULL){
        printf("eroare deschidere fisier");
        return -1;
    }
    
    char * line = NULL;
	size_t len = 0;
    trienode * Trie_books = getNode();
    trienode * Trie_authors = getNode();

    while (getline(&line, &len, in) != -1){
        char * comand = strtok(line," ");
        if(comand[strlen(comand) - 1] == '\n'){
               comand[strlen(comand) - 1] = '\0';
        }

        if(strcmp(comand,"add_book") == 0){
            TBook * book = create_book();

            int rez =insert(Trie_books,book->title,book);

            if(!rez){
                free(book->title);
                free(book->author);
                free(book);
            }else
            {
                insert_author(Trie_authors,book->author,book);
            }
            
        }

        if(strcmp(comand,"search_book") == 0){
            char *key = strtok(NULL,"\n");

            if(key[strlen(key) - 1] == '~'){
                key[strlen(key) - 1] = '\0';
                search_book_pref(Trie_books,key,out);
            }
            else{
                search_book(Trie_books,key,out);
            }
        }

        if(strcmp(comand,"list_author") == 0){
            char *author = strtok(NULL,"\n");

            if(author[strlen(author) - 1] == '~'){
                author[strlen(author) - 1] = '\0';
                list_author_pref(Trie_authors,author,out);
            }
            else{
                list_author(Trie_authors,author,out);
            }
        }

        if(strcmp(comand,"search_by_author") == 0){
            char * aut = strtok(NULL,":\n");
            if(aut[strlen(aut) - 1] == '~'){
                aut[strlen(aut) - 1] = '\0';
                
                list_author_pref(Trie_authors,aut,out);
            }else
            {
                char * title = strtok(NULL,"\n");
                if(title[strlen(title) - 1] == '~'){
                    title[strlen(title) - 1] = '\0';
                    search_by_author_pref(Trie_authors,aut,title,out);
                }
                else{
                search_by_author(Trie_authors,aut,title,out);
                }
            }
        }

        if(strcmp(comand,"delete_book") == 0){
            char * title = strtok(NULL,"\n");
            int rez = search(Trie_books,title);
            if(rez){

                char * author = calloc(41, sizeof(char));
                delete_book(&Trie_books,title,author);

                int r = delete_from_author(Trie_authors,author,title,out);
                
                if(r){
                    delete_author(&Trie_authors,author);
                }
                free(author);
            }else
            {
                fprintf(out,"Cartea %s nu exista in recomandarile tale.\n",title);
            }
            
        }

    }
    free(line);
    free_books(Trie_books);
    free_authors(Trie_authors);
    fclose(in);
    fclose(out);

    return 0;
}
