/*TOMA Bogdan-Nicolae-313CB*/
#include "trie.h"

int get_index(char c)
{
    //Intoarce pozitia din alfabet a literei c:
    if((int)c >= 97 && (int)c <= 122 )
        return (int)c - 97;
    
    if((int)c >= 65 && (int)c <= 90)
        return (int)c - 39;

    if((int)c >= 48 && (int)c <= 57)
        return (int)c + 4;
    
    if((int)c == 46)
        return (int)c + 16;
    
    if((int)c == 45)
        return (int)c + 18;

    if((int)c == 39)
        return (int)c + 25;
    
    if((int)c == 63)
        return (int)c + 2;
    
    if((int)c == 33)
        return (int)c + 33;
    
    if((int)c == 32)
        return (int)c + 35;
    return -1;
}
trienode *getNode()
{
    int i;
    //Alocam noul nod:
    trienode *node = NULL;
    node = (trienode *)malloc(sizeof(trienode));
  
    if (node)
    {
        node->leaf = 0;
        //Face fiecare copil al nodului NULL:
        for (i = 0; i < ALPHABET_SIZE; i++){
            node->children[i] = NULL;
        }
    }
    return node;
}
int insert(trienode *root, char *key, void* ae)
{
    int i, l = strlen(key), index;

    trienode *p = root;

    //Parcurgem arborele, iar daca o litera nu exista initializam un nod nou:
    for(i = 0; i < l; i++)
    {
        index = get_index(key[i]);

        if(!p->children[index]){
            p->children[index] = getNode();
        }
        p = p->children[index];
    }
    //In cazul in care cartea exista deja returnam 0:
    if(p->leaf == 1)
        return 0;
    p->leaf = 1;
    p->info = ae;

    return 1;
}

void insert_author(trienode *root, char *key, void* ae)
{
    int i, l = strlen(key), index;

    trienode *p = root;

    for(i = 0; i < l; i++){

        index = get_index(key[i]);
        if(!p->children[index])
            p->children[index] = getNode();

        p = p->children[index];
    }

    //Copiem informatia din ae intr-o noua carte pe care o inseram in arborele autorului:
    TBook* book = malloc(sizeof(TBook));
    book->title = calloc(51, sizeof(char));
    memcpy(book->title,((TBook*)ae)->title,strlen(((TBook*)ae)->title));

    book->author = calloc(41, sizeof(char));
    memcpy(book->author,((TBook*)ae)->author,strlen(((TBook*)ae)->author));

    book->rating = ((TBook*)ae)->rating;
    book->nr_of_pages = ((TBook*)ae)->nr_of_pages;

    if(p->leaf == 0){
        //Arborele autorului era gol
        p->info = getNode();
        insert(p->info,book->title,book);
        p->leaf = 1;
    }
    else{
        //Arborele autorului nu era gol
        insert(p->info,book->title,book);
    }
}

/*Functie de search care intoarce 0 sau 1 in functie de rezultat */
int search(trienode *root, char *key)
{
    if(!root)
        return 0;
    int i, l = strlen(key), index;
    trienode* p = root;

    for(i = 0; i < l; i++)
    {
        index = get_index(key[i]);
        if(!p->children[index])
            return 0;
        p = p->children[index];
    }
    if(p != NULL && p->leaf == 1)
        return 1;
    return 0;
}

int search_book(trienode *root, char *key, FILE * out)
{
    if(!root)
        return 0;
    
    int i, l = strlen(key), index;
    trienode* p = root;

    for(i = 0; i < l; i++)
    {
        index = get_index(key[i]);
        if(!p->children[index]){
            fprintf(out,"Cartea %s nu exista in recomandarile tale.\n",key);
            return 0;
        }
        p = p->children[index];
    }
    if(p != NULL && p->leaf == 1){
        fprintf(out,"Informatii recomandare: %s, %s, %d, %d\n",((TBook*)p->info)->title,((TBook*)p->info)->author,((TBook*)p->info)->rating, ((TBook*)p->info)->nr_of_pages);
        return 1;
    }
    fprintf(out,"Cartea %s nu exista in recomandarile tale.\n",key);
    return 0;
}
int search_book_pref(trienode *root, char *key, FILE * out)
{
    if(!root)
        return 0;
    
    int i , l = strlen(key), index, s = 0;
    trienode* p = root;

    //Parcurgem arborele pana la finalul prefixului:
    for(i = 0; i < l; i++)
    {
        index = get_index(key[i]);
        if(!p->children[index]){
            fprintf(out,"Nicio carte gasita.\n");
            return 0;
        }
        p = p->children[index];
    }

    show_books_pref(p,&s,out);

    return 0;
}

void show_books_pref(trienode* node, int *s, FILE* out)
{
    int i;
    //Verificam daca un nod este frunza
    if(!node)
        return;
    
    if(*s > 2)
        return;

    if(node->leaf == 1){
        (*s)++;
        fprintf(out,"%s\n",((TBook*)node->info)->title);
    }

    //Parcurgem lexicografic arborele:
    for(i = 0; i < ALPHABET_SIZE; i++){
        if(node->children[i]){
            show_books_pref(node->children[i],s,out);
        }
    }
}

void show_books(trienode* node, FILE* out)
{
    int i;
    if(!node)
        return;
    
    if(node->leaf == 1){
        fprintf(out,"%s\n",((TBook*)node->info)->title);
    }

    for(i = 0; i < ALPHABET_SIZE; i++){
        if(node->children[i]){
            show_books(node->children[i],out);
        }
    }
}

int list_author(trienode *root, char *key, FILE * out)
{
    if(!root)
        return 0;
    int i, l = strlen(key), index;
    trienode* p = root;

    for(i = 0; i < l; i++)
    {
        index = get_index(key[i]);
        if(!p->children[index]){
            fprintf(out,"Autorul %s nu face parte din recomandarile tale.\n",key);
            return 0;
        }
        p = p->children[index];
    }
    if(p != NULL && p->leaf == 1){
        show_books(p->info,out);
        return 1;
    }
    fprintf(out,"Autorul %s nu face parte din recomandarile tale.\n",key);
    return 0;
}

int list_author_pref(trienode *root, char *key, FILE * out)
{
    if(!root)
        return 0;
    int i, l = strlen(key), index, s = 0;
    trienode* p = root;

    for(i = 0; i < l; i++)
    {
        index = get_index(key[i]);
        if(!p->children[index]){
            fprintf(out,"Niciun autor gasit.\n");
            return 0;
        }
        p = p->children[index];
    }

    show_author_pref(p,&s,out);

    return 0;
}

void show_author_pref(trienode* node, int *s, FILE* out)
{
    int i, ok = 0;
    if(!node)
        return;

    if(*s > 2)
        return;
    
    if(node->leaf == 1){
        (*s)++;
        print_authors(node->info,&ok,out);
    }
    
    for(i = 0; i < ALPHABET_SIZE; i++){

        if(node->children[i]){
            show_author_pref(node->children[i],s,out);
        }
    }
}

/*Parcurge arborele autorului pana ajunge la un nod frunza si afiseaza autorul*/
void print_authors(trienode* node, int *ok, FILE* out)
{
    int i;
    if(!node || *ok == 1)
        return;
    
    if(node->leaf == 1){
        *ok = 1; 
        fprintf(out,"%s\n",((TBook*)node->info)->author);
    }

    for(i = 0; i < ALPHABET_SIZE; i++){
        if(node->children[i]){
            print_authors(node->children[i],ok,out);
        }
    }
}

int search_by_author_pref(trienode *root, char *key1, char *key2, FILE * out)
{
    if(!root)
        return 0;
    int i, l = strlen(key1), index;
    trienode* p = root;

    for(i = 0; i < l; i++)
    {
        index = get_index(key1[i]);
        if(!p->children[index]){
            fprintf(out,"Autorul %s nu face parte din recomandarile tale.\n",key1);
            return 0;
        }
        p = p->children[index];
    }
    if(p != NULL && p->leaf == 1){
        search_book_pref(p->info,key2,out);
        return 1;
    }
    fprintf(out,"Autorul %s nu face parte din recomandarile tale.\n",key1);
    return 0;
}

int search_by_author(trienode *root, char *key1, char *key2, FILE * out)
{
    if(!root)
        return 0;
    
    int i, l = strlen(key1), index;
    trienode* p = root;

    for(i = 0; i < l; i++)
    {
        index = get_index(key1[i]);

        if(!p->children[index]){
            fprintf(out,"Autorul %s nu face parte din recomandarile tale.\n",key1);
            return 0;
        }
        p = p->children[index];
    }

    if(p != NULL && p->leaf == 1){
        search_book(p->info,key2,out);
        return 1;
    }

    fprintf(out,"Autorul %s nu face parte din recomandarile tale.\n",key1);

    return 0;
}

int count_children(trienode * node)
{
    //Numara copiii unui nod:
    int nr = 0, i;
    if(!node)
        return 0;
    for(i = 0; i < ALPHABET_SIZE; i++){
        if(node->children[i])
            nr++;
    }
    return nr;
}

int delete_book(trienode **node, char* key, char* author)
{
    if (*node == NULL)
        return 0;
    
    //Daca string-ul se termina
    if (*key == '\0' && (*node)->leaf == 1)
    {
        strcpy(author, ((TBook*)(*node)->info)->author);
        free( ((TBook*)((*node)->info))->author );
        free( ((TBook*)((*node)->info))->title );
        free( (*node)->info );

        if (!count_children(*node))
        {
            //Daca nodul nu are copiii il stergem
            free(*node);
            (*node) = NULL;
            return 1;
        }
         else {
            (*node)->leaf = 0;
            return 0;
        }
    }
    //Parcurgem recursiv arborele, iar daca nodurile nu mai au alti copii le stergem:
    if(*key){
        delete_book(&((*node)->children[get_index(key[0])]),key + 1,author);
        if ( (*node)->leaf == 0)
        {
            if (!count_children(*node))
            {
                free(*node);
                (*node) = NULL;
                return 1;
            }
            else {
                return 0;
            }
        }
    }

    return 0;
}

int delete_from_author(trienode *root, char *key1, char *key2, FILE * out)
{
    if(!root)
        return 0;
    int i, l = strlen(key1), index;
    trienode* p = root;

    for(i = 0; i < l; i++)
    {
        index = get_index(key1[i]);
        p = p->children[index];
    }
    if(p != NULL && p->leaf == 1){
        trienode** T2 = (trienode**)&(p->info);
        delete_book(T2,key2,key1);
        if(!count_children(p->info)){
            return 1;
        }
    }
    return 0;
}
int delete_author(trienode **node, char* key)
{
    if (*node == NULL)
        return 0;
    
    //Daca string-ul se termina
    if (*key == '\0' && (*node)->leaf == 1)
    {
        free( (*node)->info );

        if (!count_children(*node))
        {
            //Daca nodul nu are alti copii il stergem
            free(*node);
            (*node) = NULL;
            return 1;
        }
         else {
            (*node)->leaf = 0;
            return 0;
        }
    }
    //Parcurgem recursiv arborele, iar daca nodurile nu mai au alti copii le stergem:
    if(*key){
        delete_author(&((*node)->children[get_index(key[0])]),key + 1);
        if ( (*node)->leaf == 0)
        {
            if (!count_children(*node))
            {
                free(*node);
                (*node) = NULL;
                return 1;
            }
            else {
                return 0;
            }
        }
    }

    return 0;
}

void free_books(trienode* node)
{
    int i;
    if(!node) 
        return;

    // Parcurgere recursiva, ajunge la finalul arborelui 
    for (i = 0; i < ALPHABET_SIZE; i++){
        if(node->children[i]){
            free_books(node->children[i]);
        }
    }

    // Daca nodul este frunza, eliberam memoria ecoupata de carte
    if(node->leaf == 1){
        free( ((TBook*)node->info)->author );
        free( ((TBook*)node->info)->title );
        free(node->info);
    }
    free(node);
    node = NULL;
}
void free_authors(trienode* node)
{
    int i;
    if(!node) return;

    //Parcurgere recursiva, ajunge la finalul arborelui 
    for (i = 0; i < ALPHABET_SIZE; i++){
        if(node->children[i]){
        free_authors(node->children[i]);
        }
    }

    // Daca nodul este frunza elibereaza arborele memorat in info
    if(node->leaf == 1){
        free_books(node->info);
    }
    free(node);
    node = NULL;
}