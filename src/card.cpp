#include "card.h"

void shuffle(byte* arr,int len){
	srand((unsigned)time(NULL));
	for(int i=len-1; i>0; --i){
		int p=rand()%(i+1);
		byte tmp=arr[i];
		arr[i]=arr[p];
		arr[p]=tmp;
	}
}

bool has_supercard(const byte* arr, int len, const byte* super, int super_len){
	for(int i=0;i<super_len;++i){
		for(int j=0;j<len;++j){
			if(get_value(super[i]) == get_value(arr[j]))
				return true;
		}
	}
	return false;
}

bool is_straight(const byte* arr, int len){
	bool result=false;
	for(int i=len-1; i>0; --i){
		result=abs(get_value(arr[i])-get_value(arr[i-1])) == 1;
		if(!result)break;
	}
	return result;
}

bool has_jokers(const byte* arr, int len){
	int joker=0;
	for(int i=0; i<len; ++i){
		joker+=(arr[i]&0xF0)==0x40? 1:0;
	}
	return joker==2;
}

void analyse_combo(const byte* arr,int len, ComboAnalysis& analy){
	ComboAnalysis::Counter counter[len];int counter_len=0;
	for(int i=0,j=0; i<len; ++i){
		do{
			if(j>=counter_len){
				counter[j].val=arr[i];
				counter[j].num=1;
				++counter_len;
				break;
			}
			if((arr[i]&0x0F) == (counter[j].val&0x0F)){
				++counter[j].num;
				break;
			}
			++j;
		}while(1);
		j=0;
	}
	ComboAnalysis tmp_analy={0,0,0,0,{0,0,0,0,false}};
	tmp_analy.count=len;
	tmp_analy.unequal=counter_len;
	tmp_analy.equal_max=len>0? 1:0;
	tmp_analy.equal_min=len;
	for(int i=0; i<counter_len; ++i){
		if(counter[i].num==1)tmp_analy.combo.single++;
		if(counter[i].num==2)tmp_analy.combo.pair++;
		if(counter[i].num==3)tmp_analy.combo.three++;
		if(counter[i].num==4)tmp_analy.combo.four++;
		tmp_analy.equal_max=counter[i].num>=tmp_analy.equal_max? counter[i].num:tmp_analy.equal_max;
		tmp_analy.equal_min=counter[i].num<=tmp_analy.equal_min? counter[i].num:tmp_analy.equal_min;
	}
	byte tmp_arr[len];int tmp_arr_len=0;
	for(int i=0; i<len; ++i){
		if(counter[i].num == tmp_analy.equal_max){
			tmp_arr[tmp_arr_len++]=counter[i].val;
		}
	}
	sort(tmp_arr, tmp_arr_len);
	tmp_analy.combo.straight=is_straight(tmp_arr, tmp_arr_len);
	tmp_analy.combo.jokers=has_jokers(arr, len);
	memcpy(&analy, &tmp_analy, sizeof(ComboAnalysis));
}

bool arr_contains(byte one, const byte* arr, int len){
	for(int i=0;i<len;++i){
		if(get_value(one) == arr[i])
			return true;
	}
	return false;
}	

void analyse_combo(const byte* arr,int len, const byte* super, int super_len, ComboAnalysis& analy){
	ComboAnalysis::Counter counter[len];int counter_len=0;
	ComboAnalysis tmp_analy={0,0,0,0,{0,0,0,0,false}};
	for(int i=0,j=0; i<len; ++i){
		do{
			if(arr_contains(arr[i], super, super_len)){
				tmp_analy.combo.super++;
				break;
			}
			if(j>=counter_len){
				counter[j].val=arr[i];
				counter[j].num=1;
				++counter_len;
				break;
			}
			if((arr[i]&0x0F) == (counter[j].val&0x0F)){
				++counter[j].num;
				break;
			}
			++j;
		}while(1);
		j=0;
	}
	tmp_analy.count=len;
	tmp_analy.unequal=counter_len;
	tmp_analy.equal_max=len>0? 1:0;
	tmp_analy.equal_min=len;
	for(int i=0; i<counter_len; ++i){
		if(counter[i].num==1)tmp_analy.combo.single++;
		if(counter[i].num==2)tmp_analy.combo.pair++;
		if(counter[i].num==3)tmp_analy.combo.three++;
		if(counter[i].num==4)tmp_analy.combo.four++;
		tmp_analy.equal_max=counter[i].num>=tmp_analy.equal_max? counter[i].num:tmp_analy.equal_max;
		tmp_analy.equal_min=counter[i].num<=tmp_analy.equal_min? counter[i].num:tmp_analy.equal_min;
	}
	byte tmp_arr[len];int tmp_arr_len=0;
	for(int i=0; i<len; ++i){
		if(counter[i].num == tmp_analy.equal_max){
			tmp_arr[tmp_arr_len++]=counter[i].val;
		}
	}
	sort(tmp_arr, tmp_arr_len);
	tmp_analy.combo.straight=is_straight(tmp_arr, tmp_arr_len);
	tmp_analy.combo.jokers=has_jokers(arr, len);
	memcpy(&analy, &tmp_analy, sizeof(ComboAnalysis));
}

/*
len min	max	dif
1	1	1	1	a
2	2	2	1	aa
2	1	1	2	NM
3	3	3	1	aaa
4	1	3	2	aaab
4	4	4	1	aaaa
5	1	1	5	abcde
5	2	3	2	aaabb
6	1	4	3	aaaabc
6	2	2	3	aabbcc
6	3	3	2	aaabbb
8	2	4	3	aaaabbcc
8	1	3	4	aaabbbcd
10	2	3	4	aaabbbccdd
*/

// bool match_analyse(ComboAnalysis result, int count, int min, int max, int unequal,bool straight){
// 	return result.count==count 
// 		&& result.equal_min==min 
// 		&& result.equal_max==max 
// 		&& result.unequal==unequal 
// 		&& result.combo.straight==straight;
// }

// int get_type(const byte* arr, int len, const ComboAnalysis result){
// 	if(len==0)return CardType_Check;
// 	if(match_analyse(result,1,1,1,1,false))return CardType_Single;
// 	if(match_analyse(result,2,2,2,1,false))return CardType_Pair;
// 	if(match_analyse(result,2,1,1,2,false))return CardType_Rocket;
// 	if(match_analyse(result,3,3,3,1,false))return CardType_Three;
// 	if(match_analyse(result,4,1,3,2,false))return CardType_Three_Attach_Single;
// 	if(match_analyse(result,4,4,4,1,false))return CardType_Bomb;
// 	if(match_analyse(result,5,1,1,5,true))return CardType_Straight;
// 	if(match_analyse(result,5,2,3,2,false))return CardType_Three_Attach_Pair;
// 	if(match_analyse(result,6,1,4,3,false))return CardType_Four_Attach_Single;
// 	if(match_analyse(result,6,2,2,3,true))return CardType_Straight_Pair;
// 	if(match_analyse(result,6,3,3,2,true))return CardType_Straight_Three;
// 	if(match_analyse(result,8,2,4,3,false))return CardType_Four_Attach_Pair;
// 	if(match_analyse(result,8,1,3,4,true))return CardType_Straight_Three_Attach_Single;
// 	if(match_analyse(result,10,2,3,4,true))return CardType_Straight_Three_Attach_Pair;
// 	return CardType_Check;
// }


void upgrade(ComboAnalysis* analy){
	
}

int get_type(const ComboAnalysis* analy){
	if(analy->count==0)return CardType_Check;
	if(analy->combo.equal(1,0,0,0))return CardType_Single;
	if(analy->combo.equal(0,1,0,0))return CardType_Pair;
	if(analy->combo.equal(2,0,0,0) && analy->combo.jokers)return CardType_Rocket;
	if(analy->combo.equal(0,0,1,0))return CardType_Three;
	if(analy->combo.equal(1,0,1,0))return CardType_Three_Attach_Single;
	if(analy->combo.equal(0,1,1,0))return CardType_Three_Attach_Pair;
	if(analy->combo.equal(0,0,0,1))return CardType_Bomb;
	if(analy->combo.equal(2,0,0,1))return CardType_Four_Attach_Single;
	if(analy->combo.equal(0,2,0,1))return CardType_Four_Attach_Pair;
	if(analy->combo.more(5,0,0,0) && analy->combo.straight)return CardType_Straight;
	if(analy->combo.more(0,3,0,0) && analy->combo.straight)return CardType_Straight_Pair;
	if(analy->combo.more(0,0,2,0) && analy->combo.straight)return CardType_Straight_Three;
	if(analy->combo.more(2,0,2,0) && analy->combo.straight && analy->combo.attach())return CardType_Straight_Three_Attach_Single;
	if(analy->combo.more(0,2,2,0) && analy->combo.straight && analy->combo.attach())return CardType_Straight_Three_Attach_Pair;
	return CardType_Check;
}

void check_combo(const ComboAnalysis* analy, CardCombo& combo){
	CardCombo tmp_combo={0,0,0,0,0,0};
	do{
		if(analy->count==0)break;
		if(analy->combo.equal(1,0,0,0)){
			tmp_combo.base_type=CardCombo::BaseType_Single;
			break;
		}
		if(analy->combo.equal(0,1,0,0)){
			tmp_combo.base_type=CardCombo::BaseType_Pair;
			break;
		}
		if(analy->combo.equal(2,0,0,0) && analy->combo.jokers){
			tmp_combo.base_type=CardCombo::BaseType_Rocket;
			break;
		}
		if(analy->combo.equal(0,0,1,0)){
			tmp_combo.base_type=CardCombo::BaseType_Three;
			break;
		}
		if(analy->combo.equal(1,0,1,0)){
			tmp_combo.base_type=CardCombo::BaseType_Three;
			tmp_combo.attach_type=CardCombo::AttachType_Single;
			break;
		}
		if(analy->combo.equal(0,1,1,0)){
			tmp_combo.base_type=CardCombo::BaseType_Three;
			tmp_combo.attach_type=CardCombo::AttachType_Pair;
			break;
		}
		if(analy->combo.equal(2,0,0,1)){
			tmp_combo.base_type=CardCombo::BaseType_Four;
			tmp_combo.attach_type=CardCombo::AttachType_Single;
			break;
		}
		if(analy->combo.equal(0,2,0,1)){
			tmp_combo.base_type=CardCombo::BaseType_Four;
			tmp_combo.attach_type=CardCombo::AttachType_Pair;
			break;
		}
		if(analy->combo.more(5,0,0,0) && analy->combo.straight){
			tmp_combo.base_type=CardCombo::BaseType_Straight;
			break;
		}
		if(analy->combo.more(0,3,0,0) && analy->combo.straight){
			tmp_combo.base_type=CardCombo::BaseType_Straight;
			tmp_combo.straight_type=CardCombo::StraightType_Pair;
			break;
		}
		if(analy->combo.more(0,0,2,0) && analy->combo.straight){
			tmp_combo.base_type=CardCombo::BaseType_Straight;
			tmp_combo.straight_type=CardCombo::StraightType_Three;
			break;
		}
		if(analy->combo.more(2,0,2,0) && analy->combo.straight && analy->combo.attach()){
			tmp_combo.base_type=CardCombo::BaseType_Straight;
			tmp_combo.straight_type=CardCombo::StraightType_Three;
			tmp_combo.attach_type=CardCombo::AttachType_Single;
			break;
		}
		if(analy->combo.more(0,2,2,0) && analy->combo.straight && analy->combo.attach()){
			tmp_combo.base_type=CardCombo::BaseType_Straight;
			tmp_combo.straight_type=CardCombo::StraightType_Three;
			tmp_combo.attach_type=CardCombo::AttachType_Pair;
			break;
		}
	}while(0);
	memcpy(&combo, &tmp_combo, sizeof(combo));
}


byte get_value(byte card){
    byte val=card&0x0F;
    val=(card&0xF0)==0x40? val+2:val;
    val=val<=2? val+13:val;
    return val;
}

//O(n^2)
void sort(byte* arr,int len){
  for(int i=1; i<len; ++i){
    if(get_value(arr[i]) < get_value(arr[i-1])){
	int j=i-1;
	byte tmp=arr[i];
	arr[i]=arr[i-1];
	while(get_value(tmp) < get_value(arr[j])){
          arr[j+1]=arr[j];
          --j;
          if(j<0)break;
        }
        arr[j+1]=tmp;
    }
  }
}

bool contains_value(byte one,byte* arr,int len){
  for(int i=0; i<len; ++i){
    if(arr[i] == 0xFF)return false;
    if((arr[i]&0x0F) == (one&0x0F))return true;
  }
  return true;
}