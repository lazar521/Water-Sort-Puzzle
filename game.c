#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

#define True 1
#define False 0
#define tubeSize 4

//structovi za stack


//structovi za drvo
typedef struct treeNode {
	struct treeNode* down;
	struct treeNode* right;
	struct treeNode* up;
	char** matrix;
	char depth;
	char tube_A;
	char tube_B;
	char winState;
}treeNode;

typedef struct tree {
	treeNode* root;
}tree;


//structovi za red
typedef struct queueNode {
	struct queueNode* next;
	treeNode* treeNodePointer;
}queueNode;

typedef struct queue {
	queueNode* rear;
	queueNode* front;
}queue;


typedef struct stack {
	queueNode* top;
}stack;

//funkcije za stack
stack* createStack() {
	stack* tempStack = malloc(sizeof(stack));
	if (tempStack != NULL) {
		tempStack->top = NULL;
		return tempStack;
	}
	else {
		printf("\nAlokacija nije uspela");
		return NULL;
	}
}



treeNode* removeStack(stack* tempStack) {
	treeNode* tempTreeNode = tempStack->top->treeNodePointer;
	tempStack->top = tempStack->top->next;
	return tempTreeNode;
}

char stackEmpty(stack* tempStack) {
	if (tempStack->top == NULL) {
		return 1;
	}
	else {
		return 0;
	}
}

//funkcije za drvo
treeNode* createTreeNode() {
	treeNode* temp = (treeNode*)malloc(sizeof(treeNode));
	if (temp != NULL) {
		temp->down = NULL;
		temp->right = NULL;
		temp->up = NULL;
		temp->matrix = NULL;
		temp->depth = -1;
		temp->tube_A = -1;
		temp->tube_B = -1;
		temp->winState = 0;
		return temp;
	}
	else {
		printf("\nNema dovoljno memorije");
		return NULL;
	}
}

tree* createTree() {
	tree* tempTree = malloc(sizeof(tree));
	if (tempTree != NULL) {
		tempTree->root = createTreeNode();
		tempTree->root->depth = 0;
		return tempTree;
	}
	else {
		printf("\nNema dovoljno memorije");
		return NULL;
	}
}


//funckije za red
queueNode* createQueueNode() {
	queueNode* tempQueueNode = malloc(sizeof(queueNode));
	if (tempQueueNode != NULL) {
		tempQueueNode->treeNodePointer = NULL;
		tempQueueNode->next = NULL;
		return tempQueueNode;
	}
	else {
		printf("\nAlokacija nije uspela");
		return NULL;
	}
}

queue* createQueue() {
	queue* tempQueue = malloc(sizeof(queue));
	if (tempQueue != NULL) {
		tempQueue->front = NULL;
		tempQueue->rear = NULL;
		return tempQueue;
	}
	else {
		printf("\nAlokacija nije uspela");
		return NULL;
	}
}

void insertQueue(treeNode* tempTreeNode, queue* tempQueue) {
	queueNode* tempQueueNode = malloc(sizeof(queueNode));
	if (tempQueueNode != NULL) {
		tempQueueNode->treeNodePointer = tempTreeNode;

		if (tempQueue->rear == NULL) {
			tempQueue->front = tempQueueNode;
			tempQueue->rear = tempQueueNode;
		}
		else {
			tempQueue->rear->next = tempQueueNode;
			tempQueue->rear = tempQueueNode;
		}
	}
	else {
		printf("\nAlokacija nije uspela");
		return;
	}
}

treeNode* removeQueue(queue* tempQueue) {
	if (tempQueue->front != NULL) {

		queueNode* tempQueueNode = tempQueue->front;
		treeNode* tempTreeNode = tempQueueNode->treeNodePointer;




		if (tempQueue->front == tempQueue->rear) {
			tempQueue->rear = NULL;
			tempQueue->front = NULL;
		}
		else {
			tempQueue->front = tempQueue->front->next;
		}

		free(tempQueueNode);
		return tempTreeNode;
	}
	else {
		printf("\nAlokacija nije uspela");
		return NULL;
	}
}

char queueEmpty(queue* tempQueue) {
	if (tempQueue->front == NULL) {
		return 1;
	}
	else {
		return 0;
	}
}

void printQueue(queue* tempQueue) {
	if (tempQueue->front == NULL) {
		printf("red je prazan");
		return;
	}
	else {
		queueNode* current = tempQueue->front;
		while (current != NULL) {
			printf("%d ", current->treeNodePointer->depth);
			current = current->next;
		}
	}
}

void insertStack(treeNode* tempTreeNode, stack* tempStack) {
	queueNode* node = createQueueNode();
	node->treeNodePointer = tempTreeNode;

	if (tempStack->top == NULL) {
		tempStack->top = node;
	}
	else {
		node->next = tempStack->top;
		tempStack->top = node;
	}
	return;
}


//funkcije glavnog programa
void LCG(char** tubes, int n) {
	long long int a, c, m, index;
	a = 16625;
	c = 101223;
	m = 429496;

	srand(time(NULL));
	long long int x = rand();
	char* colorCount = calloc(n, sizeof(char));
	if (colorCount == NULL) {
		printf("\nAlokacija nije uspela");
		return;
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < tubeSize; j++) {
			int valueChecker = 0;
			x = (x * 4213 + 482) % 425;
			while (1) {
				x = ((x * a + c) % m);
				index = x % n;

				//provera da se petlja ne vrti u krug sa konstantno istim rezultatom za X
				if (valueChecker == 4) {
					valueChecker = 0;

					continue;
				}
				//provera da li je rezultat generisanja validan
				if (colorCount[index] == 4) {
					valueChecker++;
					continue;
				}
				//provera da li je cela tuba popunjena istom bojom
				if (j == tubeSize - 1) {
					char again = True;
					for (int b = 0; b < tubeSize - 1; b++) {
						if (tubes[i][b] != index + 1) {
							valueChecker++;
							again = False;
							break;
						}
					}
					if (again) continue;
				}

				colorCount[index]++;
				tubes[i][j] = index + 1;
				break;
			}
		}
	}
	free(colorCount);
	return;
}

void printTubes(char** tubes, char n) {
	printf("\n");
	for (int i = tubeSize - 1; i >= 0; i--) {
		for (int j = 0; j < n; j++) {
			printf(" |%2d| ", tubes[j][i]);
		}
		printf("\n");
	}
	for (int i = 0; i < n; i++) printf(" \\__/ ");
}

int checkMove(char** tubes, int tube_A, int tube_B) {
	//prebacuje se iz boce A u bocu b potrebno je proveriti validnost poteza

	char topA = 0;
	char topB = 3;

	if (tubes[tube_B][3] != 0) {

		return 0;
	}
	if (tubes[tube_A][0] == 0) {

		return 0;
	}

	while (tubes[tube_A][topA] != 0 && topA < 4) {
		topA++;
	}
	while (tubes[tube_B][topB] == 0 && topB > 0) {
		topB--;
	}
	topA--;

	if ((tubes[tube_A][topA] != tubes[tube_B][topB]) && tubes[tube_B][topB] != 0) {

		return 0;
	}

	return 1;
}

void makeMove(char** tubes, int tube_A, int tube_B) {
	char topA = 0;
	char topB = 3;

	while (tubes[tube_A][topA] != 0 && topA < 4) {
		topA++;
	}
	while (tubes[tube_B][topB] == 0 && topB > -1) {
		topB--;
	}
	topB++;
	topA--;

	char current_color = tubes[tube_A][topA];
	while ((tubes[tube_A][topA] == current_color) && (topA > -1) && (topB < tubeSize)) {
		tubes[tube_B][topB] = tubes[tube_A][topA];
		tubes[tube_A][topA] = 0;
		topB++;
		topA--;
	}
	return;
}

char** newTubes(char** tubes, int n, int tube_A, int tube_B) {
	char** newMatrix = malloc(n * sizeof(char*));
	if (newMatrix == NULL) {
		printf("\nAlociranje nije uspelo");
		return NULL;
	}
	for (int i = 0; i < n; i++) {
		newMatrix[i] = malloc(tubeSize * sizeof(char));
		if (newMatrix[i] == NULL) {
			printf("\nAlociranje nije uspelo");
			return NULL;
		}
		memcpy(newMatrix[i], tubes[i], tubeSize);
	}
	makeMove(newMatrix, tube_A, tube_B);
	return newMatrix;
}

void winCheck(treeNode* tempTreeNode, int n) {
	char** tubes = tempTreeNode->matrix;
	for (int i = 0; i < n; i++) {
		char win = 1;
		char temp = tubes[i][0];
		for (int j = 1; j < tubeSize; j++) {
			if (temp != tubes[i][j]) {
				return;
			}
		}
	}
	tempTreeNode->winState = 2;
}

char retardedMove(char** tubes, int n, int tube_A, int tube_B) {
	char doneTube = True;
	char tempColor = tubes[tube_A][0];
	for (int i = 1; i < tubeSize; i++) {
		if (tempColor != tubes[tube_A][i]) {
			doneTube = False;
			break;
		}
	}
	if (doneTube && (tubes[tube_B][0] == 0)) {
		return 1;
	}
	else {
		return 0;
	}
}

treeNode* giveHint(treeNode* current) {
	current = current->down;
	while (current->winState != 1 && current->winState != 2) {
		current = current->right;
	}

	return current;
}

void solveIt(treeNode* current) {
	while (1) {
		current = current->down;
		while (current->winState != 1 && current->winState != 2) {
			current = current->right;
		}
		if (current->winState == 1) {
			printf("\nOdigraj potez %d u %d", current->tube_A + 1, current->tube_B + 1);
		}
		if (current->winState == 2) {
			printf("\nOdigraj potez %d u %d", current->tube_A + 1, current->tube_B + 1);
			return;
		}
	}
}

void printTreeLevelOrder(treeNode* root, int p) {
	printf("\n\n");

	treeNode* temp;
	treeNode* current;
	queue* treeNodeQueue = createQueue();
	insertQueue(root, treeNodeQueue);

	int level = -1;
	while (1) {
		if (queueEmpty(treeNodeQueue)) break;
		current = removeQueue(treeNodeQueue);

		if (current->depth > level) {
			printf("\n");
			level = current->depth;
		}
		if (level > p) break;

		while (current != NULL) {
			if (current->down != NULL) {
				insertQueue(current->down, treeNodeQueue);
			}
			else {
				temp = createTreeNode();
				temp->depth = level + 1;
				temp->winState = 9;
				insertQueue(temp, treeNodeQueue);
			}
			if (current->winState != 9) {
				printf("_%d,%d_", current->tube_A + 1, current->tube_B + 1);
				current = current->right;
			}
			else {
				printf("---");
				free(current);
				break;
			}
		}
		printf("  ||  ");
	}


}

void printTreePreOrder(treeNode* root, int p) {
	queue** queueVector = malloc((p + 1) * sizeof(queue*));
	if (queueVector == NULL) {
		printf("\nAlokacija nije uspela");
		return;
	}
	for (int i = 0; i < p + 1; i++) {
		queueVector[i] = malloc(sizeof(queue));
		if (queueVector[i] == NULL) {
			printf("\nAlokacija nije uspela");
			return;
		}
		queueVector[i] = createQueue();
	}
	stack* tempStack = createStack();
	insertStack(root, tempStack);
	treeNode* current;

	while (!stackEmpty(tempStack)) {
		current = removeStack(tempStack);



		while (current != NULL) {
			insertQueue(current, queueVector[current->depth]);
			if (current->right != NULL) {
				insertStack(current->right, tempStack);
			}

			current = current->down;

		}

	}
	for (int i = 0; i < (p + 1); i++) {
		while (!queueEmpty(queueVector[i])) {
			current = removeQueue(queueVector[i]);


			printf("(%d,%d)  ", current->tube_A + 1, current->tube_B + 1);
		}
		free(queueVector[i]);
		printf("\n");
	}
	free(queueVector);

}

int main() {

	int n, p, k;
	printf("unesite ukupan broj bocica, broj praznih bocica i broj koraka: ");
	scanf("%d %d %d", &n, &k, &p);

	//pravljenje matrice
	char** tubes = (char**)malloc(n * sizeof(char*));
	if (tubes == NULL) {
		printf("\nAlokacija nije uspela");
		return 0;
	}
	for (int i = 0; i < n; i++) {
		tubes[i] = (char*)calloc(tubeSize, sizeof(char));
		if (tubes[i] == NULL) {
			printf("\nAlokacija nije uspela");
			return 0;
		}
		if (tubes[i] == NULL) {
			printf("\nAlokacija nije uspela");
			return 0;
		}

	}

	//deklarisanje pomocne matrice koja ce sluziit za proveru poteza

	char** checkerMatrix = malloc(n * sizeof(char*));
	if (checkerMatrix == NULL) {
		printf("\nAlokacija nije uspela");
		return 0;
	}
	for (int i = 0; i < n; i++) {
		checkerMatrix[i] = malloc(tubeSize * sizeof(char));
		if (checkerMatrix[i] == NULL) {
			printf("\nAlokacija nije uspela");
			return 0;
		}
	}

	tree* workTree = createTree();
	queue* workQueue = createQueue();

	treeNode* son;
	treeNode* temp;
	char  level;
	char firstSon;





	workTree->root->matrix = tubes;
	treeNode* father = workTree->root;
	father->depth = 0;




	LCG(tubes, n - k);

	while (1) {


		firstSon = True;
		level = father->depth + 1;

		if (level > p)break;

		for (int i = 0; i < n; i++) {

			if (father->winState == 2)break;

			for (int j = 0; j < n; j++) {

				if (j == i) continue;


				if (checkMove(father->matrix, i, j)) {

					if (retardedMove(father->matrix, n, i, j)) continue;

					temp = createTreeNode();
					temp->matrix = newTubes(father->matrix, n, i, j);
					temp->depth = level;
					temp->tube_A = i;
					temp->tube_B = j;
					temp->up = father;


					if (firstSon == True) {
						son = temp;
						father->down = son;

						firstSon = False;
					}
					else {
						son->right = temp;
						son = temp;

					}
					winCheck(son, n);
					if (son->winState == 2) {
						treeNode* winNode = son;
						winNode = winNode->up;

						while (winNode != NULL) {
							winNode->winState = 1;
							winNode = winNode->up;
						}
					}
					insertQueue(son, workQueue);

				}
			}
		}

		if (queueEmpty(workQueue)) break;
		father = removeQueue(workQueue);
	}

	if (workTree->root->winState != 1) {
		printf("\nGenerisano stablo nije resivo");
		return 0;
	}


	printTubes(tubes, n);
	printf("\nIgra moze da pocne:");



	//graficki interface


	treeNode* current = workTree->root;
	level = 0;
	int tubeA, tubeB;

	while (1) {
		//ispis matrice


		printf("\n1.Odigrajte potez?\n2.Pomoc?\n3.Resi celu igricu :(\n4.Nacrtaj mi drvo :)\n5.Izadji\n");
		char c;
		while ((c = getchar()) != '\n');
		c = getchar();
		switch (c) {
		case '1':
			scanf("%d %d", &tubeA, &tubeB);
			tubeA--;
			tubeB--;

			if ((tubeA == tubeB) || tubeA<0 || tubeA>n - 1 || tubeB<0 || tubeB>n - 1) {
				printf("\nNe valja potez");
				continue;
			}

			if (!checkMove(current->matrix, tubeA, tubeB)) {
				printf("\nNe valja potez");
				continue;
			}
			if (retardedMove(current->matrix, n, tubeA, tubeB)) {
				printf("\nNema presipanja iz  supljeg u prazno !");
				continue;
			}

			current = current->down;

			while ((current->tube_A != tubeA) || (current->tube_B != tubeB)) {
				current = current->right;
			}
			printTubes(current->matrix, n);
			level++;
			if (current->winState == 0) {
				printf("\n\nOdavde nema pobede\nIgra je gotova\n");
				return 0;
			}
			if (level > p) {
				printf("\n\nDostigli ste maksimal broj poteza\nIgra je gotova\n");
				return 0;
			}
			if (current->winState == 2) {
				printf("\nCestitamo genije si\n");
				printTubes(current->matrix, n);
				return 0;
			}
			printf("\nBroj poteza: %d\n", current->depth);
			break;
		case '2':
			current = giveHint(current);
			if (level > p) {
				printf("\n\nDostigli ste maksimal broj poteza\nIgra je gotova\n");
				return 0;
			}
			if (current->winState == 2) {
				printf("\nCestitamo genije si\n");
				printTubes(current->matrix, n);
				return 0;
			}
			else {
				printTubes(current->matrix, n);
			}
			printf("\nBroj poteza: %d\n", current->depth);
			break;

		case '3':
			solveIt(current);
			break;
		case '4':
			//printTreeLevelOrder(workTree->root,p);
			printTreePreOrder(workTree->root, p);
			break;

		case '5':
			return 0;
			break;
		}
	}


}
