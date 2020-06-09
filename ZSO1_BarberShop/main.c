#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <string.h>

pthread_cond_t clientsAvailable = PTHREAD_COND_INITIALIZER;
pthread_cond_t barberReady = PTHREAD_COND_INITIALIZER;
pthread_cond_t barberCutting = PTHREAD_COND_INITIALIZER;
pthread_mutex_t accessChairs = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t accessBarber = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t gettingCut = PTHREAD_MUTEX_INITIALIZER;

int numberOfWaitingClients = 0;
int numberOfChairs ;
int totalNumberOfClients = 0;
int numberOfServedClients = 0;
int numberOfResignedClients = 0;
int currentlyServedClient = 0;
int cutting = 0;

typedef struct Pos_Struct
{
	int id;
	pthread_cond_t turn;
	struct Pos_Struct *next;
}Position;


void DoCut(Position *client)
{
	int i = rand()%4;
	pthread_mutex_lock(&gettingCut);
	cutting = 1;
	sleep(i);
	cutting = 0;
	pthread_cond_signal(&barberCutting);
// signals the end of the haircut
	pthread_mutex_unlock(&gettingCut);
}

void GetCut()
{
// waiting for haircut
	pthread_mutex_lock(&gettingCut);
	while (cutting)
	{
		pthread_cond_wait(&barberCutting, &gettingCut);
	}
	pthread_mutex_unlock(&gettingCut);
}

Position *barberQueue = NULL;
Position *resignedClients = NULL;


void PrintQueue(Position *start)
{
	Position *tmp = NULL;
	if (start == NULL) 
	{	
		return;
	}
	else 
	{
		tmp = start;
		while (tmp != NULL)
		{
			printf("%d ", tmp->id);
			tmp = tmp->next;
		}
	}
	printf("\n");
}


Position *AddNewClientToQueue(Position **start, int id)
{
	Position *new = malloc(sizeof(Position));
	new->id = id;
	new->next = NULL;
	pthread_cond_init(&new->turn, NULL);

	if (*start == NULL)
	{
		(*start) = new;
	}
	else 
	{
		Position *tmp = *start;
		while (tmp->next != NULL)
		{
			tmp = tmp->next;
		}
		tmp->next = new;
	}
	return new;
}


// letting the client in, from the waiting room.
Position *AllowClientIn()
{
	Position *tmp = barberQueue;
	barberQueue = barberQueue->next;
	return tmp;

}

void PrintStats()
{
	if (currentlyServedClient != 0)
	{
		printf("Currently served Client: %d | Sofa: %d/%d | Num of left clients: %d\n", currentlyServedClient, numberOfWaitingClients, numberOfChairs, numberOfResignedClients);
	}
	else
	{
		printf("Currently served Client: - | Sofa: %d/%d | Num of left clients: %d\n",numberOfWaitingClients, numberOfChairs,numberOfResignedClients);
	}
}


void Customer()
{
	int id;

// client enters the waiting room and changes the number of free chairs in the waiting room.
	pthread_mutex_lock(&accessChairs);
	totalNumberOfClients++;
	id = totalNumberOfClients;
// if there are free seats in the waiting room
	if (numberOfWaitingClients < numberOfChairs)
	{
// then we add the next one waiting and add it to the list
		numberOfWaitingClients++;
		Position *client = AddNewClientToQueue(&barberQueue, totalNumberOfClients);
		printf("New client arrived.\n");
		PrintStats();
// informs that the client is waiting
		pthread_cond_signal(&clientsAvailable);
		pthread_mutex_unlock(&accessChairs);
// waiting for barber
		pthread_mutex_lock(&accessBarber);
		while (currentlyServedClient != id)
		{
			pthread_cond_wait(&client->turn, &accessBarber);
		}
		pthread_mutex_unlock(&accessBarber);
		GetCut();
	}
	else 
	{
// add the resigned customer to the list.
		AddNewClientToQueue(&resignedClients, totalNumberOfClients);
		numberOfResignedClients++;
		printf("Customer left!\n");
		PrintStats();
		pthread_mutex_unlock(&accessChairs);
	}
}
void Barber()
{
	while(1)
	{
		pthread_mutex_lock(&accessChairs);
		while (numberOfWaitingClients == 0)
		{
			currentlyServedClient = 0;
			pthread_cond_wait(&clientsAvailable, &accessChairs);
		}
		numberOfWaitingClients--;
		Position *client= AllowClientIn(barberQueue);
		currentlyServedClient = client->id;
		printf("Next!\n");
        numberOfServedClients++;
		PrintStats();
// enable changing the state of the waiting room
		pthread_mutex_unlock(&accessChairs);
// signal readiness to cut
		pthread_cond_signal(&client->turn);
// cut the hair
		DoCut(client);
	}				
}

int main(int argc, char *argv[])
{
	int status = 0;
	pthread_t id1, id2;
	srand(time(NULL));
	if (argc < 2)
	{
		printf("Assign number of Chairs in WaitingRoom.\n");
		exit(-1);
	}
	numberOfChairs = atoi(argv[1]);
	

	status = pthread_create(&id1, NULL, (void*)Barber, NULL);
	if (status!=0)
	{
		printf("Barber thread couldn't start'\n");
		exit(status);
	}
	while (1)
	{

		int k = rand()%3;
		sleep(k);
		status = pthread_create(&id2, NULL, (void*)Customer, NULL);

        if(numberOfServedClients >= 50 ){
				return 0;  
        }

		if (status != 0)
		{
			printf("Customer thread couldn't start\n");
			exit(status);
		}
	}

	
}
