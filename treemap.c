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


TreeNode * createTreeNode(void* key, void * value) 
{
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

//IS_EQUAL retorna 1 si las claves son iguales y 0 si no lo son
//LOWER_THAN retorna 1 si la clave del primer parametro es menor que la del segundo parametro 

void insertTreeMap(TreeMap * tree, void* key, void * value) 
{
    if(tree == NULL || key == NULL || value == NULL) return;

    if(tree->root == NULL) 
    {        
        TreeNode *nuevo = createTreeNode(key, value);
        tree->root = nuevo;
        tree->current = nuevo;
        return;
    }
    
    TreeNode *aux = tree->root;
    TreeNode *nuevo = createTreeNode(key, value);

    while(aux != NULL)
    {
        if(is_equal(tree, aux->pair->key, key)) return; //iguales

        if(tree->lower_than(key, aux->pair->key) == 1) 
        { 
            if(aux->left == NULL)
            {
                aux->left = nuevo;
                nuevo->parent = aux;
                tree->current = aux->left;
                return;
            }
            else aux = aux->left;
        }
        else 
        {
            if(aux->right == NULL) //si la derecha esta vacia
            {
                aux->right = nuevo; //la derecha del aux es nuevo
                nuevo->parent = aux; //el padre de nuevo es aux,conecto
                tree->current = aux->right;
                return;
            }
            else aux = aux->right;
        }
    }    
}

/*
mplemente la función TreeNode * minimum(TreeNode * x). Esta función retorna el nodo con la mínima clave ubicado en el subárbol con raiz x. Para obtener el nodo tiene que, a partir del nodo x, irse por la rama izquierda hasta llegar al final del subárbol. Si x no tiene hijo izquierdo se retorna el mismo nodo.
*/

TreeNode * minimum(TreeNode * x)
{
    if (x == NULL) return NULL;
    
    while (x->left != NULL) x = x->left;
    
    return x;
}

/*
Implemente la función void removeNode(TreeMap * tree, TreeNode* node). Esta función elimina el nodo node del árbol tree. Recuerde que para eliminar un node existen 3 casos: Nodo sin hijos: Se anula el puntero del padre que apuntaba al nodo Nodo con un hijo: El padre del nodo pasa a ser padre de su hijo Nodo con dos hijos: Descienda al hijo derecho y obtenga el menor nodo del subárbol (con la función minimum). Reemplace los datos (key,value) de node con los del nodo "minimum". Elimine el nodo minimum (para hacerlo puede usar la misma función removeNode). 
*/

void removeNode(TreeMap * tree, TreeNode* node) 
{
    if(node == NULL || tree == NULL) return;

    if(node->left == NULL && node->right == NULL) //si no tiene hijos
    {
        if(node->parent == NULL) //si es raiz
        {
            tree->root = NULL;
            tree->current = NULL;
        }
        else //si no es raiz
        {
            if(node->parent->left == node) node->parent->left = NULL;
            else node->parent->right = NULL;
        }
    }

    if(node->left != NULL && node->right == NULL)//si tiene solo hijo izquierdo
    {
        if(node->parent == NULL) //si el nodo es raiz
        {
            tree->root = node->left;//el hijo izquierdo es la raiz
            node->left->parent = NULL;//el padre de nuevo raiz es NULL
        }
        else //si no es raiz
        {
            if(node->parent->left == node) node->parent->left = node->left; //el hijo izquierdo es el hijo izquierdo del padre
            else //si no es el hijo izquierdo del padre
            {
                node->parent->right = node->left;//el hijo izquierdo es el hijo derecho del padre
                node->left->parent = node->parent;//el padre de nuevo hijo izquierdo es el padre
            }
        }
    }

    if(node->right != NULL && node->left == NULL)//si tiene solo hijo derecho
    {
        if(node->parent == NULL) //si el nodo es raiz
        {
            tree->root = node->right;//el hijo izquierdo es la raiz
            node->right->parent = NULL;//el padre de nuevo raiz es NULL
        }
        else //si no es raiz
        {
            if(node->parent->right == node) node->parent->right = node->right; //el hijo izquierdo es el hijo izquierdo del padre
            else 
            {
                node->parent->left = node->right;
                node->left->parent = node->parent;
            }
        } 
    }

    if(node->left != NULL && node->right != NULL) //si tiene dos hijos 
    {//intercambio de datos, el menor del subarbol derecho es el nuevo padre
        TreeNode *min = minimum(node->right);
        node->pair->key = min->pair->key;
        node->pair->value = min->pair->value;
        removeNode(tree, min);
    }
    
    return;
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
        if(is_equal(tree, key, aux->pair->key))
        {
            tree->current = aux;
            return aux->pair;
        }
        if(tree->lower_than(key, aux->pair->key) == 1)
        {
            ub_node = aux;
            aux = aux->left;
        }

        else aux = aux->right;
    }
    tree->current = ub_node;
    return ub_node != NULL ? ub_node->pair : NULL;
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
    /*TreeNode *aux = tree->current;

    if(aux == NULL) return NULL;

    if(aux->right != NULL)
    {
        aux = minimum(aux->right);
        tree->current = aux;
        return aux->pair;
    }
    */
    return NULL;
    
}
