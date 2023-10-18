#include <stdlib.h>

#include "resourceManagement.h"

int const STRING_LENGTH = 31;

pqType createPQType( Department *d, double totalSpent );
double minDouble( double x, double y );
//NOTE TO SELF:  REMEMBER TO REMOVE ALL TEST CASES BEFORE TURN IN AND ALL ELSES IF ONLY USED FOR TESTIG PURPOSES 

/* printNames
 * input: none
 * output: none
 *
 * Prints names of the students who worked on this solution
 * REMINDER: You may only work with one other cs2124 student
 */
void printNames( )
{
    /* TODO : Fill in you and your partner's names (or N/A if you worked individually) */
    printf("\nThis solution was completed by:\n");
    printf("Qustandi Fashho\n");
    printf("N/A\n");
}

/* TODO:
 * ResourceManagement
 * input: char* fileNames[], double budget
 * output: none
 *
 * Simulates the algorithm from the pdf to determine what items are purchased
 * for the given budget and department item lists.
 * Print a summary of what each department received and did not receive.
 */
void ResourceManagement( char* fileNames[], int testDataSize, double budget ){

	double remainingBudget = budget; // will track how much budget will be remianing  to spend 


    /* Create a department for each file listed in fileNames */
	Department* departments[testDataSize];
	// malloc
	int i;
	for(i = 0; i < testDataSize; i++){ 
		departments[i] = (Department*)malloc(sizeof(Department));
		departments[i]->name = NULL;
		departments[i]->totalSpent = 0.0;
		departments[i]->itemsDesired = createQueue();
    	departments[i]->itemsReceived = createQueue();
    	departments[i]->itemsRemoved = createQueue();

		//printf("\nTest case 1");
		// READ FILE 
		FILE *file = fopen( fileNames[i], "r");
		if(file == NULL){
			printf("Cant open %s\t",  fileNames[i]);
			exit(-1);
		}
		else{
			//printf("\nOpening %s success\n ", fileNames[i]); // working all departments opened
		}
		//printf("\nTest 2\n"); // working 



		char arr[STRING_LENGTH]; // array of char named arr. reps max length of a line to read from file 
		// read department 
		if(fgets(arr, STRING_LENGTH, file) != NULL){ //reads line of text from the file into the arr array.
			// strcspn calculates the length of the number of characters before the 1st occurrence of character present in both the string.
			arr[strcspn(arr, "\n")] = '\0'; // removing newline characters from file
			departments[i] -> name = (char*)malloc(strlen(arr)+1); // mallocing the array arr. remember to free use valgrid to check 
			strcpy(departments[i]-> name, arr); //copies the department name from arr array
		}
		else{
			printf("Department missing\n"); // remove before turn in. 
			exit(-1); // exit only if department mssing
		}
		//printf("\nTest case 3\n"); // working 





		// read items needed for the departments and their prices 
		while (fgets(arr, STRING_LENGTH, file) != NULL) {
    		arr[strcspn(arr, "\n")] = '\0'; // Remove the newline character (same as above)
    		queueType item = (queueType)malloc(sizeof(Item)); // Allocate memory for the item
    		item->name = (char *)malloc(strlen(arr) + 1);
    		strcpy(item->name, arr); // Copy item name (SRC, DEST)

    		if (fgets(arr, STRING_LENGTH, file) != NULL) {
        		item->price = atof(arr); // Convert string to decimal number
				//printf( "\nWorks for:  %s\n", fileNames[i]); // it is workig for all of themn so idk why in the else it says physics and astronomy not 
    		} 
			else {
        		// Handle the case where the price is missing
        		printf( "Error: Missing price for an item in %s\n", fileNames[i]); // still showing missing price for physics and astronomy HOW TO FIX 
        		free(item->name); // Free memory allocated for the item name
        		free(item); // Free memory allocated for the item
        		continue; // Skip adding this item to the list
    }
    
    enqueue(departments[i]->itemsDesired, item); // Add item to the end of the queue
	//printf("\nTEST 5\n"); // works 
}
		
		
	fclose(file);
	}


    /* Simulate the algorithm for picking the items to purchase */
	PriorityQueue *pq =  createPQ();
	//printf("\nTEST 6\n"); // works 



	//Each department is given an initial priority of 0. This priority will represent the total amount
	//of money spent on that department so far. Each time an item is purchased you’ll update
	// that priority accordingly
	//int i = 0;
	for(i = 0; i < testDataSize; i++){ // this loop should cover all departments 
		pqType pt = createPQType(departments[i], 0.0); // create a priority queue of the departments  // This type represents the elements in the priority queue, and it includes a department pointer and a priority value. Tkes in current department pointer and priority value 
		insertPQ(pq, pt); // add department with its priority to the queue
		//printf("\nTEST 7\n"); // working
	}
	//printf("\nTEST 8\n");  // working

	// while the remaining money is not 0 and the queue is not empty 
	while(remainingBudget > 0 && !isEmptyPQ(pq)){
		pqType topDepartment = removePQ(pq); // we are removing the first department in from the queue, but then we are also saving it in topDepartment
		Department *currentDepartment = topDepartment.info; // extracts the Department pointer from topDepartment and assigns it to currentDepartment
		//printf("\nTEST 9\n"); // works 

		// while the price of the next item is wanted by the department is greater than what we have remaining in the budget, move the item to the item removed queue
		while(!isEmpty(currentDepartment->itemsDesired)){
			queueType nextItem = getNext(currentDepartment->itemsDesired);
			//printf("\nTEST 10\n"); // works 
			if(nextItem->price > remainingBudget){
				// move item to the itemreomved list of the department 
				enqueue(currentDepartment->itemsRemoved, nextItem);
				dequeue(currentDepartment->itemsDesired);
				//printf("\nTEST 11\n"); // works 
			}
			else{
				//else the next item, we will buy for the department 
				currentDepartment -> totalSpent += nextItem->price;
				remainingBudget -= nextItem->price;
				// update priority of the department:
				topDepartment.priority = currentDepartment->totalSpent;
				//add to their list of items recived queue using enqueue and dequeu
				enqueue(currentDepartment->itemsReceived, nextItem);
				dequeue(currentDepartment->itemsDesired);
				// department back to the queue 
				insertPQ(pq, topDepartment);
				//printf("\nTEST 12\n"); // works 
			}
		}

		if(isEmpty(currentDepartment->itemsDesired)){
			// if the departments dont need anything anymore and thus their itemsdesired queue is empty, give $1000 scholarship or whatver the remaining budget is if smaller:
			double scholarship = 1000.0;
			double scholarshipOrRemBudget;// = fmin(remainingBudget, scholarship); // whichever is smaller (used fmin rather than condinal)


			if(remainingBudget < scholarship){
				scholarshipOrRemBudget = remainingBudget;
				printf("\nRemaining Budget %lf\n", remainingBudget); // works 
			}
			else{
				scholarshipOrRemBudget = scholarship;
				//printf("\nScholarship %lf\n", scholarship); // works 1000
			}
			currentDepartment->totalSpent += scholarshipOrRemBudget; //////////// check these if they work with the conditionals 
			remainingBudget -= scholarshipOrRemBudget; ////// check this to see if it works with the condtionals 
			}
				
			}

	
       
	/* Print the information for each department (including which items were received and which were not) */ // 10/17
	//int i;
	printf("TESTING with budget = $%.2lf\n\n", budget);
	printf("ITEMS PURCHASED\n");
	printf("----------------------------\n");
	for(i = 0; i < testDataSize; i++){
		printf("Department of %s\n", departments[i]->name);
		printf("Total Spent = %.2lf\n", departments[i] -> totalSpent);
		printf("Percent of budget = %.2lf\n", (departments[i]-> totalSpent/budget)*100);
		printf("----------------------------\n");
		printf("ITEMS RECIEVED\n");
		// printing the items that were recieved 
		while(!isEmpty(departments[i]->itemsReceived)){
			queueType item = dequeue(departments[i]->itemsReceived);
            printf("%s - $%.2lf\n", item->name, item->price);
            free(item->name);
            free(item);
        
		}
		 printf("ITEMS NOT RECEIVED\n");
        // Print items not received
        while (!isEmpty(departments[i]->itemsRemoved)) {
            queueType item = dequeue(departments[i]->itemsRemoved);
            printf("%s - $%.2lf\n", item->name, item->price);
            free(item->name);
            free(item);
        }
		printf("\n");
		free(departments[i]->name);
        freeQueue(departments[i]->itemsDesired);
        freeQueue(departments[i]->itemsReceived);
        freeQueue(departments[i]->itemsRemoved);
        free(departments[i]);



		
	}
	freePQ(pq);
}



/* createPQType
 * input: Department *d, double totalSpent
 * output: pqType
 *
 * Creates and returns a pqType struct (i.e the type in the priority queue)
 * from the given department and totalSpent. 
 */
pqType createPQType( Department *d, double totalSpent ){
	pqType pt;
	pt.info = d;
	pt.priority = totalSpent;

	return pt;
}

/* minDouble
 * input: double x, double y 
 * output: the smaller of x and y
 */
double minDouble( double x, double y ){
	if( x<=y )
		return x;
	return y;
}