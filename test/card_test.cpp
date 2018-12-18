#include "card.h"
#include "game.h"

bool test(byte* arr, int len, int type){
	ComboAnalysis analy;
	sort(arr, len);
	analyse_combo(arr, len, NULL, 0, analy);
	return type==get_type(&analy);
}

void test_1(){
	byte arr[1]={All_Cards[rand()%54]};
	bool pass=test(arr, 1, CardType_Single);
	DEBUGPRO("%s\n",pass? "succeed!":"failed!");
}

void test_2(){
	byte val=All_Cards[rand()%52];
	byte arr[2]={val,val};
	bool pass=test(arr, 2, CardType_Pair);
	DEBUGPRO("%s\n",pass? "succeed!":"failed!");
}

void test_3(){
	byte val=All_Cards[rand()%52];
	byte arr[3]={val,val,val};
	bool pass=test(arr, 3, CardType_Three);
	DEBUGPRO("%s\n",pass? "succeed!":"failed!");
}

void test_4(){
	byte val=All_Cards[rand()%52];
	byte arr[4]={val,val,val,val};
	bool pass=test(arr, 4, CardType_Bomb);
	DEBUGPRO("%s\n",pass? "succeed!":"failed!");
}

void test_31(){
	byte val=All_Cards[rand()%12];
	byte attach=val+1;
	byte arr[4]={val,val,val,attach};
	bool pass=test(arr, 4, CardType_Three_Attach_Single);
	DEBUGPRO("%s\n",pass? "succeed!":"failed!");
}

void test_32(){
	byte val=All_Cards[rand()%12];
	byte attach=val+1;
	byte arr[5]={val,val,val,attach,attach};
	bool pass=test(arr, 5, CardType_Three_Attach_Pair);
	DEBUGPRO("%s\n",pass? "succeed!":"failed!");
}

void test_411(){
	byte val=All_Cards[rand()%11];
	byte attach1=val+1;
	byte attach2=val+2;
	byte arr[6]={val,val,val,val,attach1,attach2};
	bool pass=test(arr, 6, CardType_Four_Attach_Single);
	DEBUGPRO("%s\n",pass? "succeed!":"failed!");
}

void test_422(){
	byte val=All_Cards[rand()%11];
	byte attach1=val+1;
	byte attach2=val+2;
	byte arr[8]={val,val,val,val,attach1,attach1,attach2,attach2};
	bool pass=test(arr, 8, CardType_Four_Attach_Pair);
	DEBUGPRO("%s\n",pass? "succeed!":"failed!");
}

void test_12345(){
	byte val=All_Cards[rand()%6+2];
	byte arr[5]={val,val+1,val+2,val+3,val+4};
	bool pass=test(arr, 5, CardType_Straight);
	DEBUGPRO("%s\n",pass? "succeed!":"failed!");
}

void test_222(){
	byte val=All_Cards[rand()%8+2];
	byte arr[6]={val,val,val+1,val+1,val+2,val+2};
	bool pass=test(arr, 6, CardType_Straight_Pair);
	DEBUGPRO("%s\n",pass? "succeed!":"failed!");
}

void test_33(){
	byte val=All_Cards[rand()%10+2];
	byte arr[6]={val,val,val,val+1,val+1,val+1};
	bool pass=test(arr, 6, CardType_Straight_Three);
	DEBUGPRO("%s\n",pass? "succeed!":"failed!");
}

void test_3311(){
	byte val=All_Cards[rand()%9+2];
	byte arr[8]={val,val,val,val+1,val+1,val+1,val+2,val+3};
	bool pass=test(arr, 8, CardType_Straight_Three_Attach_Single);
	DEBUGPRO("%s\n",pass? "succeed!":"failed!");
}

void test_3322(){
	byte val=All_Cards[rand()%9+2];
	byte arr[10]={val,val,val,val+1,val+1,val+1,val+2,val+2,val+3,val+3};
	bool pass=test(arr, 10, CardType_Straight_Three_Attach_Pair);
	DEBUGPRO("%s\n",pass? "succeed!":"failed!");
}

void test_NM(){
	byte arr[2]={0x4E,0x4F};
	bool pass=test(arr, 2, CardType_Rocket);
	DEBUGPRO("%s\n",pass? "succeed!":"failed!");
}


int main(void){
	test_1();
	test_2();
	test_3();
	test_4();
	test_31();
	test_32();
	test_411();
	test_422();
	test_12345();
	test_222();
	test_33();
	test_222();
	test_33();
	test_3311();
	test_3322();
	test_NM();
	return 0;
}