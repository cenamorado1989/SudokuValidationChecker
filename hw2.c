#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define N 11 

/*
Christian Enamorado
2/17/2019
operating systems
homework assignment two

create threads for sudoku validation checker
*/

typedef struct{
int row;
int col;
int id;
} parameters;


void *column_worker(void *param);
void *row_worker(void *param);
void *grid_worker(void *param); 	

int *solution;
int valid[N] = {0};

 int main(int argc, char *argv[])
{
	
	FILE *fp = fopen(argv[1], "r"); 				// open file with read permission using first argument and file pointer
	if(fp == NULL) {
		printf("Can't be opened\n"); 				// error checking if file does not exist or is null
		exit(EXIT_FAILURE); 
	}
	solution = (int*)calloc(9*9, sizeof(int));  			// allocating and initializing array to zero

	int i = 0;
	int y = 0;
	fscanf(fp, "%d", &i);        					// scan file int by int and store inside i
	int x = 0;

	pthread_t threads[N];
	pthread_attr_t attr;      
	pthread_attr_init(&attr);

	while(!feof(fp)){						// loop that runs until file reaches the end
	
	*(solution+x) = i;						// used pointer and pointer arithmetic to assign each int to array
	printf("%d ",*(solution + x));	
	fscanf(fp, "%d", &i);
	x++; 								// x is to increment the array
	}
	printf("\n");	

	x = 0;
	int counter = 0;
	parameters *par = (parameters*)malloc(sizeof(parameters));	// parameter object
	par->row = 0;
	par->col = 0;
	par->id = 0;
	int idT = 0;							// thread ID
	y = 0;	
	while( x <= 6){

		while( y <= 6){

			if( x == 0 && y == 0 ) {
				par->row = x;
				par->col = y;
				pthread_create(&threads[10],&attr,column_worker,par);	// while x = o and y = 0 spawn column and row worker
				pthread_create(&threads[9],&attr,row_worker,par);
			}
		par->row = x;						//assign values row and columns to structure
		par->col = y;
		par->id = idT;
		printf("Thread input: (%d,%d,%d)\n",par->row, par->col,par->id); //print thread input 
		pthread_create(&threads[counter],&attr,grid_worker,par);
		usleep(1000);
		idT++;
		counter++;				

		y+= 3;
		}
		y = 0;

	x+=3;
	}
	x = 0;

	while(x < 11){
		pthread_join(threads[x],NULL);		// after spawning join all threads
		x++;
	}

	x = 0;
	while(x < 11){
		printf("%d ", valid[x]);
		if(valid[x] == 0){
			printf("\nThis is not a valid solution!");		// check final validity array
			return EXIT_SUCCESS;
		}
	x++;
	}
	printf("\nThis is a valid solution!");	
	return 0;
}


void *column_worker(void *param){ // these work
	
	parameters *params = (parameters*)param;
	int row = params->row;		// parameter point creation
	int col = params->col;
	int test[9] = {0};	 	//test array

	while(col < 9){
		while(row < 9){
			if(test[*(solution + (9 * row) + col) - 1] == 1){
				printf("Column worker failed\n");
				pthread_exit(NULL);
			} else {
				test[*(solution + (9 * row) + col) - 1] = 1;	
			}		
		row++;
		}
		row = 0;

	int x = 0;
	while( x < 9){
	test[x] = 0;
	x++;
	}

	col++;
	}
	valid[10] = 1;	
	printf("Column worker succeeded!\n");	

	pthread_exit(0);
}

void *row_worker(void *param){// these work

	parameters *params = (parameters*)param;
	int row = params->row;				// create parameter pointer and cast parameter
	int col = params->col;
	int test[9] = {0};

	while(row < 9){
		while(col < 9){
			if(test[*(solution + (9 * row) + col) - 1] == 1){ // pointer arithmetic to access array and use value as index
				printf("Row worker failed\n");
				pthread_exit(NULL);	
			} else {
				test[*(solution + (9 * row) + col) - 1] = 1;
			}
		col++;
		}
		col = 0;	

	int x = 0;
	while( x < 9){			// reset test array after first iteration
	test[x] = 0;
	x++;
	}

	row++;
	}
	valid[9] = 1;
	printf("row worker succeeded!\n");
	
	pthread_exit(0);
}

void *grid_worker(void *param){
	
	parameters *params = (parameters*)param;  //cast pointer to parameter object pointer
	int row = params->row;
	int col = params->col;
	int id = params->id;
	int i = 0;
	int j = 0;

	int test[9] = {0};			// test array to check if value exists

	while(i < 3){
		while(j < 3){
			if(test[*(solution + (9 * row) + col) - 1] == 1){	//use pointer arithmetic to traverse array and use as index
				printf(" Grid worker failed\n");
				pthread_exit(NULL);
			} else {
				test[*(solution + (9 * row) + col) - 1] = 1;	//use pointer arithmetic to traverse array and use as index	
			}
		col++;
		j++;	
		}
		j = 0;
		col = params->col;
		printf("\n");
	row++;
	i++;
	}
	
	valid[id] = 1;// check as ok
	printf("Grid worker succeeded!\n");		
	
	pthread_exit(0);
}













