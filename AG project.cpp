#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 

using namespace std;

int randomNumber(int Min, int Max) {

	return Min + rand() % (Max - Min); ;
}

void afisare(int** indivizi) {
	for (int i = 0; i < 10; i++) {
		cout << "Individul ";
		cout << i + 1;
		cout << ": ";
		for (int j = 0; j < 4; j++) {
			cout << indivizi[i][j] << " ";//afisam cromozomii fiecarui invidid in parte
		}
		cout << endl;
	}
}

void afisare_fitnes_individual(float f[10]) {
	cout << "\nFitnesul individual: \n";
	for (int i = 0; i < 10; i++) {
		cout << f[i] << endl;
	}
}

void afisarea_probabilitatilor(float* pr) {
	cout << "\nProbabilitati individual: \n";
	for (int i = 0; i < 10; i++) {
		cout << pr[i]<<endl;
	}
}

void afisarea_intervalelor(float** interval) {
	cout << "\nInterval: \n";
	for (int i = 0; i < 10; i++) {
		cout << interval[i][0]<<" "<< interval[i][1]<<endl;
	}
}

void f_i(float* f, int** in) { //functia de calcul fitness individual
	for (int i = 0; i < 10; i++) {
		float n = abs(in[i][0] + 2 * in[i][1] + 3 * in[i][2] + 4 * in[i][3] - 30);//formula de evaluare a fitnessului
		f[i] = 1 / n;//impartirea
	}
}

void p_i(float* f, float* p, float ft) {
	for (int i = 0; i < 10; i++) {
		p[i] = f[i] / ft; //fitness individual / fitness total -probabilitatea 
	}
}

float f_t(float f_i[10]) {
	float fitnesul_total = 0;

	for (int i = 0; i < 10; i++) {
		fitnesul_total += f_i[i]; //suma tuturor fitnesurilor 
	}
	return fitnesul_total;
}

void c_i(float** in, float* p) {  //calculam intervalele 
	float a = 0;

	for (int i = 0; i < 10; i++) {
		in[i][0] = a;	//suma unde se termina intervalul precedent + intervalul urmator si probabilitatea lui
		in[i][1] = a + p[i];
		a = in[i][1];
	}
}

int g_i(float** in, float n) {
	for (int i = 0; i < 10; i++) {
		if (n >= in[i][0] && n < in[i][1]) {  //verificam in ce intervale se incadreaza numarul 
			return i; //returneaza individul
		}
	}
}

void s(int** in, float** inte, int** per) {//selectia 
	float number[20];

	for (int i = 0; i < 20; i++) {
		number[i] = (float)randomNumber(0, 9999) / 10000; //20 de numere intre 0 si 1- 10 perechi
	}

	for (int i = 0; i < 10; i++) {
		per[i][0] = g_i(inte, number[i]);
		per[i][1] = g_i(inte, number[20 - 1 - i]); //verficam intervalele
	}
}


void incru(int** per, int** in, int** d) {//incrucisare simpla


	for (int i = 0; i < 10; i++) { //generam taietura 
		int taietura = randomNumber(1, 3);

		for (int j = 0; j < taietura; j++) {
			d[i][j] = in[per[i][0]][j];  //salvam de la primul cromozom pana la taietura
		}

		for (int j = taietura; j < 10; j++) {
			d[i][j] = in[per[i][1]][j];	//salvam de la taietura pana la capatul cromozomului 
		}
	}
}

void m(int** in) {//daca nu facem mutatie,toti indivizii au aceiasi cromzomi si nu o sa gaseasca ecuatia
	for (int i = 0; i < 10; i++) {
		int n = randomNumber(0, 7);
		if (n <= 3) {
			in[i][n] = randomNumber(1, 30); //numar inter 0-7, intre 0-3,individul i cromozomul n se modifica,daca random e mai mare de 3,nu se schimba nimic
		}
	}
}

int v_e(int** in) {
	for (int i = 0; i < 10; i++) {
		if (in[i][0] + 2 * in[i][1] + 3 * in[i][2] + 4 * in[i][3] == 30)
			return i; //verificam ecuatia,returneaza nrul la individ
	}
	return -1;//nu este gasit
}

int main() {

	srand(time(NULL));
	int** indivizi = new int* [10];
	float fInd[10];//fitnesul individual
	float pr[10];//probabilitatea 
	float** inte = new float* [10];//intervale
	int** p = new int* [10];//probabilitati

	float* fInd_res = &fInd[0];
	float* pr_res = &pr[0];

	int** descendenti = new int* [10];//descendenti
	for (int i = 0; i < 10; i++) {
		descendenti[i] = new int[4];
	}

	for (int i = 0; i < 10; i++) {
		p[i] = new int[2];
	}

	for (int i = 0; i < 10; i++) {
		indivizi[i] = new int[4];
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 4; j++) {
			indivizi[i][j] = randomNumber(1, 30);
		}
	}

	for (int i = 0; i < 10; i++) {
		inte[i] = new float[2];
	}
	int generatia = 0;
	bool gasit = false;

	while (!gasit) {
		cout << "\n\nGeneratia " << generatia+1 << "\n\n";

		int ind = v_e(indivizi);//verificam cine o returnat functia de verificare a ecuatiei
		afisare(indivizi);
		if (ind >= 0) {
			cout << "Raspuns gasit!! Generatia " << generatia+1 << ".Individul " << ind+1 << " indeplineste ecuatia " << indivizi[ind][0] << " " << indivizi[ind][1] << " " << indivizi[ind][2] << " " << indivizi[ind][3] << endl;
			break;
		}
		f_i(fInd, indivizi);
		afisare_fitnes_individual(fInd);
		float fitnesulTotal = f_t(fInd);
		cout << "\nFitnesul total: " << fitnesulTotal<<"\n";
		p_i(fInd, pr, fitnesulTotal);
		afisarea_probabilitatilor(pr);
		c_i(inte, pr);
		afisarea_intervalelor(inte);
		s(indivizi, inte, p);
		incru(p, indivizi, descendenti);
		indivizi = descendenti;
		m(indivizi);
		generatia++;
		cout << "------------------------------------------------\n";
	}


	return 0;
}
//daca nu facem mutatie,toti indivizii au aceiasi cromzomi si nu o sa gaseasca ecuatia
