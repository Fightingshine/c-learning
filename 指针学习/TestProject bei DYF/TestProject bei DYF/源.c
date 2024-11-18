#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int N = 1000;
#define starten 1002
#define enden 1003
typedef struct chain_blocks {
	int last;
	int next;
	int num;
	int stor;
}chainarr;
chainarr Point_das;
chainarr test[1010];
chainarr letzt(chainarr a) {
	return test[a.last];
}
chainarr naechst(chainarr a) {
	return test[a.next];
}
void rechain(chainarr a,chainarr b,chainarr c) {
	int soll[5];
	soll[0] = c.next;
	soll[1] = c.last;
	soll[2] = c.num;
	soll[3] = a.num;
	soll[4] = b.num;
	test[c.last].next = soll[0];
	test[c.next].last = soll[1];
	test[a.num].next = soll[2];
	test[b.num].last = soll[2];
	test[c.num].last = soll[3];
	test[c.num].next = soll[4];
}
void init() {
	for (int i = 0; i < N;i++) {
		test[i].last = i - 1;
		test[i].next = i + 1;
		test[i].stor = rand();
		test[i].num = i;
	}
	test[0].last = starten;
	test[starten].next = 0;
	test[N - 1].next = enden;
	test[enden].last = N - 1;
	test[starten].num = starten;
	test[enden].num = enden;
}
int main() {
	srand((unsigned int)clock());
	init();
	for (int i = 0; i < N; i++) {
		Point_das = test[Point_das.next];
		printf("%d on %d,%d,%d,from %d,to %d\n", (Point_das.stor), (Point_das.num), test[i].stor, test[i].num, test[i].last, test[i].next);
	}
	for (int i = 1; i < N;i++) {
		Point_das = letzt(test[i]);
		for (int j = i; j >= 0;j--) {
			if (Point_das.stor<=test[i].stor) {
				if (Point_das.num == test[i].last) {
					break;
				}
				else rechain(test[Point_das.num],test[Point_das.next],test[i]);
			}
			else {
				if (Point_das.last == starten) {
					rechain(test[starten],naechst(test[starten]), test[i]);
				}
				else Point_das = letzt(Point_das);
			}
		}
	}
	test[312].next = 0;
	Point_das = test[starten];
	for (int i = 0; i < N;i++) {
		Point_das = naechst(Point_das);
		printf("%d\t%d\t%d\t%d\n",Point_das.stor, Point_das.num, Point_das.last, Point_das.next);
	}
	printf("%d", test[1002].next);
	system("pause");
	return 0;
}