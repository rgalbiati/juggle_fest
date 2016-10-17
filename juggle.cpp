/*
 * juggle.cpp
 *
 * Raina Galbiati
 *
 *
 */
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include "juggle.h"

/*
 * initializes instance of circuit and pushes it to the vector of circuits
 * args: ID of circuit, H, E and P values
 */
void Event::create_new_circuit(int name, int h_val, int e_val, int p_val)
{
	Circuit c;
	c.cID = name;
	c.h_val = h_val;
	c.e_val = e_val;
	c.p_val = p_val;

	circuits.push_back(c);
}

/*
 * initializes instance of circuit and pushes it to the vector of circuits
 * args: ID of circuit, H, E and P values, vector holding ID of
 * 	juggler's circuit preferences
 */
void Event::create_new_juggler(int name, int h_val, int e_val, int p_val,
		std::vector<int>* choices)
{
	Juggler j;
	j.jID = name;
	j.h_val = h_val;
	j.e_val = e_val;
	j.p_val = p_val;
	j.match_val = 0;

	for (int i = 0; i < (int)choices->size(); i++) {
		Juggler::Pref p;
		p.cID = choices->at(i);
		p.mat_score = calculate_score(j, circuits[choices->at(i)]);
		j.choices.push_back(p);
	}
	j.next_choice = 0;
	jugglers.push_back(j);
}

/*
 * finds and sets the number of jugglers per circuit
 */
void Event::set_capacity()
{
	circuit_capacity = jugglers.size()/circuits.size();
}

/*
 * calculates the dot product of the juggler's and circuits H,E, and P values
 * arg: a reference to a juggler and a reference to a circuit
 */
int Event::calculate_score(Juggler &j, Circuit &c)
{
	return (c.h_val*j.h_val)+(c.e_val*j.e_val)+(c.p_val*j.p_val);
}

/*
 * calls a helper function to place each juggler onto a circuit
 */
void Event::place_all_jugglers()
{
	for (int i = 0; i < (int)jugglers.size(); i++)
	{
		place_juggler(jugglers[i]);
	}
}

/*
 * places a juggler onto a circuit based on their dot product score
 */
void Event::place_juggler(Juggler &j)
{
	int choice = j.get_next_choice();//returns the index of the next choice

	// if they have exhausted their choices, place on random team
	if (choice == -1) {
		for (int i = 0; i < (int)circuits.size(); i++){
			//find a team with space left on it
			if ((int)circuits[i].members.size() < circuit_capacity){
				circuits[i].members.push_back(j.jID);
				j.match_val = calculate_score(j,circuits[i]);
				return;
			}
		}
		//should not get here if jugglers % circuits = 0
		std::cerr << "not enough teams" << std::endl;
		exit(1);
	}

	//if team of choice still has space
	if ((int)circuits[choice].members.size() < circuit_capacity) {
		circuits[choice].members.push_back(j.jID);
		j.match_val = j.choices[j.next_choice].mat_score;
		j.next_choice++;
		return;
	}
	else {
		compare_jugglers(j, j.match_val =
				j.choices[j.next_choice].mat_score,
				circuits[choice]);
	}
}

/*
 * determines if new juggler is a better fit than the circuits current worst
 * 	fit and swaps them if so
 * arg: reference to the new juggler, thier match score, and a reference to
 * 	the circuit
 */
void Event::compare_jugglers(Juggler &j, int score, Circuit &c)
{
	int weakest = find_weakest(c);

	//if the new juggler is a better fit replace old juggler with new
	//juggler and call func to reassign them a new team
	if (jugglers[c.members[weakest]].match_val < score) {
		int tempID = jugglers[c.members[weakest]].jID;
		c.members[weakest] = j.jID;
		j.next_choice++;
		place_juggler(jugglers[tempID]);
	}
	else {
		//try to place juggler on team of next preference
		j.next_choice++;
		place_juggler(j);
	}
}

/*
 * finds the juggler currently on the circuit that is the worst fit (has the
 * 	lowest match score)
 */
int Event::find_weakest(Circuit &c)
{
	//index of weakest juggler
	int weakest = 0;

	for (int i = 1; i < (int)c.members.size(); i++) {
		if (jugglers[c.members[i]].match_val <
				jugglers[c.members[weakest]].match_val){
			weakest = i;
		}
	}
	return weakest;
}

/*
 * returns the ID of the juggler's next choice circuit
 */
int Event::Juggler::get_next_choice ()
{
	if (next_choice >= (int)choices.size())
		return -1;	//sentinel indicates exhaustion of preferences

	else
		return choices[next_choice].cID;
}

/*
 * prints the ID and match score of each of the juggler's circuit choices
 */
void Event::Juggler::print_preferences()
{
	std::cout << 'J' << jID << " ";
	for (int i = 0; i < (int)choices.size(); i++) {
		std::cout << 'C' << choices[i].cID << ':'
				<< choices[i].mat_score << " ";
	}
}

/*
 * prints each circuit and the jugglers on each circuit
 */
void Event::print_circuits()
{
	for (int i = 0; i < (int)circuits.size(); i++) {
		std::cout << 'C' << circuits[i].cID << " ";

		int n = (int)circuits[i].members.size();
		for (int k = 0; k < n; k++) {
			jugglers[circuits[i].members[k]].print_preferences();
		}
		std::cout << std::endl << std::endl;
	}
}
