/* 
Noah Podgurski
CS 280
Programming Assignment 2 - lexical analyzer
10/11/2019
*/
#include<stdio.h> 
#include<string>
#include <bits/stdc++.h> 
#include<iostream>
#include<fstream>
#include <cctype>
#include<map>
#include<time.h>
#include<list>
#include "lex.h"
// #include "lex.cpp"
using namespace std;

int intSort(vector<string> &ints){
	string tmp;
	for (int i = 0; i < ints.size(); i++){
		for (int j = i; j < ints.size(); j++){
			if (i != j){
				if (stoi(ints[i]) > stoi(ints[j])){
					tmp = ints[i];
					ints[i] = ints[j];
					ints[j] = tmp;
				}
			}
		}
	}
	return 0;
}

bool notInArray(vector<string> v, string str){
	for (int i = 0; i < v.size(); i++){
		if (v[i] == str)
			return false;
	}
	return true;
}

int main(int argc,char* argv[]) 
{ 
    string x, line, filename;
    int linenum = 0;
    bool v = false;
    bool consts = false;
    bool ids = false;
    bool file = false;
    int tokens = 0;

    vector<string> strings;// vector
    vector<string> ints;// vector
    vector<string> idsvec;// vector

	string arg;
	if (argc > 0){
		// check for bad arg first
		for (int i = 1; i < argc; i++){
			arg = argv[i];
			// if it starts with a flag
			if (arg[0] == '-'){
				if (arg != "-v" && arg != "-consts" && arg != "-ids"){
					cout << "UNRECOGNIZED FLAG " << arg << endl;
					return 0;
				}
			}
		}
		for (int i = 1; i < argc; i++){
			arg = argv[i];
			if (arg == "-v"){
				v = 1; //print every token
			}
			else if (arg == "-consts"){
				consts = 1; //print strings in alphabetical order, then integers in numeric order
			}
			else if (arg == "-ids"){
				ids = 1; //print all ids in alphabetical order
			}
			else {
				if (i == argc-1) { //if it's the last commnd line arg
					ifstream myfile(arg);
	                if (myfile.is_open()){ //open the file!
	                	// check if file is empty
	                	myfile.seekg(0, ios::end);
	                	if (myfile.tellg() == 0){
	                		cout << "Lines: 0" << endl;
	                		return 0;
	                	}
	                	file = 1;
	                	filename = arg;
	                	myfile.close();
	                }
	                else { //cannot open the file :()
	                	cout << "CANNOT OPEN " << arg << endl;
	                	return 0;
	                }
				} else { //more than 1 filename provided
					cout << "ONLY ONE FILE NAME ALLOWED" << endl;
					return 0;
				}
			}
		}

		if (file == 1){ //file stream
			ifstream myfile(filename);
			Lex data;

			while (( data = getNextToken(myfile, linenum)) != DONE && data.GetToken() != DONE && data.GetToken() != ERR){
				if (v == 1)
        			cout << data;

        		tokens++;
			

        		// -consts
        		if (data.GetToken() == STR)// if string, add to string array
        			if (notInArray(strings, data.GetLexeme()))
	    				strings.push_back(data.GetLexeme());
        		if (data.GetToken() == INT)//if int, add to int array
        			if (notInArray(ints, data.GetLexeme()))
	    				ints.push_back(data.GetLexeme());
        		// -ids
        		if (data.GetToken() == ID)
        			if (notInArray(idsvec, data.GetLexeme()))
	    				idsvec.push_back(data.GetLexeme());
		    }
		    //CHECK FOR ERROR
        	if (data.GetToken() == ERR){
        		cout << data;
        		return 0;
	        }
			if (consts == 1){
				//STRINGS
				sort(strings.begin(), strings.end());// sort strings vector
				if (strings.size() > 0)
					cout << "STRINGS:" << endl; //space??
				for (int i = 0; i < strings.size(); i++){
					cout << strings[i] << endl;
				}
				
				//INTEGERS
				intSort(ints);//sort ints vector
				if (ints.size() > 0)
					cout << "INTEGERS: " << endl; //space??
				for (int i = 0; i < ints.size(); i++){
					cout << ints[i] << endl;
				}

			}
			if (ids == 1){
				//IDENTIFIERS
				sort(idsvec.begin(), idsvec.end());// sort idsvec vector
				if (idsvec.size() > 0)
					cout << "IDENTIFIERS: "; //space??
				for (int i = 0; i < idsvec.size(); i++){
					cout << idsvec[i];
					if (i != idsvec.size()-1)
						cout << ", ";
					else
						cout << endl;
				}
			}
			cout << "Lines: " << linenum << endl << "Tokens: " << tokens << endl;
        	
			myfile.close(); //dont forget to close!
		}
		else{ //standard input stream
			Lex data;

			while (( data = getNextToken(cin, linenum)) != DONE && data.GetToken() != DONE && data.GetToken() != ERR){
				if (v == 1)
        			cout << data;

        		tokens++;
			

        		// -consts
        		if (data.GetToken() == STR)// if string, add to string array
        			if (notInArray(strings, data.GetLexeme()))
	    				strings.push_back(data.GetLexeme());
        		if (data.GetToken() == INT)//if int, add to int array
        			if (notInArray(ints, data.GetLexeme()))
	    				ints.push_back(data.GetLexeme());
        		// -ids
        		if (data.GetToken() == ID)
        			if (notInArray(idsvec, data.GetLexeme()))
	    				idsvec.push_back(data.GetLexeme());
		    }
		    //CHECK FOR ERROR
        	if (data.GetToken() == ERR){
        		cout << data;
        		return 0;
	        }
			if (consts == 1){
				//STRINGS
				sort(strings.begin(), strings.end());// sort strings vector
				if (strings.size() > 0)
					cout << "STRINGS:" << endl; //space??
				for (int i = 0; i < strings.size(); i++){
					cout << strings[i] << endl;
				}
				
				//INTEGERS
				intSort(ints);// sort ints vector
				if (ints.size() > 0)
					cout << "INTEGERS:" << endl; //space??
				for (int i = 0; i < ints.size(); i++){
					cout << ints[i] << endl;
				}

			}
			if (ids == 1){
				//IDENTIFIERS
				sort(idsvec.begin(), idsvec.end());// sort idsvec vector
				if (idsvec.size() > 0)
					cout << "IDENTIFIERS: "; //space??
				for (int i = 0; i < idsvec.size(); i++){
					cout << idsvec[i];
					if (i != idsvec.size()-1)
						cout << ", ";
					else
					cout << endl;
				}
			}
			cout << "Lines: " << linenum << endl << "Tokens: " << tokens << endl;
		}

	}
	return 0;
}