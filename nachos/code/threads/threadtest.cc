#include "kernel.h"
#include "main.h"
#include "thread.h"
#include "request.h"
#include "list.h"
#include "bitmap.h"
using namespace std;


//Variables required for Simulation
List<request*> *onPlane = new List<request*>();
List<request*> *discarded = new List<request*>();
int uId = 100;
Bitmap *seats = new Bitmap(20);
List<Thread*> *BList = new List<Thread*>();
List<Thread*> *CList = new List<Thread*>();
List<Thread*> *DList = new List<Thread*>();
List<Thread*> *EList = new List<Thread*>();

//Decide the Destination for the request according to predefined probabilities
char calculateDest(char dept){
	switch(dept)
	{
		case 'A': if ( rand() % 100 < 10)
					return 'B';
				  else if(rand() % 100 < 30)
				  	return 'C';
				  else if(rand() % 100 < 60)
				  	return 'D';
				  else 
				  	return 'E';
				break;
		case 'B': if(rand() % 100 < 20)
					return 'C';
				  else if(rand() % 100 < 50)
				  	return 'D';
				  else 
				  	return 'E';
				break;
		case 'C': if(rand() % 100 < 35 )
					return 'D';
				  else
				  	return 'E';
				break;
		case 'D': return 'E';
				break;
		default: return 'E';
	}
}

//Prints the generated request
void printReq(request* req){

	printf("\nReqId: %d,  Passengers: %d, Departure: %c, Destination: %c ",req->getUniqueId(),req->getPassenger(), req->getDeparture(), req->getDest() ); 
}

//Print the available seats on the plane
void printClearBits(){
	printf("\nAvailable seat numbers:");
	if(seats->NumClear() > 0){
		for(int i = 0; i <20 ; i++){
			if(!seats->Test(i))
				printf(" %d ", i+1);
		}
	}else
		printf(" No Seat available");
}

//Prints Seats assigned and number of available seats
void printSeats(request* req){
	ListIterator<int> *iter1 = new ListIterator<int>(req->getSeatsAssign());

    printf("\n\nRequest Successfull: %d", req->getUniqueId());
    if(!iter1->IsDone()){
    	printf("\nSeat Assigned:"); 
    	for (; !iter1->IsDone(); iter1->Next()) {
     	 printf(" %d ",iter1->Item()) ;    
    	}
    }   
    printf("\nNumber of Seats available: %d", seats->NumClear()); 
    printClearBits(); 
}


//Prints the Request Id's of requests on the plane and occupancy rate
void printOnPlane(){
	ListIterator<request*> *iter1 = new ListIterator<request*>(onPlane);
	printf("\nReqId’s of Request on the Plane:");
    if(!iter1->IsDone()){
    	for (; !iter1->IsDone(); iter1->Next()) {
    		request *req = iter1->Item();
     	 	printf(" %d ", req->getUniqueId()) ;    
    	}
   	} else
   		printf(" No Passenger on Plane");
   	float oRate = (float) (20 - seats->NumClear())*5;
    printf("\nOccupancy Rate : %0.2f %%", oRate); 
}

//Prints the discarded requests
void printDiscard(){
	ListIterator<request*> *iter1 = new ListIterator<request*>(discarded);
    if(!iter1->IsDone()){
    	printf("\n\nDiscarded ReqId’s:");
    	for (; !iter1->IsDone(); iter1->Next()) {
    		request *req = iter1->Item();
     	 	printf(" %d ", req->getUniqueId()) ;    
    	}
   	} 
   	printf("\n"); 
}

//Clear the seats of the passengers getting off the plane
void clearSeats(request* req){
	onPlane->Remove(req);
	ListIterator<int> *iter1 = new ListIterator<int>(req->getSeatsAssign());
	printf("\n\nPassengers of the following request are getting off at %c:", req->getDest());
    printf("\nReqId: %d", req->getUniqueId());  
    if(!iter1->IsDone()){
    	printf("   Seats Returned:");
    	for (; !iter1->IsDone(); iter1->Next()) {
     	 printf(" %d ",iter1->Item()) ; 
     	 seats->Clear(iter1->Item() -1);   
    	}
    } 
    printf("\nNumber of Seats available: %d", seats->NumClear()); 
    printClearBits(); 
    printOnPlane();

}

/*Allocate the seats and if the request is granted then the current thread will be stored in a list corresponding to a destination city
If required seats are not available then discards the request*/
void allocateSeat(request* req){
	int passenger = req->getPassenger();
	char dest = req->getDest();
	List<int> *m = new List<int>();
	printf("\nRequest Generated:");
	printReq(req);
	if( passenger <= seats->NumClear()){
		for(int i=0; i<passenger;i++){
			int seat = seats->FindAndSet();
			m->Append(++seat);
		}
		req->setSeatsAssign(m);
		onPlane->Append(req);
		printSeats(req);
		switch(dest){
			case'B':BList->Append(kernel->currentThread);
				break;
			case 'C':CList->Append(kernel->currentThread);
				break;
			case 'D':DList->Append(kernel->currentThread);
				break;
			case 'E':EList->Append(kernel->currentThread);
				break;
			default:printf("\ndestination : %c", dest);
		}
		kernel->interrupt->SetLevel(IntOff);
		kernel->currentThread->Sleep(FALSE);

		clearSeats(req);
	}else{
		discarded->Append(req);
		printf("\nRequest Discarded: %d", req->getUniqueId() );
		kernel->currentThread->Finish();
	}	
}

//Creates the Request 
void createRequest(char dept)
{
	printf("\n");
	uId++;
	int passenger = (rand() % 5) + 1;
	char dest = calculateDest(dept);
	List<int> *m = new List<int>();
	request *req1 = new request(uId,passenger,dept,dest,m);
	allocateSeat(req1);
}

//The Flight thread calls Scheduler::ReadyToRun() on all the threads in the list for the destination city to wake them up
void runThread(List<Thread*>* list){
	if(!list->IsEmpty()){
		int size = list->NumInList();
		for(int i=0; i < size;i++){  
			Thread *t = list->RemoveFront();
			kernel->interrupt->SetLevel(IntOff);
			kernel->scheduler->ReadyToRun(t);
		}
	}
}

//Select the list of destination city and call runThread function
void callRunThread(char dept){

	if(dept != 'A'){
		switch(dept){
			case 'B':runThread(BList);	
				break;
			case 'C':runThread(CList);
				break;
			case 'D':runThread(DList);
				break;
			case 'E':runThread(EList);
				break;
			default: printf("\ndept: %c", dept);
		}
	}
}

//Creates the Reservation threads and call callRUnThread fucntion to woke up the threads in the list
void
SimpleThread(int d)
{
	srand(time(0));
	for(char dept = 'A'; dept <= 'E'; dept++){
		callRunThread(dept);
		
		if(dept != 'E'){
			for(int i = 1; i<6 ;i++){
   				Thread *res1 = new Thread("reserve");
    			res1->Fork((VoidFunctionPtr) createRequest, (void *) dept);

    		}
   		}
    	kernel->currentThread->Yield();
	}
	printDiscard();
}

//Create Filght thread
void
ThreadTest()
{
    Thread *flightA = new Thread("flight");
    flightA->Fork((VoidFunctionPtr) SimpleThread, (void *) 1);
}
