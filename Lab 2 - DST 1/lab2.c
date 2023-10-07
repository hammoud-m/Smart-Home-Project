#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


//Struct declaration
struct DLinkedList {
	double sensorData;
	struct DLinkedList *prev;
	struct DLinkedList *next;
};

//Declarations
void insertFirst (struct DLinkedList **first, struct DLinkedList *el);
int isMember (struct DLinkedList **first, struct DLinkedList *el);
void printList(struct DLinkedList *first);
void ourRemove(struct DLinkedList **first, struct DLinkedList *el);
struct DLinkedList *readSensor();
void printListReverse(struct DLinkedList *first);
void sort(struct DLinkedList *first);
void freeAll(struct DLinkedList *first);


/*Authors Joakim Svensson & Mohammad Hammoud
  Date finished 25/11/2020
*/

//main creates
int main(int argc, char const *argv[])
{
	struct DLinkedList *first;
	struct DLinkedList *theFirst = readSensor();
	first = theFirst;
	for (int i = 0; i < 10; i++)
	{
		struct DLinkedList *new = readSensor();
		insertFirst(&first,new);
	}
	
	//printListReverse(first);
	printList(first);
	freeAll(first);
	//printList(first);
    return 0;
}

//function inserts el at the beginning of the list and updates first.
void insertFirst (struct DLinkedList **first, struct DLinkedList *el) {
	if (first == NULL || el == NULL)
	{
		printf("Error in InsertFirst");
		return;
	}
	struct DLinkedList *temp;
	temp = *first; //creates temp to change prev for the old first 
	temp->prev = el; //sets prev for temp
	*first = temp; //assigns temp to first
	el->next = temp; //sets el's next to the old first reference
	*first = el; //changes so *first points to el instead.
}

//function returns 0 if el is a member if the list and 1 otherwise
int isMember (struct DLinkedList **first, struct DLinkedList *el) { 
	if (first == NULL|| el == NULL)
	{
		printf("Error in isMember");
		return 0;
	}
	struct DLinkedList *temp;
	temp = *first;
	while (temp != NULL) {
		if (temp == el) { 
			return 1;// if it finds the data in the list then return 1
		}
		temp = temp->next;
	}
	return 0;
}

void printList(struct DLinkedList *first) { //function prints the list from the beginning till the end
	if (first == NULL)
	{
		printf("Error in printList");
		return;
	}
	struct DLinkedList *temp;
	temp = first; // makes temp pointer point to first
	while (temp != NULL) {
		printf("%f\n",temp->sensorData);//prints the sensorData
		temp = temp->next;//goes to next
	}
}


void freeAll(struct DLinkedList *first) { //frees the list from memory
	if (first == NULL)
	{
		printf("Error in printList");
		return;
	}
	//printf("tries");
	struct DLinkedList *temp = first;//temp pointer to first
	while (temp != NULL) {
		if (temp->prev != NULL)//while the previous is not null
		{
			//printf("freeing prev: %f \n",temp->prev->sensorData);
			free(temp->prev);//free the previous
		}
		temp = temp->next;//go to next
	}
	free(temp->next);//free the next one ( as there is one )
	free(temp);//free current
}

void printListReverse(struct DLinkedList *first) { //function prints the list from the end to the start
	if (first == NULL)
	{
		printf("Error in printListReverse");
		return;
	}
	struct DLinkedList *temp;
	temp = first;
	while (temp->next != NULL) {
		temp = temp->next;//goes to the end
	}
	while (temp != NULL) {
		printf("%f\n",temp->sensorData);//prints from the end
		temp = temp->prev;
	}
}

// this function removes el from the list (if it is a member) without deleting it from memory
void ourRemove(struct DLinkedList **first, struct DLinkedList *el) { 
	if (first == NULL|| el == NULL)
	{
		printf("Error in ourRemove");
		return;
	}
	struct DLinkedList *temp;
	temp = *first;
	if (temp == el) { //if first is the to be removed element remove set first to next one
		*first = temp->next;
	}
	while (temp != NULL) {// if it finds the to be removed element, it cuts it out of the linkedlist (skips it)
		if (temp == el) {
			temp = temp->prev;
			temp->next = temp->next->next; // sets next to skip removed
			temp->next->prev = temp; // sets prev
		}
		temp = temp->next;
	}
}

static unsigned int randSeed = 100;//creates to be combined int together with time for random value each new value

//creates DLinkedList with random number between 0 and 1
struct DLinkedList *readSensor() {
	struct DLinkedList *returnedSensor;
	returnedSensor = (struct DLinkedList *)malloc(sizeof(struct DLinkedList));

	srand((randSeed++)+time(NULL)); //creates seed using randSeed and 
	double sensorNumber = (rand()/(double)RAND_MAX);//
	int randTemp = sensorNumber*100000000;
	randTemp = randTemp%100000;//Shaves off numbers so it becomes random with less time needing to have passed
	sensorNumber = (double)randTemp/(double)100000;

	returnedSensor->sensorData = sensorNumber;
	returnedSensor->next = NULL;
	returnedSensor->prev = NULL;
	//printf("%f sensorGenerated \n",sensorNumber);
	return returnedSensor;
}

//sorts in order small to big
void sort(struct DLinkedList *first) {
	if (first == NULL)
	{
		printf("Error in sort");
		return;
	}
	struct DLinkedList *temp;
	temp = first;
	double tempValue;
	while (temp->next != NULL) {//checks if next is null otherwise it cannot be compared
		if(temp->sensorData > temp->next->sensorData) { //if current is larger it sets it to the next and next to current 
			tempValue = temp->next->sensorData;  //saves next in temp variable
			temp->next->sensorData = temp->sensorData; //pushes large to next
			temp->sensorData = tempValue; //makes current into temp var
		}
		temp = temp->next; //to go check next
	}
	temp = temp->next; // since last one is checked but never "saved"
	first = temp;
}