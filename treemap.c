#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap 
{
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

/*
Implemente la función createTreeMap en el archivo treemap.c. Esta función recibe la función de comparación de claves y crea un mapa (TreeMap) inicializando sus variables. El siguiente código muestra como inicializar la función de comparación. Reserve memoria, inicialice el resto de variables y retorne el mapa
*/

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) 
{
    TreeMap *arbol = (TreeMap *)malloc(sizeof(TreeMap));
    arbol->root = NULL;
    arbol->current = NULL;
    arbol->lower_than = lower_than;
    return arbol;
}

/*
 Implemente la función void insertTreeMap(TreeMap * tree, void* key, void * value). Esta función inserta un nuevo dato (key,value) en el árbol y hace que el current apunte al nuevo nodo. Para insertar un dato, primero debe realizar una búsqueda para encontrar donde debería ubicarse. Luego crear el nuevo nodo y enlazarlo. Si la clave del dato ya existe retorne sin hacer nada (recuerde que el mapa no permite claves repetidas).
*/


void insertTreeMap(TreeMap * tree, void* key, void * value) 
{
    //TreeNode *nuevo = createTreeNode(key, value);
    //TreeNode *aux = tree->root;

    return;
}

TreeNode * minimum(TreeNode * x)
{

    return NULL;
}


void removeNode(TreeMap * tree, TreeNode* node) {

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

/*
Implemente la función Pair* searchTreeMap(TreeMap* tree, void* key), la cual busca el nodo con clave igual a key y retorna el Pair asociado al nodo. Si no se encuentra la clave retorna NULL. Recuerde hacer que el current apunte al nodo encontrado.
*/

Pair * searchTreeMap(TreeMap * tree, void* key) 
{
    TreeNode *aux = tree->root;

    while(aux != NULL)
    {
        if(is_equal(tree, key, aux->pair->key)) 
        {
            tree->current = aux;
            return aux->pair;
        }
        if(tree->lower_than(key, aux->pair->key)) aux = aux->left;
        else aux = aux->right;
    }
    return NULL;
}


/*
 La función Pair* upperBound(TreeMap* tree, void* key) retorna el Pair con clave igual a key. En caso de no encontrarlo retorna el primer par asociado a una clave mayor o igual a key. Para implementarla puede realizar una búsqueda normal y usar un puntero a nodo auxiliar ub_node que vaya guardando el nodo con la menor clave mayor o igual a key. Finalmente retorne el par del nodo ub_node.
*/
    
Pair * upperBound(TreeMap * tree, void* key) 
{
    TreeNode * aux = tree->root;
    TreeNode * ub_node = NULL;
    

    while(aux != NULL)
    {
        if(is_equal(tree, aux->pair->key, key))
        {
            return aux->pair;
        }
        else 
        {
            if(is_equal(tree, aux->pair->key, key) == 1)
            {
                ub_node = aux;
                aux = aux->left;
            }
            else aux = aux->right;
        }
    }

    if(ub_node != NULL) return ub_node->pair;
    else return NULL;
}

/*
Implemente las funciones para recorrer la estructura: Pair* firstTreeMap(TreeMap* tree) retorna el primer Pair del mapa (el menor). Pair* nextTreeMap(TreeMap* tree) retornar el siguiente Pair del mapa a partir del puntero TreeNode* current. Recuerde actualizar este puntero.
*/

//IS_EQUAL retorna 1 si las claves son iguales y 0 si no lo son
//LOWER_THAN retorna 1 si la clave del primer parametro es menor que la del segundo parametro 


Pair * firstTreeMap(TreeMap * tree) 
{
    TreeNode *aux = tree->root;
    if(aux == NULL) return NULL;

    while(aux->left != NULL) aux = aux->left;

    tree->current = aux;
    return aux->pair;
}

    
Pair * nextTreeMap(TreeMap * tree) 
{
    TreeNode *aux = tree->current;
    if(aux == NULL) return NULL;

    while(aux != NULL)
    {
        if(aux->right == NULL)
        {
            tree->current = NULL;
            return NULL;
        }
        else 
        {
            tree->current = aux->right;
            aux = aux->right;
        }
    }
    return aux->pair;
        
}
