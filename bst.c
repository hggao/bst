#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct _queue {
   int size;
   int first;
   int last;
   int count;
   void *buffer[0];
} Queue;



typedef struct _bstree BSTree;
struct _bstree {
   int value;
   BSTree *left;
   BSTree *right;
};

#define NUM 15
int arrs_sorted[NUM] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
char bitmap[10][130];

Queue *create_queue(int size)
{
   Queue *que;
   
   que = malloc(sizeof(Queue) + sizeof(void*) * size);
   assert(que != NULL);
   que->size = size;
   que->first = 0;
   que->last = 0;
   que->count = 0;

   return que;
}

void destroy_queue(Queue *que)
{
   free(que);
}

void enqueue(Queue *que, void *value)
{
   assert(que->count < que->size);

   que->buffer[que->last] = value;
   que->last = (que->last + 1) % que->size;
   que->count++;
}

void *dequeue(Queue *que)
{
   void *value = NULL;

   if (que->count > 0) {
      value = que->buffer[que->first];
      que->first = (que->first + 1) % que->size;
      que->count--;
   }

   return value;
}

int get_queue_count(Queue *que)
{
   return que->count;
}

BSTree *create_bst_from_sorted_array(int arr[], int start, int end)
{
   int mid;
   BSTree *root = NULL;

   if (start > end) {
      return NULL;
   }

   mid = (start + end) / 2;
   root = malloc(sizeof(BSTree));
   assert(root != NULL);
   root->value = arr[mid];
   root->left = create_bst_from_sorted_array(arr, start, mid - 1);
   root->right = create_bst_from_sorted_array(arr, mid + 1, end);

   return root;
}

void destroy_bst(BSTree *root)
{
   if (root == NULL) {
      return;
   }

   if (root->left != NULL) {
      destroy_bst(root->left);
   }
   if (root->right != NULL) {
      destroy_bst(root->right);
   }
   free(root);
}

int bst_depth(BSTree *root)
{
   int lDepth, rDepth;

   if (root == NULL) {
      return 0;
   }
   lDepth = bst_depth(root->left);
   rDepth = bst_depth(root->right);
   return MAX(lDepth, rDepth) + 1;
}

void visit_print_bst(BSTree *root, int row, int col, int w)
{
   int i, j, len;
   char buffer[5];

   if (root == NULL) {
      return;
   }

   if (root->left != NULL) {
      for (j = col - w/2; j < col; j++) {
         bitmap[row][j] = '_';
      }
   }
   if (root->right != NULL) {
      for (j = col; j < col + w/2; j++) {
         bitmap[row][j] = '_';
      }
   }
   len = sprintf(buffer, "%2d", root->value);
   for (i = 0, j = col - 1; i < len; i++, j++) {
      bitmap[row][j] = buffer[i];
   }

   visit_print_bst(root->left, row+1, col - w/2, w/2);
   visit_print_bst(root->right, row+1, col + w/2, w/2);
}

int print_bst(BSTree *root)
{
   int tw, depth, i, j;
   Queue *que;
   BSTree *tmp;

   for (i = 0; i < 10; i++) {
      for (j = 0; j < 130; j++) {
         bitmap[i][j] = ' ';
      }
   }
   depth = bst_depth(root);
   printf("====== depth = %d ======\n", depth);
   if (depth == 0) {
      bitmap[0][62] = 'N';
      bitmap[0][63] = 'U';
      bitmap[0][64] = 'L';
      bitmap[0][65] = 'L';
   } else {
      tw = 4 * (1 << (depth - 1));
      visit_print_bst(root, 0, 64, tw/2);
   }

   for (i = 0; i < 10; i++) {
      for (j = 0; j < 130; j++) {
         printf("%c", bitmap[i][j]);
      }
      printf("\n");
   }

   return 0;
}

int main(void)
{
    int ret;
    BSTree *root, **pptr = NULL, *node = NULL;

    root = create_bst_from_sorted_array(arrs_sorted, 0, NUM-1);
    printf("====== The BST ======\n");
    print_bst(root);

    destroy_bst(root);
    return 0;
}
