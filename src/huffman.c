#include <stdio.h>
#include <stdlib.h>

#define STRING_LENGTH 256

typedef struct Node
{
    int frequency;
    char letter;
    struct Node *left;
    struct Node *right;
} Node;

struct Node* createNode(char c)
{
    struct Node* node = malloc(sizeof(struct Node));
    node->letter = c;
    node->frequency = 1;  // Initialize frequency to 1
    node->left = NULL;
    node->right = NULL;
    return node;
}

void printTree(struct Node **nodes, size_t size) { // moves backward as last node in array of pointers is the first node
    printf("\n");
    for(size_t i = 0; i < 0; i++)
    {
        if(nodes[i] == NULL) continue;
        if(nodes[i]->left == NULL && nodes[i]->right == NULL) continue;
        if(nodes[i]->left != NULL)
        {
            printf("Left Node letter: %c frequency:%d\t\t", nodes[i]->left->letter, nodes[i]->left->frequency);
        }
        if(nodes[i]->right != NULL)
        {
            printf("Right Node letter: %c frequency:%d\t\t", nodes[i]->right->letter, nodes[i]->right->frequency);
        }
        printf("\n");
    }
}

struct Node* searchTree(struct Node *tree[], char c, int size) {
    for (int i = 0; i < size; i++) {
        if(tree[i] == NULL) continue;
        if (tree[i]->letter == c) {
            return tree[i];
        }
    }
    return NULL;
}

struct Node* find_freqorhigher(struct Node *tree[], size_t size, size_t frequency, struct Node *excludingNodes[])
{
    if(frequency > size) return NULL;

    for(size_t i = 0; i < size; i++)
    {
        if(tree[i] == NULL) continue;
        if(tree[i]->frequency == frequency && excludingNodes == NULL) return tree[i];
        else if (excludingNodes != NULL && tree[i]->frequency == frequency)
        {
            for(size_t j = 0; j < size; j++)
            {
                if(excludingNodes[j] == tree[i]) goto exit_loop;
            }
            return tree[i];
        }
        
    }
    exit_loop:
    return find_freqorhigher(tree, size, frequency+1, excludingNodes);
}

struct Node** setup_tree(struct Node *tree[], size_t size) // returns pointer to allocated nodes
{
    size_t currFrequency = size; // find smallest frequency node
    struct Node *lowestFreqNode = NULL;
    for(size_t i = 0; i < size; i++)
    {
        if(tree[i] == NULL) continue;
        if(tree[i]->frequency < currFrequency)
        {
            lowestFreqNode = tree[i];
            currFrequency = tree[i]->frequency;
        }
    }

    if(lowestFreqNode == NULL) return NULL;

    struct Node **newNodes = malloc(sizeof(Node*) * size);
    for(size_t i = 0; i < size; i++)
    {
        newNodes[i] = malloc(sizeof(Node));
        newNodes[i]->left = NULL;
        newNodes[i]->right = NULL;
    }

    struct Node *nextLowestNode;
    struct Node *excludingNodes[STRING_LENGTH] = {lowestFreqNode, nextLowestNode};

    newNodes[0]->right = lowestFreqNode; // set first nodes right to lowest node
    if((nextLowestNode = find_freqorhigher(tree, size, currFrequency, excludingNodes)) != NULL)
    {
        newNodes[0]->left = nextLowestNode;
    }
    else
    {
        return newNodes;
    }

    size_t excludingNodesIndex = 2;
    size_t newNodesIndex = 2;

    struct Node* lastLetterNode = nextLowestNode;
    struct Node* nextLetterNode;
    while((nextLetterNode = find_freqorhigher(tree, size, 0, excludingNodes)) != NULL)
    {
        printf("found new node\n");
        newNodes[newNodesIndex]->left = nextLetterNode;
        newNodes[newNodesIndex]->right = newNodes[newNodesIndex-1];

        excludingNodes[excludingNodesIndex] = nextLetterNode;
        lastLetterNode = nextLetterNode;

        ++excludingNodesIndex;
        ++newNodesIndex;
    }

    return newNodes;
}

int main()
{
    FILE *fptr = fopen("/root/Compression-Algorithms/huffman.txt", "r");

    if(fptr == NULL)
    {
        printf("file opening error\n");
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
            existingNode->frequency++;
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

    printf("\n");
    struct Node **newNodes = setup_tree(treeElements, STRING_LENGTH);

    printTree(newNodes, STRING_LENGTH);
    printf("%c\n", newNodes[0]->left->letter);
    

    for(int i = 0; i < STRING_LENGTH; i++)
     {
        if(treeElements[i] == NULL) continue;
        free(treeElements[i]);
    }
    for(size_t i = 0; i < STRING_LENGTH; i++)
    {
        if(newNodes[i] != NULL) free(newNodes[i]);
    }
    free(newNodes);
    fclose(fptr);
    return 0;
}