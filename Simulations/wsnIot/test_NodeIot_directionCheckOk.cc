#include <iostream>
#include <assert.h>

using namespace std;
bool justReturned;

bool directionCheckOk (const int direction)//, bool justReturned)
{
    if (1 == direction ) {//0 means going away from sink
        if (false == justReturned )
            return false;
        else {
            justReturned = false;
            return false;
        }
    }
    else{ // direction == 0
        if (false == justReturned ) {
            justReturned = true;
            return true;
        }
        else
            return false;
    }
    //TODO adjut how should i actually send data packets after reciving drop replies.
}

int main() {
	// your code goes here

	justReturned = false; //test cases here are sequencial, and dependent on each other for justReturned value
	
	//test going towards sink means direction 1 
	assert (directionCheckOk (1) == false);
	cout<< "justReturned is " << justReturned << endl;
	
	//test just returning after direction change
	assert (directionCheckOk(0) == true);
	cout<< "justReturned is " << justReturned << endl;
	
	//test  returning after direction change but continueing back
	//justReturn 
	assert (directionCheckOk(0) == false);
	cout << "justReturned is " << justReturned << endl;
	
	//test  going back towards sink
	assert (directionCheckOk(1) == false);
	cout << "justReturned is " << justReturned << endl;
	
	//first test again
	assert (directionCheckOk (1) == false);
	cout<< "justReturned is " << justReturned << endl;
	
	//second test again
	assert (directionCheckOk(0) == true);
	cout<< "justReturned is " << justReturned << endl;
	
	cout<< "salam";
	return 0;
}


