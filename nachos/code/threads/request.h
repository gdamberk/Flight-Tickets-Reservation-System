/*request.h
Declare all the variables and methods required for the request class
*/

#include "debug.h"
#include "list.h"
class request
{
	private:
		int uniqueId;
		int passenger;
		char departure;
		char destination;
		List<int>* seatsAssign;
	public:
		~request();
		request(int s, int p, char l, char c, List<int> *m );

		void setUniqueId(int s);
		void setPassenger(int p);
		void setDest(char c);
		void setDeparture(char l);
		void setSeatsAssign(List<int>* m);

		int getUniqueId();
		int getPassenger();
		char getDest();
		char getDeparture();
		List<int>* getSeatsAssign();
};
