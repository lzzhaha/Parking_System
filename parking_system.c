/*A simple parking system implementation using consumer and producer model.
  car_picker = consumer, pick car only when num_car > 0
  car_parker = producer, park car only when num_space > 0
*/


#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define ONE_SECOND 1000000
#define RANGE 10
#define PERIOD 2
#define NUM_THREADS 6

// structure of parking_lot
typedef struct {
    int *spaces; //parking spaces
    int capacity; // maximum number of cars to park
    int occupied; // number of spaces occupied
    int nextin;  // next position for a car to park
    int nextout; // next position for a car to pick
    int cars_in; // number of cars ever parked 
    int cars_out;  //number of cars ever picked
    pthread_mutex_t lock; //the mutex for controlling the access to spaces
    pthread_cond_t num_space; //the conditional variable on number of available sapces
    pthread_cond_t num_car;   //the conditional vavriable on number of parked cars
    pthread_barrier_t bar; //barrier of thread
} parking_lot_t;

static void * parking_handler(parking_lot_t *parking_lot);//start routine of producer thread

static void * picking_handler(parking_lot_t *parking_lot);//start routine of consumer thread

static void * monitor(parking_lot_t* parking_lot);//start routine of monitor

static void initialize(parking_lot *cp, int size);//initialize parking_lot


int main(int argc, char *argv[]) {

	if (argc != 2) {
		printf("Usage: %s capacity\n", argv[0]);
		exit(1);
	}
	
	parking_lot_t parking_lot;

	initialise(&parking_lot, atoi(argv[1])); 

	pthread_t car_parker1, car_parker2, car_parker3;
	pthread_t car_picker1, car_picker2, car_picker3;
	pthread_t parking_monitor;



	pthread_create(&car_parker1, NULL, parking_handler,&parking_lot);
	pthread_create(&car_picker1, NULL, picking_handler, &parking_lot); 
	pthread_create(&car_parker2, NULL, parking_handler, &parking_lot); 
	pthread_create(&car_picker2, NULL, picking_handler, &parking_lot); 
	pthread_create(&car_parker3, NULL, parking_handler, &parking_lot); 
	pthread_create(&car_picker3, NULL, picking_handler, &parking_lot); 

	pthread_create(&m, NULL, monitor,&parking_lot);  

	//terminate the threads sequentially
	pthread_join(car_parker1, NULL);
	pthread_join(car_picker1, NULL);
	pthread_join(car_parker2, NULL);
	pthread_join(car_picker2, NULL);
	pthread_join(car_parker3, NULL);
	pthread_join(car_picker3, NULL);
	pthread_join(m, NULL);

	exit(0);
}


static void initialize(parking_lot_t *parking_lot, int size){

	parking_lot->capacity = size;
	parking_lot->occupied = parking_lot->nextin = parking_lot->nextout = 0;
	parking_lot->cars_in = parking_lot->cars_out = 0;

	parking_lot->spaces = (int*)malloc(size * sizeof(*(parking_lot->spaces)));

	//initialize thread barrier so that it would wait for 
	//NUM_THREADS threads to synchronize
	pthread_barrier_init(&&parking_lot->barrier, NULL, NUM_THREADS);

	if(parking_lot->spaces == NULL){
		printf("No enough memory\n");
		exit(1);
	}
		
	srand((unsigned int)(getpid());

	pthread_mutex_init(&(parking_lot->lock, NULL);

	pthread_cond_init(&(parking_lot->num_space, NULL);

	pthread_cond_init(&(parking_lot->num_car, NULL);


}


static void* parking_handler(parking_lot_t* parking_lot){
	
	unsigned int seed;

	pthread_barrier_wait(&(parking_lot->barrier));

	//simulate the random arrival of cars
	while(1){
	
		//cause the current thread to sleep for a random amount of time
		usleep(rand_r(&seed) & ONE_SECOND);
		pthread_mutex_lock(&(parking_lot->lock));

		//busy waiting for parking spaces
		while(parking_lot->occupied == parking_lot->capacity){
			//waiting on conditional variable space
			//keep releasing locks
			pthread_cond_wait(&(parking_lot->num_space), &(parking_lot->lock);
		}

		//park a  car(represented as a random number)
		parking_lot->spaces[nextin] = rand_r(&seed) % RANGE;

		parking_lot->occupied++;
		parking_lot->nextin++;
		parking_lot->nextin%= parking_lot->capacity;

		parking_lot->cars_in++;
		
		//signal the conditional variable to wake up 
		//waiting consumer
		pthread_cond_signal(&(parking_lot->num_car));

	}
	return ((void*) NULL);
}
