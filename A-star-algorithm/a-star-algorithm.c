/*

Prakkash Manohar
Under Graduate (2nd Yr)
Computer Science and Engineering
Indian Institute of Information Technology,
Sri City, Chittoor, Andhra Pradesh, India - 517646

https://in.linkedin.com/in/manoharprakkash

*/

#include<stdio.h>
#include<stdlib.h>

typedef struct node{
    int puzzle[4][4];
    int g, h;
    int x, y;           //  position of empty square
    struct node *parent;
    struct node *next;
}node;

int global = 0;
int nodes_generated, nodes_in_memory;          //  nodes_in_memory = no. of nodes in the closed list at the end of algorithm

int goal[4][4]={1,2,3,4,
                5,6,7,8,
                9,10,11,12,
                13,14,15,0};

node *openList = NULL;
node *closedList = NULL;

/*********************************** PRIORITY QUEUE OPERATIONS ****************************/

void insertIntoList(node **list, node *a){
    node *trav = *list;
    if(trav == NULL)
        *list = a;

    else{
        node *prev = *list;
        while(trav != NULL){
            if((a->g + a->h) < (trav->g + trav->h)){
                if(trav == *list){           //  INSERTION AT BEGINNING IN NON-EMPTY LIST
                    a->next = trav;
                    *list = a;
                }
                else{                       //  INSERTION IN MIDDLE
                    a->next = trav;
                    prev->next = a;
                }

                return;
            }
            else{
                prev = trav;
                trav = trav->next;
            }
        }
        // INSERTION AT END
        prev->next = a;
    }
   /* else{
        while(trav != NULL && (a->g + a->h) > (trav->g + trav->h) )
            trav = trav->next;
        a->next = trav->next;
        trav->next = a;
    }*/

    //printf("insert completed...............");
}

node *createNewNode(){
    node *a = (node *)malloc(sizeof(node));
    return a;
}

node *deleteFirstNode(node **list_address){
    node *trav = *list_address;
    *list_address = (*list_address)->next;
    trav->next = NULL;
    return trav;
}

void deleteNode(node *prev, node **list_address){
    if(prev->next == NULL){
        free(prev);
        *list_address = NULL;
    }

    else{
        node *temp = prev->next;
        prev->next = prev->next->next;
        free(temp);
    }
    //printf("delete completed...............");
}

int countNodes(node *list){
	node *trav = list;
	int c=0;
	while(trav != NULL){
		trav = trav -> next;
		c++;
	}
	return c;
}

void printMatrix(int a[4][4]){
    int i,j;
    for(i=0;i<4;i++){
        for(j=0;j<4;j++)
            printf("%d ",a[i][j]);
        printf("\n");
    }
}

void printNodes(node *list){
	node *trav = list;int i,j;
	while(trav != NULL){
        printMatrix(trav->puzzle);
		trav = trav -> next;
	}
}

/******************************************************************************************/

int compareMatrix(int a[4][4], int b[4][4]){
    int i,j,c;
    for(i=0,c=0;i<4;i++){
        for(j=0;j<4;j++){
            if(a[i][j] == b[i][j])
                c++;
        }
    }

    if(c == 16)
        return 1;
    else
        return 0;
}

node *searchInList(node *list, node *a){
    node *trav = list;
    if(trav == NULL){
    //printf("search1 completed...............");
        return NULL;}
    while(trav->next != NULL){
        if(compareMatrix(trav->next->puzzle, a->puzzle) == 1){
    //printf("search2 completed...............");
            return trav; }                       //  RETURNS THE PREVIOUS NODE OF THE FOUND NODE IF NODES >= 2 IN THE LIST
        trav = trav -> next;
        //printf("hi...................");
    }
    if(compareMatrix(trav->puzzle, a->puzzle) == 1){    //  RETURNS THE FOUND NODE IF ONLY 1 NODE EXISTS IN THE LIST
    //printf("search3 completed...............");
        return trav;}
    //printf("search completed...............");
    return NULL;
}

void operateOn(node *child){
    node *search = searchInList(openList, child);   //  RETURNS THE PREVIOUS NODE OF THE FOUND NODE IF NODES >= 2 IN THE LIST
    if(search == NULL)             //   ENTIRELY NEW NODE / ALREADY DOESN'T EXIST IN OPEN LIST
        insertIntoList(&openList, child);
    else{                          //   ALREADY EXISTS IN OPEN LIST
        if((search->g)+(search->h) > (child->g)+(child->h)){
            deleteNode(search, &openList);
            insertIntoList(&openList, child);
        }
    }

    search = searchInList(closedList, child);   //  RETURNS THE PREVIOUS NODE OF THE FOUND NODE IF NODES >= 2 IN THE LIST
    if(search != NULL){             //   ALREADY EXISTS IN CLOSED LIST
        if((search->g)+(search->h) > (child->g)+(child->h)){
            deleteNode(search, &closedList);
            insertIntoList(&openList, child);
        }
    }
    //printNodes(child);
    //printf("printing a child after operateOn() is complete\n\n");
}

void swapNumbers(int *a, int *b){
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void *expandParent(node *parent, int o){
    node *n0, *n1, *n2, *n3;
    n0 = n1 = n2 = n3 = NULL;
    int i,j;

    //printNodes(parent);
    //printf("this is the parent\n\n");

    if(parent->x >=1){              //  EMPTY SQUARE MOVES 1 ROW UP
        n0 = createNewNode();
        n0->next = NULL;
        nodes_generated++;
        n0->parent = parent;
        n0->g = (parent->g)+1;

        for(i=0;i<4;i++){
            for(j=0;j<4;j++){
                n0->puzzle[i][j] = parent->puzzle[i][j];
            }
        }
        swapNumbers(&(n0->puzzle[parent->x][parent->y]), &(n0->puzzle[(parent->x) - 1][parent->y]));

        n0->x = (parent->x) - 1;
        n0->y = (parent->y);

        if(o == 1)
            n0->h = calculateh1(n0->puzzle);
        else if(o == 2)
            n0->h = calculateh2(n0->puzzle);
    }
    //printNodes(n0);
    //printf("child 0 above\n");

    if(parent->x <=2){              //  EMPTY SQUARE MOVES 1 ROW DOWN
        n1 = createNewNode();
        n1->next = NULL;
        nodes_generated++;
        n1->parent = parent;
        n1->g = (parent->g)+1;

        for(i=0;i<4;i++){
            for(j=0;j<4;j++){
                n1->puzzle[i][j] = parent->puzzle[i][j];
            }
        }
        swapNumbers(&(n1->puzzle[parent->x][parent->y]), &(n1->puzzle[(parent->x) + 1][parent->y]));

        n1->x = (parent->x) + 1;
        n1->y = (parent->y);

        if(o == 1)
            n1->h = calculateh1(n1->puzzle);
        else if(o == 2)
            n1->h = calculateh2(n1->puzzle);
    }
    //printNodes(n1);
    //printf("child 1 above\n");

    if(parent->y >=1){              //  EMPTY SQUARE MOVES 1 COLUMN LEFT
        n2 = createNewNode();
        n2->next = NULL;
        nodes_generated++;
        n2->parent = parent;
        n2->g = (parent->g)+1;

        for(i=0;i<4;i++){
            for(j=0;j<4;j++){
                n2->puzzle[i][j] = parent->puzzle[i][j];
            }
        }
        swapNumbers(&(n2->puzzle[parent->x][parent->y]), &(n2->puzzle[parent->x][(parent->y) - 1]));

        n2->x = (parent->x);
        n2->y = (parent->y) - 1;

        if(o == 1)
            n2->h = calculateh1(n2->puzzle);
        else if(o == 2)
            n2->h = calculateh2(n2->puzzle);
    }
    //printNodes(n2);
    //printf("child 2 above\n");

    if(parent->y <= 2){              //  EMPTY SQUARE MOVES 1 COLUMN RIGHT
        n3 = createNewNode();
        n3->next = NULL;
        nodes_generated++;
        n3->parent = parent;
        n3->g = (parent->g)+1;

        for(i=0;i<4;i++){
            for(j=0;j<4;j++){
                n3->puzzle[i][j] = parent->puzzle[i][j];
            }
        }
        swapNumbers(&(n3->puzzle[parent->x][parent->y]), &(n3->puzzle[parent->x][(parent->y) + 1]));

        n3->x = (parent->x);
        n3->y = (parent->y) + 1;

        if(o == 1)
            n3->h = calculateh1(n3->puzzle);
        else if(o == 2)
            n3->h = calculateh2(n3->puzzle);
    }
    //printNodes(n3);
    //printf("child 3 above\n");


    if(n0 != NULL)
        operateOn(n0);
        //printf("\ncompleted 0 operateoOn........................\n");
    if(n1 != NULL)
        operateOn(n1);
        //printf("\ncompleted 1 operateoOn........................\n");
    if(n2 != NULL)
        operateOn(n2);
        //printf("\ncompleted 2 operateoOn........................\n");
    if(n3 != NULL)
        operateOn(n3);
    //printf("\ncompleted expandParent........................\n");
}

node *solve(node *node_start, int o){
    openList = node_start;
    int i,c,j;
    while(openList != NULL){
        node *node_current = deleteFirstNode(&openList);

        if(compareMatrix(node_current->puzzle, goal) == 1){
            printf("REACHED GOAL STAGE!\n");
            printf("-------------------------------------------------------\n");
            printf("\nNo. of Nodes Generated:  %d\n",nodes_generated);               // GLOBAL VARIABLES
            nodes_in_memory = countNodes(closedList) + countNodes(openList);
            printf("\nNo. of Nodes present in Open List = %d\n", countNodes(openList));
            printf("\nNo. of Nodes present in Closed List = %d\n", countNodes(closedList));
            printf("\nTotal No. of Nodes present in the Memory = %d\n", nodes_in_memory);
            return node_current;
        }
        //printf("\nreached expandParent........................\n");
        expandParent(node_current, o);
        insertIntoList(&closedList, node_current);
        printf("P R O C E S S I N G . . . %d\n\n",global++);
    }
}

void printPath(node *final_node){
    printf("\n-------------------------------------------------------\nREVERSE PATH FROM REACHED GOAL TO ORIGINAL PROBLEM\n\n-------------------------------------------------------\n");
    while(final_node != NULL){
        printMatrix(final_node->puzzle);
        final_node = final_node->parent;
        printf("\n---------------------------------\n");
    }
}

int calculateh1(int a[4][4]){
    int i,j,c=0;
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            if(a[i][j] != 0){
                if(a[i][j] != goal[i][j])
                    c++;
            }
        }
    }
    return c;
}

int calculateh2(int a[4][4]){
    int i,j,s=0,d=0;
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            if(a[i][j] != 0){
                d=abs(i-((a[i][j]-1)/4))+abs(j-((a[i][j]-1)%4));
                s+=d;
            }
        }
    }
    return s;
}

int getInvCount(int arr[])
{
    int inv_count = 0,i,j;
    for(i=0;i<15;i++){
        for(j=i+1;j<16;j++){
            if (arr[j] && arr[i] && arr[i] > arr[j])
                inv_count++;
        }
    }
    return inv_count;
}

// This function returns true if given
// instance of 15 puzzle is solvable
int isSolvable(node *a){
    // Count inversions in given puzzle
    int invCount = getInvCount((int *)a->puzzle);

    int pos = 4 - (a->x);
    if (pos & 1)
        return !(invCount & 1);
    else
        return invCount & 1;
}

/****************************************************************************************/

int main(){
    node *initial = (node *)malloc(sizeof(node));
    int i,j,choice;
    printf("SOLVING 15-PUZZLE PROBLEM USING A* ALGORITHM |\n");
    printf("---------------------------------------------+\n\n");
    printf("Enter the Initial State of the Puzzle (as a 4x4 matrix):\n");
    for(i=0;i<4;i++){
        for(j=0;j<4;j++)
            scanf("%d",&(initial->puzzle[i][j]));
    }

    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            if(initial->puzzle[i][j] == 0){
                initial->x = i;
                initial->y = j;
            }
        }
    }

    initial->g = 0;
    initial->parent = NULL;
    initial->next = NULL;

    printf("Choose 1 for Heuristic 1: HAMMING DISTANCE/NO. OF MISPLACED TILES\nChoose 2 for Heuristic 2: MANHATTAN DISTANCE/NO. OF MOVES EACH TILE IS AWAY FROM GOAL POSITION\n");

    while(1){
        scanf("%d", &choice);
        if(choice == 1 || choice == 2)
            break;
        else
            printf("\nWRONG CHOICE! PLEASE TRY AGAIN.\n");
    }

    openList = NULL;
    closedList = NULL;
    nodes_generated = 0;
    nodes_in_memory = 0;

    if(choice == 1){
        initial->h=calculateh1(initial->puzzle);
        printf("\nSOLVING WITH HAMMING DISTANCE/NO. OF MISPLACED TILES...\n");
    }
    else{
        initial->h=calculateh2(initial->puzzle);
        printf("\nSOLVING WITH MANHATTAN DISTANCE/NO. OF MOVES EACH TILE IS AWAY FROM GOAL POSITION...\n");
    }

    if(isSolvable(initial) == 0){
        printf("\nTHE PUZZLE IS UNSOLVABLE IN THE GIVEN CONFIGURATION.\n");
        return 0;
    }

    node *final_node = solve(initial, choice);
    printf("\nDepth of the Solution (No. of moves): %d\n",final_node->g);
    printPath(final_node);

    return 0;
}
