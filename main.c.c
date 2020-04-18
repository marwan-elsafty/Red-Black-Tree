#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLACK 0
#define RED 1



typedef struct node
{
    struct node * parent;
    struct node * left;
    struct node * right;
    int color;
    char word[30];
} node;

typedef struct
{
    node * root;
    int size;
    int height;
} RBtree;





node *  newNode (char newWord[30]);
void destructNode(node * currNode);

void insert(RBtree * tree, node * root, char newWord[30]);
void deleteRB(RBtree * tree, char word[30]);
node * search(RBtree * tree, char word[30]);

void rotateLeft(RBtree * tree, node * x);
void rotateRight(RBtree * tree, node * x);
void fixInsertion(RBtree * tree, node * z);
void fixDelete(RBtree * tree, node * x);

void rbTransplant(RBtree * tree, node * u, node * v);
node * findSuccessor( RBtree * tree, node * currNode);
node * minimum(node * node);






node *  newNode (char newWord[30])
{
    node * newNode = malloc(sizeof(node));
    newNode->color= RED;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    strcpy(newNode->word,newWord);

    return newNode;
}


void destructNode(node * currNode)
{
    currNode->left = NULL;
    currNode->right = NULL;
    currNode->parent = NULL;
    free(currNode);
    return;
}





void insert(RBtree * tree, node * root, char newWord[30])
{
    // Looking for the right place to insert the new word
    if(root == NULL)
    {
        root = newNode(newWord);
        fixInsertion(tree,root);
        return;
    }
    else if( strcmp( newWord, root->word ) < 0)
        insert(tree, root->left, newWord );
    else
        insert(tree, root->right, newWord );
}




void deleteRB(RBtree * tree, char word[30])
{

    // find the node containing key
    node * z = NULL;
    node * root = tree->root;
    node *x, *y;
    x = (node *)malloc(sizeof(node));
    while (root != NULL)
    {
        if (strcmp(word, root->word) == 0)
        {
            z = root;
        }

        if (strcmp(root->word,word) < 0)
        {
            root = root->right;
        }
        else
        {
            root = root->left;
        }
    }

    if (z == NULL)
    {
        printf("ERROR: word not found in dictionary");
        return;
    }

    y = z;
    int y_original_color = y->color;
    if (z->left == NULL)
    {
        x = z->right;
        rbTransplant(tree, z, z->right);
    }
    else if (z->right == NULL)
    {
        x = z->left;
        rbTransplant(tree, z, z->left);
    }
    else
    {
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z)
        {
            x->parent = y;
        }
        else
        {
            rbTransplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        rbTransplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (y_original_color == BLACK)
    {
        fixDelete(tree, x);
    }
}



node * search(RBtree * tree, char word[30])
{

    node * root = tree->root;

     while (root != NULL)
    {
        if (strcmp(word, root->word) == 0)
        {
            return root;
        }

        if (strcmp(word, root->word) < 0)
        {
            root = root->left;
        }
        else
        {
            root = root->right;
        }
    }
`
    printf("There is no such word");

    return;

}









void fixInsertion(RBtree * tree, node * z)
{
    node * uncle;

    while(z->color == RED)
    {
        //if Z's parent is a left child
        if(z->parent == z->parent->parent->left)
        {
            //get Z's uncle
            uncle = z->parent->parent->right;

            //if Z's uncle is red
            //set Z's parent and uncle to black
            // and Z's grandparent to Red
            // continue doing so till you reach the root
            if(uncle->color == RED)
            {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            // If uncle is black and z is a right child
            else if(z == z->parent->right)
            {
                z = z->parent;
                rotateLeft(tree,z);
            }
            z->parent->color = BLACK;
            z->parent->parent->color = RED;
            rotateRight(tree,z->parent->parent);
        }
        else if( z->parent == z->parent->parent->right)
        {
            //get Z's uncle
            uncle = z->parent->parent->left;

            //if Z's uncle is red
            //set Z's parent and uncle to black
            // and Z's grandparent to Red
            // continue doing so till you reach the root
            if(uncle->color == RED)
            {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            // If uncle is black and z is a right child
            else if(z == z->parent->left)
            {
                z = z->parent;
                rotateRight(tree,z);
            }
            z->parent->color = BLACK;
            z->parent->parent->color = RED;
            rotateLeft(tree,z->parent->parent);

        }
    }
    return;

}



void fixDelete(RBtree * tree, node * x)
{
    node * s;
    while (x != tree->root && x->color == BLACK)
    {
        if (x == x->parent->left)
        {
            s = x->parent->right;
            if (s->color == RED)
            {
                // case 3.1
                s->color = BLACK;
                x->parent->color = RED;
                rotateLeft(tree,x->parent);
                s = x->parent->right;
            }

            if (s->left->color == BLACK && s->right->color == BLACK)
            {
                // case 3.2
                s->color = RED;
                x = x->parent;
            }
            else
            {
                if (s->right->color == BLACK)
                {
                    // case 3.3
                    s->left->color = BLACK;
                    s->color = RED;
                    rotateRight(tree, s);
                    s = x->parent->right;
                }

                // case 3.4
                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->right->color = BLACK;
                rotateLeft(tree,x->parent);
                x = tree->root;
            }
        }
        else
        {
            s = x->parent->left;
            if (s->color == RED)
            {
                // case 3.1
                s->color = BLACK;
                x->parent->color = RED;
                rotateRight(tree,x->parent);
                s = x->parent->left;
            }

            if (s->right->color == BLACK && s->right->color == BLACK)
            {
                // case 3.2
                s->color = RED;
                x = x->parent;
            }
            else
            {
                if (s->left->color == BLACK)
                {
                    // case 3.3
                    s->right->color = BLACK;
                    s->color = RED;
                    rotateLeft(tree,s);
                    s = x->parent->left;
                }

                // case 3.4
                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->left->color = BLACK;
                rotateRight(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}





void rotateLeft(RBtree * tree, node * x)
{
    //get y
    node * y = x->right;

    //set X's right child to Y's left child
    x->right = y->left;

    //if Y's left isn't NULL then set its parent to X
    if(y->left != NULL)
        y->left->parent = x;

    //set Y's parent to X's parent
    y->parent = x->parent;

    //if X has no parent then it's the root
    //then set Y as the root
    if(x->parent == NULL)
        tree->root = y;

    //if X has a parent then identify wether
    //X is the left or right child and set
    //Y accordingly
    else if(x->parent->left == x)
        x->parent->left = y;
    else
        x->parent->right = y;

    //set X as Y's left child
    y->left = x;
    //set Y as X's parent
    x->parent = y;

    return;
}


void rotateRight(RBtree * tree, node * x)
{
    //get y
    node * y = x->left;

    //set X's right child to Y's left child
    x->left = y->right;

    //if Y's left isn't NULL then set its parent to X
    if(y->right != NULL)
        y->right->parent = x;

    //set Y's parent to X's parent
    y->parent = x->parent;

    //if X has no parent then it's the root
    //then set Y as the root
    if(x->parent == NULL)
        tree->root = y;

    //if X has a parent then identify wether
    //X is the left or right child and set
    //Y accordingly
    else if(x->parent->left == x)
        x->parent->left = y;
    else
        x->parent->right = y;

    //set X as Y's left child
    y->right = x;
    //set Y as X's parent
    x->parent = y;

    return;
}





//Function to find the successor of a given tree node
node * findSuccessor( RBtree * tree, node * currNode)
{
    node *succ, *root;
    root = tree->root;
    succ = malloc(sizeof(node));


    while (root != NULL)
    {
        if ( strcmp(currNode->word, root->word) == 0 )
            root = root->right;

        else if ( strcmp(currNode->word, root->word) < 0  || root->right == NULL)
        {
            succ = root;
            root = root->left;

        }
        else if (strcmp(currNode->word, root->word) > 0 || root->left == NULL)
            root = root->right;


        else
            break;
    }
    return succ;
}


void rbTransplant(RBtree * tree, node * u, node * v)
{
    if (u->parent == NULL)
    {
        tree->root = v;
    }
    else if (u == u->parent->left)
    {
        u->parent->left = v;
    }
    else
    {
        u->parent->right = v;
    }
    v->parent = u->parent;
}


// FUNCTION TO FIND MINIMUM NODE IN THE SUBTREE
node * minimum(node * node)
{
    while (node->left != NULL)
    {
        node = node->left;
    }
    return node;
}






