/**********************************************************************
* File Name: avl.c          					    				  *
* Name: Uriel Naiman								    			  *
* Create Date: 2.8.2020		    									  *
* Reviewed by: Itai Adi- Yosef        							 	  *
* Topic: AVL C file                									  *
**********************************************************************/
#include <stdio.h>          /* NULL */
#include <stdlib.h>         /* malloc, free*/
#include <assert.h>			/* assert*/

#include "avl.h"
#define SUCCESS 0
#define FAIL 1

typedef enum
{
    left = 0,
    right = 1
} child;

typedef struct avl_node
{
    void* data;         /* pointer to user data (key inside) */
    struct avl_node *child[2];      /* pointer to child array */
    int height;                 /* hight of node */
}node_t;

struct avl
{
    node_t *root;           /* pointer to root of tree */
    avl_cmp_func_t cmp;     /* compare func pased by user at creation */
    void *param;            /* parameter to be passed to cmp func if needed */
};

/* recursive functions */
     
static node_t *InsertRec(avl_t *avl, node_t *root, void *data);
static node_t *RemoveRec(avl_t *avl, node_t *root, void *data);
static void DestroyRec(node_t *root);
static void *FindRec(const avl_t *avl, node_t *root, const void *data);
static int ForEachRec(node_t *root, avl_do_action_func_t do_action,
                            void *param, int status);
static size_t SizeRec(node_t *root);

/* util functions */

static node_t *NewNode(void *data);     /* creates new node for tree */
static int Height(node_t* node);        /* gets height of a node */
static int Max(int left, int right);    /* returns greater value 
												between two heights */
static node_t *RotateRight(avl_t *avl, node_t *prev);   /* right rotation for 
															balancing tree */
static node_t *RotateLeft(avl_t *avl, node_t *prev);    /* left rotation 
														for balancing tree */
static int GetBalance(node_t *node);        /* calculates balance of two 
															children nodes */
static void PrintPreOrderRec(node_t *root); /* prints tree in pre 
															order travers */
static node_t *MinNode(node_t *root);       /* retrurns smallest 
														value node (Begin) */

/* API functions */
/*---------------------------------------------------------------------------*/

avl_t *AVLCreate(avl_cmp_func_t cmp, void *param)
{
    avl_t *avl = (avl_t*) malloc(sizeof(avl_t));
    if(NULL == avl)
    {
        return (NULL);
    }

    avl->root = NULL;
    avl->cmp = cmp;
    avl->param = param;

    return(avl);
}

/*---------------------------------------------------------------------------*/

void AVLDestroy(avl_t *avl)
{
    assert(avl);
    
    DestroyRec(avl->root);
    
    free (avl);
    avl = NULL;
}

/*---------------------------------------------------------------------------*/

size_t AVLSize(const avl_t *avl)
{
    assert(avl);
    
    return (SizeRec(avl->root));
}

/*---------------------------------------------------------------------------*/

int AVLIsEmpty(const avl_t *avl)
{
    assert(avl);
    return (avl->root == NULL);
}

/*---------------------------------------------------------------------------*/

int AVLInsert(avl_t *avl, void *data)
{
	assert(avl);

    return (InsertRec(avl, avl->root, data) == NULL);
}

/*---------------------------------------------------------------------------*/

void AVLRemove(avl_t *avl, void *data)
{
    assert(avl);

	RemoveRec(avl, avl->root, data);
}

/*---------------------------------------------------------------------------*/

void *AVLFind(const avl_t *avl, const void *data)
{
	assert(avl);

    return (FindRec(avl, avl->root, data));
}


/*---------------------------------------------------------------------------*/

int AVLForEach(avl_t *avl, avl_do_action_func_t do_action ,void *param)
{
    int status = 0;
    
    return (!AVLIsEmpty(avl) && !ForEachRec(avl->root, do_action, param, status));
    
}

/*----------------------------------------------------------*/

/* util test function reachable from test file */
void PrintPreOrder(avl_t *avl)
{
    node_t *root = avl->root;
    printf("\n");
    if (NULL != root)
    {
        printf("%d ", *((int*)root->data));
        PrintPreOrderRec(root->child[0]);
        PrintPreOrderRec(root->child[1]);
    }
    printf("\n");
}

/*----------------------------------------------------------*/

/* util functions */
static node_t *NewNode(void *data)
{
    node_t *new_node = (node_t*) malloc(sizeof(node_t));
    if (NULL == new_node)
    {
        return (NULL);
    }

    new_node->data = data;
    new_node->child[0] = new_node->child[1] = NULL;
    new_node->height = 1;

    return (new_node);
}

/*----------------------------------------------------------*/

static node_t *InsertRec(avl_t *avl, node_t *root, void *data)
{
    node_t *new_node = NULL;
    int balance = 0;
    int child_ptr = 0;

    if (NULL == root)
    {
        new_node = NewNode(data);
        
        if (AVLIsEmpty(avl))
        {
            avl->root = new_node;
        }
        return (new_node);
    }

    if (avl->cmp(data, root->data, NULL) == 0)
    {
        return (root);    
    }
    
    child_ptr = avl->cmp(data, root->data, NULL) > 0;
    root->child[child_ptr] = InsertRec(avl, root->child[child_ptr], data);
   
    root->height = Max(Height(root->child[0]), Height(root->child[1])) + 1;
    balance = GetBalance(root);

    if (balance > 1 && avl->cmp(data, root->child[0]->data, NULL) < 0)
    {
        return (RotateRight(avl, root));
    }

    if (balance < -1 && avl->cmp(data, root->child[1]->data, NULL) > 0)
    {
        return (RotateLeft(avl, root));
    }

    if (balance > 1 && avl->cmp(data, root->child[0]->data, NULL) > 0)
    {
        root->child[0] = RotateLeft(avl, root->child[0]);
        return (RotateRight(avl, root));
    }

    if (balance < -1 && avl->cmp(data, root->child[1]->data, NULL) < 0)
    {
        root->child[1] = RotateRight(avl, root->child[1]);
        return (RotateLeft(avl, root));
    }

    return (root);
}

/*----------------------------------------------------------*/

static node_t *RemoveRec(avl_t *avl, node_t *root, void *data)
{
    node_t *temp = NULL;
    int balance = 0;
    int balance_l = 0;
    int balance_r = 0;
    int child_ptr = 0;
    
    if (NULL == root)
    {
        return (root);
    }

    if (avl->cmp(data, root->data, NULL) == 0)
    {
        if (NULL == root->child[0] || NULL == root->child[1])
        {
            temp = (root->child[1] ? root->child[1] : root->child[0]);

            if (NULL == temp)
            {
                temp = root;
                root = NULL;
            }
            else
            {
                root->data = temp->data;
            }
            free (temp);
        }
        else
        {
            temp = MinNode(root->child[1]);
            root->data = temp->data;
            root->child[1] = RemoveRec(avl, root->child[1], data);
        }
    }

    if (NULL == root)
    {
        return (root);
    }
    
    child_ptr = avl->cmp(data, root->data, NULL) > 0;
    root->child[child_ptr] = RemoveRec(avl, root->child[child_ptr], data);
    
    if (NULL == root)
    {
        return (root);
    }    

    root->height = Max(Height(root->child[0]), Height(root->child[1]) +1);
    balance = GetBalance(root);
    balance_l = GetBalance(root->child[0]);
    balance_r = GetBalance(root->child[1]);


    if (balance > 1 && balance_l >= 0)
    {
        return (RotateRight(avl, root));
    }

    if (balance < -1 && balance_r <= 0)
    {
        return (RotateLeft(avl, root));
    }

    if (balance > 1 && balance_l < 0)
    {
        root->child[0] = RotateLeft(avl, root->child[0]);
        return (RotateRight(avl, root));
    }

    if (balance < -1 && balance_r > 0)
    {
        root->child[1] = RotateRight(avl, root->child[1]);
        return (RotateLeft(avl, root));
    }

    return (root);
}

/*----------------------------------------------------------*/

static void *FindRec(const avl_t *avl, node_t *root, const void *data)
{
    int child_ptr = 0;

    if (NULL == root->data)
    {
        return (NULL);
    }

    if (avl->cmp(data, root->data, NULL) == 0)
    {
        return (root->data);
    }
    
    child_ptr = avl->cmp(data, root->data, NULL) > 0;
    
    return (FindRec(avl, root->child[child_ptr], data));
}

/*----------------------------------------------------------*/

static int ForEachRec(node_t *root, avl_do_action_func_t do_action,
                            void *param, int status)
{
    
    if (status)
    {
        return (status);
    }

    if (root->child[0])
    {
        status = ForEachRec(root->child[0], do_action, param, status);
    }
    
    status = do_action(root->data, param);

    if (root->child[1] != NULL)
    {
        status = ForEachRec(root->child[1], do_action, param, status);
    }

    return (status);
}

/*----------------------------------------------------------*/

static size_t SizeRec(node_t *root)
{
    if(NULL == root)
    {
        return (0);
    } 

    return (SizeRec(root->child[0]) + SizeRec(root->child[1]) +1);
}

/*----------------------------------------------------------*/

static int Height(node_t* node)
{
    if (NULL == node)
    {
        return (SUCCESS);
    }
    return (node->height);
}

/*----------------------------------------------------------*/

static int Max(int left, int right)
{
    return ((left > right) ? left : right);
}

/*----------------------------------------------------------*/

static node_t *RotateRight(avl_t *avl, node_t *curr)
{
    int flag = 0;
    node_t *prev = curr->child[0];
    node_t *prev2 = prev->child[1];

    if (curr == avl->root)
    {
        flag = 1;
    }

    prev->child[1] = curr;
    curr->child[0] = prev2;

    curr->height = Max(Height(curr->child[0]), Height(curr->child[1])+1);
    prev->height = Max(Height(prev->child[0]), Height(prev->child[1])+1);

    if (flag)
    {
        avl->root = prev;
    }

    return (prev);
}

/*----------------------------------------------------------*/

static node_t *RotateLeft(avl_t *avl, node_t *curr)
{
    int flag = 0;
    node_t *prev = curr->child[1];
    node_t *prev2 = prev->child[0];

    if (curr == avl->root)
    {
        flag = 1;
    }

    prev->child[0] = curr;
    curr->child[1] = prev2;

    curr->height = Max(Height(curr->child[0]), Height(curr->child[1])+1);
    prev->height = Max(Height(prev->child[0]), Height(prev->child[1])+1);

    if (flag)
    {
        avl->root = prev;
    }

    return (prev);
}

/*----------------------------------------------------------*/

static int GetBalance(node_t *node)
{
    if (NULL == node)
    {
        return (SUCCESS);
    }
    return (Height(node->child[0]) - Height(node->child[1]));
}

/*----------------------------------------------------------*/

static void PrintPreOrderRec(node_t *root)
{
    if (NULL != root)
    {
        printf("%d ", *((int*)root->data));
        PrintPreOrderRec(root->child[0]);
        PrintPreOrderRec(root->child[1]);
    }
}

static node_t *MinNode(node_t *root)
{
    node_t *temp = root;

    while (temp->child[0])
    {
        temp = temp->child[0];
    }
    return (temp);
}

static void DestroyRec(node_t *root)
{
    
    if(NULL == root)
    {
    	return;
    }
    
    if (!root->child[0] && !root->child[1])
    {
        free(root);
        root = NULL;
        return;
    }

    if (root->child[0])
    {
        DestroyRec(root->child[0]);
    }
    
    if (root->child[1])
    {
        DestroyRec(root->child[1]);
    }
    
    free(root);
    root = NULL;

}

