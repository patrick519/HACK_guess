// Patrick t Jong
// September 2017
// compile w/ g++ -O3 -Wall -Wextra -o dates dates.cc
// run w/ ./dates

#include <iostream> 
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <crypt.h>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;
vector<string> dates;

//create book with all dates from 1960
void createdates(){
	string hash,date;
	ostringstream oss;
	int yy;
	for (int y = 1960; y < 1985; y++){
		yy = y - 1900;
		for (int m = 1; m < 10; m++){
			for (int d = 1; d < 10; d++){
			oss << "0" << m << "0" << d << y;
			date = oss.str();
			dates.push_back(date);			
			oss.str("");
			oss << "0" << m << "0" << d << yy;
			date = oss.str();
			dates.push_back(date);			
			oss.str("");
			}	
			for (int d = 10; d < 32; d++){
			oss << "0" << m << d << y;
			date = oss.str();
			dates.push_back(date);
			oss.str("");
			oss << "0" << m << d << yy;
			date = oss.str();
			dates.push_back(date);
			oss.str("");
			}
		}//month 0 till 10	
		for (int m = 10; m < 13; m++){
			for (int d = 1; d < 10; d++){
			oss << m << "0" << d << y;
			date = oss.str();
			dates.push_back(date);
			oss.str("");
			oss << m << "0" << d << yy;
			date = oss.str();
			dates.push_back(date);
			oss.str("");
			}	
			for (int d = 10; d < 32; d++){
			oss << m << d << y;
			date = oss.str();
			dates.push_back(date);
			oss.str("");
			oss << m << d << yy;
			date = oss.str();
			dates.push_back(date);
			oss.str("");
			}	  
		}//month 10 till 12
	}//year
} //createdates

int main(void){

	cout << "Creating..";	
	createdates();
	
	cout << "writing..";
	//Output of dates
	ofstream outfile;
	int datessize = dates.size();
	outfile.open ("dates.txt");
	for (int i = 0; i < datessize; i++)
		outfile << dates[i] << "\n";
	outfile.close();

	cout << "done." << endl;
	return 0;
}

