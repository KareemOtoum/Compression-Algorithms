#include <stdio.h>
#include <stdlib.h>

#define STRING_LENGTH 256

typedef struct Node
{
    int value;
    char letter;
    struct Node *left;
    struct Node *right;
} RootNode;

struct Node* createNode(char c)
{
    struct Node* node = malloc(sizeof(struct Node));
    node->letter = c;
    node->value = 1;  // Initialize frequency to 1
    node->left = NULL;
    node->right = NULL;
    return node;
}

void printTree(struct Node *tree[], int size) {
    printf("\n");
    for (int i = 0; i < size; i++) {
        if (tree[i] != NULL) {
            printf("Tree node character: %c, frequency: %d\n", tree[i]->letter, tree[i]->value);
        }
    }
}

struct Node* searchTree(struct Node *tree[], char c, int size) {
    for (int i = 0; i < size; i++) {
        if (tree[i] != NULL && tree[i]->letter == c) {
            return tree[i];
        }
    }
    return NULL;
}

int main()
{
    FILE *fptr = fopen("/root/CompressionTechniques/huffman.txt", "r");

    if(fptr == NULL)
    {
        printf("file opening error");
        exit(1);
    }

    struct Node *treeElements[STRING_LENGTH] = { NULL };

    char c;
    int i = 0;
    while((c = fgetc(fptr)) != EOF)
    {
        printf("%c", c);
        struct Node *existingNode = searchTree(treeElements, c, STRING_LENGTH);
        if(existingNode != NULL)
        {
            existingNode->value++;
        }
        else
        {
            if (i >= STRING_LENGTH) {
                fprintf(stderr, "Error: Exceeded maximum tree size.\n");
                break;
            }
            struct Node *newNode = createNode(c);
            treeElements[i] = newNode;
            ++i;
        }
    }

    printTree(treeElements, STRING_LENGTH);

    for(int i = 0; i < STRING_LENGTH; i++)
     {
        if(treeElements[i] == NULL) continue;
        free(treeElements[i]);
    }
    fclose(fptr);
    return 0;
}