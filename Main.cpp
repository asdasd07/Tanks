#include <iostream>
#include <fstream>
#include <graphics.h>
#include <queue>
#include <string>
#include <vector>
#include <time.h>
#include "RotatedFigurs.h"

using namespace std;


class Element {
public:
	string* mapa;
	vector<Element*>* objects;
	int x, y, maxx, maxy, team;
	Element() {};
	~Element() {};
	virtual void destroi() {
	}
	virtual int getStatus() {
	}
	virtual void update() {
	}
	virtual int getTeam() {
		return 0;
	}
};

class Pocisk :public Element {
public:
	char orient;
	int px, py, stan;
	Pocisk(string* mt, vector<Element*>* En, int ny, int nx, char nOrient) {
		y = ny;
		x = nx;
		stan = 2;
		mapa = mt;
		objects = En;
		orient = nOrient;
		px = orient == 'l' ? -1 : (orient == 'r' ? 1 : 0);
		py = orient == 'u' ? -1 : (orient == 'd' ? 1 : 0);
	}
	void destroi() {
		stan = stan == 1 ? 0 : 1;
	}
	int getStatus() {
		return stan;
	}
	void update() {
		if (stan == 2) {
			char temp = mapa[y + py][x + px];
			if (temp == 'H') {
				mapa[y + py][x + px] = ' ';
				destroi();
			}
			if (temp == '#') {
				destroi();
			}
			for (int i = 0; i < objects->size(); i++) {
				if (objects[0][i]->x == x + px && objects[0][i]->y == y + py && objects[0][i]->getStatus() == 2) {
					objects[0][i]->destroi();
					destroi();
				}
			}
		}
		if (stan == 2) {
			x += px;
			y += py;
			draw();
		}
	}
	void draw() {
		setfillstyle(1, 15);
		setcolor(15);
		int poly[10] = { 23,30, 23,20, 25,15, 27,20, 27,30 };
		RPolygon(poly, 10, orient, x, y);
	}
};

class tank :public Element {
public:
	int coldown, lifes, type, playerNum, color, px, py;
	bool mov;
	char orient;
	Element* poc;
	int spawnx, spawny, maxy;
	tank(string* mt, vector<Element*>* elements, int spawnX, int spawnY, int pType, int pNumber) {
		objects = elements;
		mapa = mt;
		poc = NULL;
		lifes = 3;
		type = pType;
		spawnx = spawnX;
		spawny = spawnY;
		maxy = mapa->size();
		maxx = mapa[0].size();
		orient = 'u';
		coldown = 4;
		playerNum = pNumber;
		if (pNumber == 1) {
			color = 1;
			team = 1;
			px = 0;
			py = maxy - 2;
		}
		if (pNumber == 2) {
			color = 9;
			team = 1;
			px = maxx - 1;
			py = maxy - 2;
		}
		if (pNumber == 3) {
			color = 4;
			team = 2;
			px = maxx - 1;
			py = 0;
		}
		if (pNumber == 4) {
			color = 12;
			team = 2;
			px = 0;
			py = 0;
		}
		spawn();
	}
	void spawn() {
		x = spawnx;
		y = spawny;
	}
	void destroi() {
		lifes--;
		if (lifes > 0) {
			spawn();
		}
	}
	int getStatus() {
		if (lifes > 0) {
			return 2;
		}
		return 0;
	}
	void moving(char key) {
		int tempx = key == 'l' ? -1 : (key == 'r' ? 1 : 0);
		int tempy = key == 'u' ? -1 : (key == 'd' ? 1 : 0);
		if (orient == key && mov == true) {
			if (mapa[y + tempy][x + tempx] == ' ') {
				int emp = 0;
				for (int i = 0; i < objects->size(); i++) {
					if (objects[0][i]->x == x + tempx && objects[0][i]->y == y + tempy) {
						emp = 1;
					}
				}
				if (emp == 0) {
					x += tempx;
					y += tempy;
					mov = false;
				}
			}
		}
		else {
			orient = key;
			mov = true;
		}
	}
	int getTeam() {
		return team;
	}
	void shoot() {
		if (!poc && coldown == 4) {
			mov = false;
			coldown = 0;
			poc = new Pocisk(mapa, objects, y, x, orient);
			objects->push_back(poc);
		}
	}

	virtual void update() {
		if (poc && poc->getStatus() == 1) {
			poc->destroi();
			poc = NULL;
		}
		if (type == 1) {
			if (GetKeyState(VK_SPACE) & 0x8000) {
				shoot();
			}
			if (GetKeyState('A') & 0x8000) {
				moving('l');
			}
			if (GetKeyState('W') & 0x8000) {
				moving('u');
			}
			if (GetKeyState('D') & 0x8000) {
				moving('r');
			}
			if (GetKeyState('S') & 0x8000) {
				moving('d');
			}
		}
		if (type == 2) {
			if (GetKeyState(VK_RETURN) & 0x8000) {
				shoot();
			}
			if (GetKeyState(VK_LEFT) & 0x8000) {
				moving('l');
			}
			if (GetKeyState(VK_UP) & 0x8000) {
				moving('u');
			}
			if (GetKeyState(VK_RIGHT) & 0x8000) {
				moving('r');
			}
			if (GetKeyState(VK_DOWN) & 0x8000) {
				moving('d');
			}
		}
		draw();
	}

	void draw() {
		if (coldown < 4) {
			coldown++;
		}
		setfillstyle(1, color);
		setcolor(color);
		hearts(lifes, px, py, color);

		RBar(1, 10, 10, 49, orient, x, y);
		RBar(40, 10, 49, 49, orient, x, y);
		setcolor(0);
		if (mov == false) {
			RLine(0, 20, 50, 20, orient, x, y);
			RLine(0, 30, 50, 30, orient, x, y);
			RLine(0, 40, 50, 40, orient, x, y);
		}
		if (mov == true) {
			RLine(0, 15, 50, 15, orient, x, y);
			RLine(0, 25, 50, 25, orient, x, y);
			RLine(0, 35, 50, 35, orient, x, y);
			RLine(0, 45, 50, 45, orient, x, y);
		}
		setcolor(color);

		int poly[20] = { 12,20, 19,15, 19,20, 30,20, 30,15, 37,20, 37,35, 32,40, 17,40, 12,35 };
		RPolygon(poly, 20, orient, x, y);
		RBar(23, 17 - coldown * 4, 27, 20, orient, x, y);

	}
};

class Orzel :public Element {
public:
	int color, stan;
	Orzel(vector<Element*>* En, int tx, int ty, int pteam) {
		objects = En;
		x = tx;
		y = ty;
		stan = 2;
		team = pteam;
		if (pteam == 1) {
			color = 1;
		}
		else {
			color = 4;
		}
	}
	int getStatus() {
		return stan;
	}
	void update() {
		int tx = x * 50;
		int ty = y * 50 + 50;
		setfillstyle(1, color);
		setcolor(color);
		bar(tx + 22, ty + 10, tx + 28, ty + 50);
		int poly[8] = { tx + 0,ty + 0, tx + 22,ty + 25, tx + 22,ty + 35, tx + 10,ty + 30 };
		fillpoly(4, poly);
		int poly2[8] = { tx + 28,ty + 25, tx + 28,ty + 35, tx + 40,ty + 30, tx + 50,ty + 0 };
		fillpoly(4, poly2);
		int poly3[6] = { tx + 28,ty + 10, tx + 28,ty + 13, tx + 30,ty + 12 };
		fillpoly(3, poly3);
		int poly4[8] = { tx + 10,ty + 50, tx + 15,ty + 48, tx + 22,ty + 40, tx + 22,ty + 35 };
		fillpoly(4, poly4);
		int poly5[8] = { tx + 40,ty + 50, tx + 35,ty + 48, tx + 28,ty + 40, tx + 28,ty + 35 };
		fillpoly(4, poly5);
	}
	void destroi() {
		for (int i = 0; i < objects->size(); i++) {
			if (objects[0][i]->getTeam() == team) {
				delete objects[0][i];
				objects[0][i] = objects[0][objects->size() - 1];
				objects->pop_back();
				i--;
			}
		}
		stan = 0;
	}
	int getTeam() {
		return team;
	}
};
class Pole {
public:
	int x, y;
	Pole(int ny, int nx) {
		x = nx;
		y = ny;
	}
};

class AI :public tank {
public:
	string path1, path2, dodgePath;
	queue<Pole> Q;
	int counter;//liczy klatki do aktualizacji trasy
	int coliderx, colidery;
	bool tour = false, dod = false;
	vector<Element*> target;
	AI(string* mt, vector<Element*>* elements, int spawnX, int spawnY, int pNumber) : tank(mt, elements, spawnX, spawnY, 3, pNumber) {
		counter = 0;
		dodgePath = "";
	}
	void getEnemies() {
		target.clear();
		for (int i = 0; i < objects->size(); i++) {
			if (objects[0][i]->getTeam() != team && objects[0][i]->getTeam() != 0) {
				target.push_back(objects[0][i]);
			}
		}
	}
	//znajdowanie drogi
	void pathFinding() {
		getEnemies();
		Pole s(y, x);
		int drogi[maxy][maxx];
		for (int j = 0; j < maxy; j++) {
			for (int k = 0; k < maxx; k++) {
				drogi[j][k] = 100;
			}
		}
		drogi[y][x] = 1;
		Q.push(s);

		while (!Q.empty()) {
			int ax = Q.front().x;
			int ay = Q.front().y;
			counter = drogi[ay][ax] + 1;
			int tx = 0;
			int ty = 0;
			int tmp;

			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					if (i == 0) {
						tx = ax + 1 + (-2) * j;
						ty = ay;
					}
					else {
						tx = ax;
						ty = ay + 1 + (-2) * j;
					}
					if (mapa[ty][tx] == ' ' && drogi[ty][tx] > counter) {
						Pole n(ty, tx);
						Q.push(n);
						drogi[ty][tx] = counter;
					}
					if (mapa[ty][tx] == 'H' && drogi[ty][tx] > counter + 3) {
						Pole n(ty, tx);
						Q.push(n);
						drogi[ty][tx] = counter + 3;
					}
				}
			}
			Q.pop();
		}

		path2 = "";
		for (int i = 0; i < target.size(); i++) {
			path1 = "";
			int ax = target[i]->x;
			int ay = target[i]->y;
			while (drogi[ay][ax] != 1) {
				int control = drogi[ay][ax];
				if (drogi[ay][ax] > drogi[ay + 1][ax]) {
					path1 += "uu";
					ay++;
				}
				if (drogi[ay][ax] > drogi[ay - 1][ax]) {
					path1 += "dd";
					ay--;
				}
				if (drogi[ay][ax] > drogi[ay][ax + 1]) {
					path1 += "ll";
					ax++;
				}
				if (drogi[ay][ax] > drogi[ay][ax - 1]) {
					path1 += "rr";
					ax--;
				}
			}
			if (path1.size() < path2.size() || i == 0) {
				path2 = path1;
			}
		}

		int tempx = x;
		int tempy = y;
		coliderx = 0;
		colidery = 0;
		bool brek = false;
		path1 = path2;
		while (path1.size() > 1 && brek == false) {
			char temp = path1[path1.size() - 1];
			tempx += temp == 'l' ? -1 : (temp == 'r' ? 1 : 0);
			tempy += temp == 'u' ? -1 : (temp == 'd' ? 1 : 0);
			if (mapa[tempy][tempx] == 'H') {
				coliderx = tempx;
				colidery = tempy;
				brek = true;
			}
			path1.pop_back();
			path1.pop_back();
		}
	}
	void aimShoot(char a) {
		if (a == orient) {
			shoot();
		}
		else {
			moving(a);
		}
		tour = true;
	}
	void clearShot(char a, int targetX, int targetY) {
		int i, j, k, l;
		i = min(y, targetY);
		j = max(y, targetY);
		k = min(x, targetX);
		l = max(x, targetX);
		char z = ' ';
		for (; i <= j; i++) {
			for (; k <= l; k++) {
				if (mapa[i][k] == '#') {
					z = '#';
				}
			}
		}
		if (z == ' ') {
			aimShoot(a);
		}
	}
	void dodge() {
		if (dodgePath.size() == 0) {
			char keys[4]{ 'u', 'd', 'l', 'r' };
			srand(time(0));
			int a = rand() % 4;
			dodgePath += keys[a];
			dodgePath += keys[a];
			a = rand() % 4;
			dodgePath += keys[a];
			dodgePath += keys[a];
		}
		dod = dod == false ? true : false;
		tour = true;
		moving(dodgePath[dodgePath.size() - 1]);
		dodgePath.pop_back();
	}
	void update() {
		if (poc && poc->getStatus() == 1) {
			poc->destroi();
			poc = NULL;
		}
		tour = false;
		counter--;
		int distance = path2.length();
		if (counter < 1 && coldown == 4) {
			pathFinding();
			counter = 4;
		}
		if (!poc && dod == false) {
			//jezeli na drodze jest przeszkoda
			if (coliderx != 0 || colidery != 0) {
				char ta = ' ';
				if (coliderx == x) {
					ta = colidery < y ? 'u' : 'd';
				}
				if (colidery == y) {
					ta = coliderx < x ? 'l' : 'r';
				}
				if (ta != ' ') {
					aimShoot(ta);
				}
			}
			//jezeli nie ma przeszkody, sprawdz czy wrog jest w zasiegu
			else {
				//szansa na unik
				int a = rand() % 4;
				if (a == 0) {
					dodge();
				}
				//szansa na strzal
				else {
					for (int i = 0; i < target.size(); i++) {
						if (tour == false) {
							char ta = ' ';
							int tx, ty, j;
							tx = target[i]->x;
							ty = target[i]->y;
							if (tx == x) {
								ta = ty < y ? 'u' : 'd';
								j = x;
							}
							if (ty == y) {
								ta = tx < x ? 'l' : 'r';
							}
							if (ta != ' ') {
								clearShot(ta, tx, ty);
								//aimShoot(ta);
								//tour=true;
							}
						}
					}
				}
			}
			//jezeli nie ma w co strzelic to idz
			if (distance > 3 && tour == false) {
				moving(path2[distance - 1]);
				path2.pop_back();
			}
		}
		//jak nie ma zaladowanego pocisku to unik
		else {
			dodge();
		}
		draw();
	}
};


int main() {

	ifstream plik;
	plik.open("tank.txt");
	if (!plik) {
		cout << "blad otwierania pliku mapy. plik powinien nazwyac sie tank.txt";
		getchar();
		return -1;
	}
	else {
		cout << "mapa zaladowana poprawnie\n";
	}
	int y, x;
	plik >> y;
	plik >> x;
	string t[y];
	getline(plik, t[0]);
	for (int i = 0; i < y; i++) {
		getline(plik, t[i]);
	}
	plik.close();

	for (int i = 0; i < y; i++) {
		for (int k = 0; k < x; k++) {
			cout << t[i][k];
		}
		cout << endl;
	}

	char start = 'n';
	int r[4] = { 0,0,0,0 };
	while (start != 't') {
		cout << "\nPozycja\tDruzyna\tRodzaj\n1\t1\t0\n2\t1\t0\n3\t2\t0\n4\t2\t0\nRodzaj: (0)Puste (1)Gracz1 (2)Gracz2 (3)Komputer\n";
		for (int i = 0;i < 4;i++) {
			cout << "Pozycja" << i + 1 << ": ";
			cin >> r[i];
		}
		cout << "\nPozycja\tDruzyna\tRodzaj\n";
		for (int i = 0;i < 4;i++) {
			cout << i + 1 << "\t";
			if (i > 1) {
				cout << 2;
			}
			else {
				cout << 1;
			}
			if (r[i] == 1) {
				cout << "\tGracz1\n";
			}
			if (r[i] == 2) {
				cout << "\tGracz2\n";
			}
			if (r[i] == 3) {
				cout << "\tKomputer\n";
			}
			if (r[i] != 1 && r[i] != 2 && r[i] != 3) {
				cout << "\tPuste\n";
			}
		}
		cout << "Zgadza sie? (t)(n) ";
		cin >> start;
	}

	//inicjacja objektow
	vector<Element*> objects;
	for (int k = 0; k < y; k++) {
		for (int l = 0; l < x; l++) {
			char temp = t[k][l];
			int itemp = temp - '0';
			if (temp == 'o') {
				objects.push_back(new Orzel(&objects, l, k, 1));
			}
			if (temp == 'O') {
				objects.push_back(new Orzel(&objects, l, k, 2));
			}
			if (itemp < 5 && itemp >0) {
				if (r[itemp - 1] == 3) {
					objects.push_back(new AI(t, &objects, l, k, itemp));
				}
				if (r[itemp - 1] == 2 || r[itemp - 1] == 1) {
					objects.push_back(new tank(t, &objects, l, k, r[itemp - 1], itemp));
				}
				t[k][l] = ' ';
			}
		}
	}

	int win1 = 3;
	int win2 = 3;
	initwindow(x * 50 + 50, y * 50 + 100);
	int page = 0;
	//glowna petla
	while (!(GetKeyState(VK_ESCAPE) & 0x8000) && win1 > 1 && win2 > 1) {
		win1 = 0;
		win2 = 0;
		setactivepage(page);
		setvisualpage(1 - page);
		cleardevice();

		for (int i = 0; i < objects.size(); i++) {
			//usuwanie obiektu
			if (objects[i]->getStatus() == 0) {
				delete objects[i];
				objects[i] = objects[objects.size() - 1];
				objects.pop_back();
				i--;
			}
			else {
				objects[i]->update();
				if (objects[i]->getTeam() == 1) {
					win1++;
				}
				if (objects[i]->getTeam() == 2) {
					win2++;
				}
			}
		}
		for (int i = 0; i < y; i++) {
			for (int k = 0; k < x; k++) {
				char temp = t[i][k];
				if (temp == '#') {
					blok(k, i);
				}
				if (temp == 'H') {
					cegla(k, i);
				}
			}
		}
		delay(100);
	}
	closegraph();
	cout << "Wygrala druzyna: ";
	if (win1 > 1) {
		cout << "1";
	}
	if (win2 > 1) {
		cout << "2";
	}
	cout << "\n\nexit (e)";
	while (start != 'e') {
		cin >> start;
	}
	return 0;
}
