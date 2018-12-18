#include "base.h"
#include "card.h"
#include "game.h"

int main(int argc,char** argv){
  Player players[3]={{},{},{}};
  byte cards[ARRAY_SIZE(All_Cards)];
  memcpy(cards,All_Cards,sizeof(All_Cards));
  shuffle(cards,54);

  for(int i=0,count=0; i<17; ++i){
    for(int p=0; p<3; ++p){
      players[p].cards[i]=cards[count++];
    }
  }
  byte super_card=cards[rand()%52];
  int val=get_value(super_card)-1;
  DEBUG("super card:%c\n",Card_Labels[val]);
  
  for(int p=0; p<3; ++p){
    printf("%lu#",ARRAY_SIZE(players[p].cards));
    sort(players[p].cards,ARRAY_SIZE(players[p].cards));
    for(int i=0; i<17; ++i){
      char c=p<2? '*':Card_Labels[(players[p].cards[i]&0x0F)-1];
      DEBUG("%c",c);
    }
    DEBUG("\n");
  }
  ComboAnalysis analy;
  char input[128];
  while(1){
    memset(input,0,sizeof(input));
    scanf("%s",input);
    sort((byte*)input, strlen(input));

    analyse_combo((byte*)input, strlen(input), &super_card, 1, analy);
    int type=get_type(&analy);
    DEBUG("single:%d,pair:%d,three:%d,four:%d,straight:%d\n",
      analy.combo.single,analy.combo.pair,analy.combo.three,analy.combo.four,analy.combo.straight);
    DEBUG("input:%d,max:%d,min:%d,dif:%d,type:%d\n", 
      analy.count,analy.equal_max,analy.equal_min,analy.unequal,type);
  }
  
  return 0;
}
