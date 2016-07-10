#include <iostream>
#include <ctime>
#include <string>
#include <list>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

#define NB_POP 40
#define NB_CHAR 95
#define TX_ECART 1

string s_cible("Best string ever");

void init_pop(list<string>* pop){
	for (int i = 0; i < NB_POP; i++){
		string indiv;
		for (int j = 0; j < s_cible.size(); j++){
			indiv.push_back(char(rand() % (128 - 32) + 32));
		}
		pop->push_back(indiv);
	}
}

void aff_pop(list<string>* pop, vector<int>* fit, int* fitmin){
	int i = 0;
	int fitmax = 10000000, idmax = 0;
	string str;
	for (list<string>::iterator it = pop->begin(); it != pop->end(); it++){
		//cout << *it << " ["<<fit->at(i)<<"]"<< endl;
		if (fit->at(i) < fitmax){
			idmax = i;
			fitmax = fit->at(i);
			str = *it;
		}
		i++;
	}
	if (fitmax < *fitmin){
		cout << str << " [" << fitmax << "]" << endl;
		*fitmin = fitmax;
		cout << endl;
	}
	
}

int calc_fit(string indiv){
	int somme = 0;
	for (int i = 0; i < indiv.size(); i++){
		somme += /*NB_CHAR -*/ abs(indiv[i] - s_cible[i]);
	}
	return somme;
}

void set_fit(vector<int>* fit, list<string>* pop){
	int i = 0;
	for (list<string>::iterator it = pop->begin(); it != pop->end(); it++){
		fit->at(i) = calc_fit(*it);
		i++;
	}
}

string mutation(string indiv){
	int i = rand() % indiv.size();
	int j = rand()%(i+1);
	if (j > 2){
		j /= 2;
	}
	for (int k = 0; k < j; k++){
		if (indiv.at(i) != s_cible.at(i)){
			indiv.at(i) = char(rand() % (128 - 32) + 32);
		}
		int o = indiv.size();
		do{
			i = rand() % indiv.size();
			o--;
		} while (indiv.at(i) == s_cible.at(i) && o != 0);
	}
	return indiv;
}

void mut_pop(list<string>* pop){
	for (list<string>::iterator it = pop->begin(); it != pop->end(); it++){
		*it = mutation(*it);
	}
}

bool find_min_fit(vector<int>* fit){
	for (int i = 0; i < fit->size(); i++){
		if (fit->at(i) == 0){
			return true;
		}
	}
	return false;
}

void init_proba(int proba[NB_POP], vector<int>* fit){
	proba[0] = fit->at(0);
	for (int i = 1; i < NB_POP; i++){
		proba[i] = fit->at(i) + proba[i-1] * TX_ECART;
	}
}

void tri_proba(int proba[NB_POP],vector<pair<int, int>>* proba_sort){
	for (int i = 0; i < NB_POP; i++){
		proba_sort->at(i).first = proba[i];
		proba_sort->at(i).second = i;
	}
	
	for (int d = NB_POP-1; d > 0; d--){
		for (int j = 0; j < d; j++){
			if (proba_sort->at(j).first < proba_sort->at(j + 1).first){
				pair<int, int> temp;
				temp.first = proba_sort->at(j).first;
				temp.second = proba_sort->at(j).second;
				proba_sort->at(j).first = proba_sort->at(j+1).first;
				proba_sort->at(j).second = proba_sort->at(j + 1).second;
				proba_sort->at(j + 1).first = temp.first;
				proba_sort->at(j + 1).second = temp.second;
			}
		}
	}
}

pair<string, string>* select_parents(list<string>* pop, int proba[NB_POP], pair<string, string>* parents){
	string p, m;
	
	vector<pair<int, int>> proba_sort(NB_POP);
	
	vector<string> list_s(NB_POP);
	int i = 0;
	for (list<string>::iterator it = pop->begin(); it != pop->end(); it++){
		list_s.at(i) = *it;
		i++;
	}

	 tri_proba(proba,&proba_sort);

	int r = rand() % proba_sort[0].first;
	for (int i = 0; i < NB_POP; i++){
		if (r < proba_sort[i].first){
			p = list_s.at(i);
		}
	}

	do{
		r = rand() % proba_sort[0].first;
		for (int i = 0; i < NB_POP; i++){
			if (r < proba_sort[i].first){
				m = list_s.at(i);
			}
		}
		//cout << "[" << r << "]" << " {" << proba_sort[0].first << "} " << "looking for a wife..." << endl;
	} while (m == p);
	parents->first = p;
	parents->second = m;
	return parents;
}

string creation_child(pair<string, string>* parents){
	int r = rand() % (parents->first.size()-2) + 1;
	string child;
	for (int i = 0; i < r; i++){
		child.push_back(parents->first.at(i));
	}
	for (int i = r; i < parents->second.size(); i++){
		child.push_back(parents->second.at(i));
	}
	return child;
}

void new_pop(list<string>* pop,int proba[NB_POP], int fitmin){
	pair<string, string> parents;
	for(list<string>::iterator it = pop->begin(); it != pop->end(); it++){
		do{
			*it = creation_child(select_parents(pop, proba, &parents));
		} while (calc_fit(*it) > fitmin*3);
	}
}

int main(){
	getline(cin, s_cible);
	if (s_cible == ""){
		s_cible = "Vive l'IA !!";
	}

	srand(time(NULL));
	list<string> population;
	vector<int> fitness(NB_POP);
	int proba[NB_POP];
	init_pop(&population);
	int fitmin = 10000000000;
	do{
		set_fit(&fitness, &population);
		aff_pop(&population,&fitness, &fitmin);
		init_proba(proba, &fitness);
		new_pop(&population, proba, fitmin);
		mut_pop(&population);
	} while (!find_min_fit(&fitness));
		
	system("PAUSE");
	return 0;
}