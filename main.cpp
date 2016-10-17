/*
 * main.cpp
 * Raina Galbiati
 * Jugglers problem
 * Purpose: place jugglers onto teams
 * NOTE: as of now, program assumes that circuits and jugglers were added in
 * 	number (their name corresponds with the index they were stored in)
 *
 */
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include "juggle.h"
#define toDigit(c) (c-'0')


void read_file(char * argv[], Event &event);
int extract_value(std::string input);
void get_choices(std::string input, std::vector<int>* choices);

int main(int argc, char * argv[])
{
	if (argc != 2) {
		std::cerr << "usage program file1\n";
		exit(1);
	}

	Event event;
	read_file(argv, event);
	event.place_all_jugglers();
	event.print_circuits();

	return 0;
}

/*
 * opens file and saves data
 * args: command line arguments, reference to Event instance
 */
void read_file(char * argv[], Event &e)
{
	std::ifstream file (argv[1]);

	if (!file)
	{
	    std::cerr << "unable to open file" << std::endl;
	    exit(1);
	}

	std::string input, type;
	int h_val, e_val, p_val, name;
	std::vector<int> *choices = new std::vector<int>;

	//ensures vector starts with capacity for at least five elements
	choices->reserve(10);

	//reads file word by word.  >> stops reading at each space or \n
	while  (file >> type) {
		if (type == "C" or type == "J") {
			file >> input;
			name = extract_value(input);
			file >> input;
			h_val = extract_value(input);
			file >> input;
			e_val = extract_value(input);
			file >> input;
			p_val = extract_value(input);
			if (type == "C") {
				e.create_new_circuit(name, h_val, e_val, p_val);
			} else {
				file >> input;
				get_choices(input, choices);
				e.create_new_juggler(name, h_val, e_val, p_val,
						choices);
				choices->clear();
			}
		}
	}
	delete choices;
	file.close();
	e.set_capacity();
}

/*
 * extracts number from word of letters and nums and changes the type to int
 * arg: string to be converted
 * ret: integer from within the string
 */
int extract_value(std::string input)
{
	std::string num = "";
	int value;

	for (int i = 0; i < (int)input.length(); i++) {
		char c = input[i];

		//specific to input file format
		if (c != 'C' and c != ','and c != 'J' and c != ':' and
				c != 'H' and c != 'E' and c != 'P'){
			//--> input[i] must be a number
			num += input[i];
		}
	}
	value = atoi(num.c_str());
	return value;
}

/*
 * extracts individual numbers in a string of choices separated by commas
 * 	and 'C's
 * arg: string of the choices and a pointer to empty vector to hold the values
 */
void get_choices(std::string input, std::vector<int>* choices)
{
	int i = 0;
	std::string num = "";

	while (i < (int)input.length()) {
		if(input[i] != 'C') {
			while (input[i] != ',') {
				num += input[i];
					i++;
			}
			choices->push_back(atoi(num.c_str()));
			num = "";
		}
		i++;
	}
}
