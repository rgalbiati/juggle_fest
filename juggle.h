/*
 * juggle.h
 *
 * Raina Galbiati
 *
 *
 */

#ifndef JUGGLE_H_
#define JUGGLE_H_

#include <iostream>
#include <vector>


class Event {
public:
	void create_new_circuit(int name, int h_val, int e_val, int p_val);
	void create_new_juggler(int name, int h_val, int e_val, int p_val,
			std::vector<int>* choices);

	void print_circuits();
	void place_all_jugglers();


	void set_capacity();
private:
	class Circuit {
	public:
		int cID;
		int h_val;
		int e_val;
		int p_val;

		//contains IDs of jugglers in juggler array
		std::vector<int>members;
	};
	class Juggler {
	public:
		class Pref {
		public:
			int cID;
			int mat_score;
		};

		int jID;
		int h_val, e_val, p_val;
		std::vector<Pref> choices;
		int next_choice;
		float match_val;	//value for current placement

		int get_next_choice ();
		void print_preferences();
	};

	int calculate_score(Juggler &j, Circuit &c);
	void place_juggler(Juggler &j);
	int find_weakest(Circuit &c);
	void compare_jugglers(Juggler &j, int score, Circuit &c);

	std::vector<Circuit>circuits;
	std::vector<Juggler>jugglers;

	int circuit_capacity;

};


#endif /* JUGGLE_H_ */
