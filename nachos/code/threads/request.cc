/*request.cc
Contains definition of all the functions in request.h
*/
#include "request.h"

//Costructor for request class
request::request(int s, int p, char l, char c, List<int>* m){
	uniqueId = s;
	passenger = p;
	departure = l;
	destination = c;
	seatsAssign = m;
}

//setter for unique Id
void request::setUniqueId(int s){
	uniqueId = s;
}

//setter for passenger
void request::setPassenger(int p){
	passenger = p;
}

//setter for destination
void request::setDest(char c){
	destination = c;
}

//setter for departure
void request::setDeparture(char l){
	departure = l;
}

//setter for seatsAssign
void request::setSeatsAssign(List<int>* m){
	seatsAssign = m;
}

//getter for uniqueId
int request::getUniqueId(){
	return uniqueId;
}

//getter for passenger
int request::getPassenger(){
	return passenger;
}

//getter for departure
char request::getDeparture(){
	return departure;
}

//getter for destination
char request::getDest(){
	return destination;
}

//getter for seatsAssign
List<int>* request::getSeatsAssign(){
	return seatsAssign;
}
