// Patrick 't Jong
// September 2017
// DataStructure hashtable from http://algolist.net
// compile w/ g++ -O3 -Wall -Wextra -o hash hash.cc -lcrypt
// run w/ ./hash

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string.h>
#include <unistd.h>
#include <crypt.h>
#include <stdio.h>

using namespace std;

const int TABLE_SIZE = 2000003;
string salt;
string users[40000][3]; // USERNAME | NAME | PASSW HACKED
string book[2000000];
string bigbook[2000000];
int iusers, ibook, ibigbook, bookend;
int counter;

class HashEntry {
private:
	unsigned key;
	vector<string> values;
public:
	HashEntry(unsigned key, string value){
		this->key = key;
		this->values.push_back(value);
	} //Constructor
	
	void addValue(string value){
		this->values.push_back(value);
	} //getValue
	
	unsigned getKey(){
		return key;
	} //getKey

	vector<string> getValue(){
		return values;
	} //getValue
}; //HashEntry

class HashMap {
private:
	HashEntry **table;
public:
	HashMap() {
		table = new HashEntry*[TABLE_SIZE];
		for (int i = 0; i < TABLE_SIZE; i++)
			table[i] = NULL;
	} //HashMap
	
	vector<string> get(unsigned key) {
		unsigned hashfcn = (key % TABLE_SIZE);
		while (table[hashfcn] != NULL && table[hashfcn]->getKey() != key)
			hashfcn = (hashfcn + 1) % TABLE_SIZE;
		if (table[hashfcn] == NULL)
			return vector<string>();
		else
			return table[hashfcn]->getValue();
	} //get
	
	void put(unsigned key, string value) {
		unsigned hashfcn = (key % TABLE_SIZE);
		while (table[hashfcn] != NULL && table[hashfcn]->getKey() != key)
			hashfcn = (hashfcn + 1) % TABLE_SIZE;
		if (table[hashfcn] != NULL)
			table[hashfcn]->addValue(value);
		else
			table[hashfcn] = new HashEntry(key, value);
	} //put
	
	void pop(unsigned key) {
		unsigned hashfcn = (key % TABLE_SIZE);
		while (table[hashfcn] != NULL && table[hashfcn]->getKey() != key)
			hashfcn = (hashfcn + 1) % TABLE_SIZE;
		if (table[hashfcn] != NULL){
			delete table[hashfcn];
			table[hashfcn] = NULL;
		}
	} //pop   

	~HashMap() {
		for (int i = 0; i < TABLE_SIZE; i++)
			if (table[i] != NULL)
				delete table[i];
		delete[] table;
	} //deconstructor
}; //HashMap

unsigned hashfcn(const char* str){
	unsigned h = 0;
	while (*str)
		h = h * 101 + (unsigned) *str++;
	return h;
} //hashfcn 

void readshadow(HashMap *map, string inputloc){
	string line, token, user, hash;
	salt = "-1";
	iusers = 0;
	ifstream file (inputloc.c_str());
	if (file.is_open()){
		while (getline(file,line)){
			istringstream iss (line);
			getline(iss, token, ':'); 
			user = token;
			getline(iss, token, ':');
			hash = token;
			if (salt == "-1")
				salt = hash;
			map->put(hashfcn(hash.c_str()),user);
			users[iusers][0] = user;
			iusers++;
		};
		file.close();
	} //if
}//readshadow

void readpasswd(string inputloc){
	string line, token;
	ifstream file (inputloc.c_str());
	int j = 0;
	if (file.is_open()){
		while (getline(file,line)){
			istringstream iss (line);
			for (int i = 0; i < 5; i++) //skip4:
				getline(iss, token, ':');
			token = token.substr(0, token.size()-3);
			users[j][1] = token;
			j++;
		};
		file.close();
	} //if	
}//readpasswd

void writebook(string line){
	book[ibook] = line;
	ibook++;
}//writebook

void readbook(string inputloc){
	string line;
	ifstream file (inputloc.c_str());
	if (file.is_open()){
		while (getline(file,line)){
			writebook(line);
		};
		file.close();
	} //if
}//readbook

void readbigbook(string inputloc){
	string line;
	ifstream file (inputloc.c_str());
	if (file.is_open()){
		while (getline(file,line)){
			bigbook[ibigbook] = line;
			ibigbook++;
		};
		file.close();
	} //if
}//readbook

//Based on known shadow, create hash with same salt
string createhash(string guess){
	//hashcnt++;
	const char* csalt = salt.c_str(); 
	char *result = crypt(guess.c_str(), csalt);
	string hash(result);
	return hash;
}//createhash

void bruteforce(HashMap *map, string password){
	string token, user, hashp, hashg;
	hashg = createhash(password);
	vector<string> values = map->get(hashfcn(hashg.c_str()));
	if (!values.empty()){
		for (int i = 0; i < (int)values.size(); i++){
			counter++;
			for (int j = 0; j < iusers; j++)
				if (users[j][0] == values[i]){
					users[j][2] = "true";
					j = iusers; //endloop
				} //if
			cout << values[i] << ":" << password << endl; //wegschrijven
		}
		map->pop(hashfcn(hashg.c_str()));
	}
}//bruteforce

void makename(){
	string user, token, firstn, secn, last;
	for (int i = 0; i < iusers; i++){
		user = users[i][1];
		istringstream iss (user);
		getline(iss, token, ' '); //Name
		firstn = token;
		writebook(firstn);
		firstn[0] = tolower(firstn[0]); //name
		writebook(firstn);
		transform(firstn.begin(), firstn.end(),firstn.begin(), ::toupper); //NAME
		writebook(firstn);
		if (count(user.begin(), user.end(), ' ') > 1){
			getline(iss, token, ' '); //Name
			secn = token;
			writebook(secn);
			secn[0] = tolower(secn[0]); //name
			writebook(secn);
			transform(secn.begin(), secn.end(),secn.begin(), ::toupper); //NAME
			writebook(secn);
		}
		getline(iss, token, ' '); //Last
		last = token;
		writebook(last);
		last[0] = tolower(last[0]); //last
		writebook(last);
		transform(last.begin(), last.end(),last.begin(), ::toupper); //LAST
		writebook(last);
	}//foriusers
}//makename

void makenameyy(){
	string token, name, namey, user, firstn;
	ostringstream nameoss;
	int yy;
	for (int i = 0; i < iusers; i++)
		if (users[i][2] != "true"){ //notguessed
			user = users[i][1];
			istringstream iss (user);
			getline(iss, token, ' ');
			firstn = token;
			firstn[0] = tolower(firstn[0]); //name
			for (int y = 1963; y < 1985; y++){
				yy = y - 1900;
				nameoss.str("");
				nameoss << firstn << y;
				namey = nameoss.str();
				writebook(namey);
				nameoss.str("");
				nameoss << firstn << yy;
				namey = nameoss.str();
				writebook(namey);
			} //for
		}//if
}//makenameyy

void makefirstsign(){
	string line, part;
	ostringstream newline;
	for (int i = 0; i < ibook; i++){
		line = book[i];
		if (line[0] == 'w'){
			part = line.substr(1);
			newline << "\\|/" << part;
			writebook(newline.str());
			newline.str("");
		}
		if (line[0] == 'n'){
			part = line.substr(1);
			newline << "]\\[" << part;
			writebook(newline.str());
			newline.str("");
		}
		if (line[0] == 'k'){
			part = line.substr(1);
			newline << "|(" << part;
			writebook(newline.str());
			newline.str("");
		}
		if (line[0] == 'n'){
			part = line.substr(1);
			newline << "|\\|" << part;
			writebook(newline.str());
			newline.str("");
		}
		if (line[0] == 'm'){
			part = line.substr(1);
			newline << "|`'|" << part;
			writebook(newline.str());
			newline.str("");
		}
		if (line[0] == 'k'){
			part = line.substr(1);
			newline << "|<" << part;
			writebook(newline.str());
			newline.str("");
		}
		if (line[0] == 'p'){
			part = line.substr(1);
			newline << "|>" << part;
			writebook(newline.str());
			newline.str("");
		}
		if (line[0] == 'm'){
			part = line.substr(1);
			newline << "|v|" << part;
			writebook(newline.str());
			newline.str("");
		}
		if (line[0] == 't'){
			part = line.substr(1);
			newline << "~|~" << part;
			writebook(newline.str());
			newline.str("");
		}
		if (line[0] == 'l'){
			part = line.substr(1);
			newline << "1" << part;
			writebook(newline.str());
			newline.str("");
		}
		if (line[0] == 'b'){
			part = line.substr(1);
			newline << "8" << part;
			writebook(newline.str());
			newline.str("");
		}
		if (line[0] == 'g'){
			part = line.substr(1);
			newline << "6" << part;
			writebook(newline.str());
			newline.str("");
		}
		if (line[0] == 'v'){
			part = line.substr(1);
			newline << "`'" << part;
			writebook(newline.str());
			newline.str("");
		}
	}//for
}//makefirstsign

void makelastsign(){
	string line, part, car;
	ostringstream newline;
	int size;
	for (int i = 0; i < ibook; i++){
		line = book[i];
		size = strlen(line.c_str())-1;
		car = line[size];
		if (car == "t"){
			part = line.substr(0,size);
			newline << part << "~|~";
			writebook(newline.str());
			newline.str("");
		}
		if (car == "n"){
			part = line.substr(0,size);
			newline << part << "]\\[";
			writebook(newline.str());
			newline.str("");
		}
		if (car == "n"){
			part = line.substr(0,size);
			newline << part << "|\\|";
			writebook(newline.str());
			newline.str("");
		}
		if (car == "m"){
			part = line.substr(0,size);
			newline << part << "|v|";
			writebook(newline.str());
			newline.str("");
		}
		if (car == "p"){
			part = line.substr(0,size);
			newline << part << "|>";
			writebook(newline.str());
			newline.str("");
		}
		if (car == "k"){
			part = line.substr(0,size);
			newline << part << "|<";
			writebook(newline.str());
			newline.str("");
		}
		if (car == "k"){
			part = line.substr(0,size);
			newline << part << "|(";
			writebook(newline.str());
			newline.str("");
		}
	}//for
}//makelastsign


int main(int, char* argv[]){
	HashMap *map = new HashMap;
	ibook = 0;
	ibigbook = 0;
	int maxbook = 0;

	if (argv[2] != NULL)
		readshadow(map,argv[2]);
	if (argv[1] != NULL)
		readpasswd(argv[1]);

	counter = 0;

	readbook("250.txt");
	readbook("allbooks.txt");
	readbook("dates.txt");
	makename();
	for (int i = 0; i < ibook; i++){
		bruteforce(map,book[i]);
		maxbook = i;
	}
	readbigbook("longwords.txt");
	for (int i = 0; i < ibigbook; i++){
		bruteforce(map,bigbook[i]);
	}
	makenameyy();
	for (int i = maxbook; i < ibook; i++){
		bruteforce(map,book[i]);
		maxbook = i;
	}
	makefirstsign();
	for (int i = maxbook; i < ibook; i++){
		bruteforce(map,book[i]);
		maxbook = i;
	}
	makelastsign();
	for (int i = maxbook; i < ibook; i++){
		bruteforce(map,book[i]);
		maxbook = i;
	}	

	//cout << "Guessed " << counter << endl;

	//Output of statics
	ofstream outfile;
	outfile.open ("openshadow.txt");
	for (int i = 0; i < iusers; i++)
		if (users[i][2] != "true")
			outfile << users[i][0] << "\n";
	outfile.close();

	return 0;
} //main
