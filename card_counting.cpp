#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;

struct Player {
	int money;	// 돈
	int win;	// 승리
	int draw;	// 무승부
	int lose;	// 패배
};


// 카드 뽑기
int Draw(vector<int>* deck) {

	int card;	

	// 덱의 맨 뒤의 값
	card = deck->back();

	// 덱의 맨 뒤 삭제
	deck->pop_back();

	// 뽑은 카드
	return card;
}


// 카드 셔플
void Shuffle(vector<int>* deck)
{
	// 덱 비우기
	deck->clear();

	// 6개의 팩 * 4개의 종류로 덱 채우기
	for (int i = 0; i < 6 * 4; i++) {
		// A부터 K까지
		for (int j = 0; j < 13; j++)
			deck->push_back(j + 1);
	}
	// 랜덤으로 덱을 셔플
	random_shuffle(deck->begin(), deck->end());
}


// 카드의 합
int Sum_Of_Num(int hand[21])
{
	int sum = 0;
	int A_cnt = 0;	// A의 갯수
	int A_sum = 0;
	bool is_bust = true;

	// 패 확인
	for (int i = 0; i < 21; i++) {

		// 패의 마지막
		if (hand[i] == 0)
			break;
		// A인 경우
		else if (hand[i] == 1)
			A_cnt++;
		// 10, J(11), Q(12), K(13)인 경우
		else if (hand[i] >= 10)
			sum += 10;
		// 2 부터 9 까지 합
		else
			sum += hand[i];
	}

	// A가 존재한다면 
	if (A_cnt) {

		// 이미 bust인 경우, A가 1 or 11 don't care
		if (sum > 21)
			sum += A_cnt;
		// A 한 장을 11로 사용하더라도 bust인 경우 무조건 A=1
		else if (sum + 11 > 21)
			sum += A_cnt;
		// A를 두 장 이상 11로 사용하는 경우는 없음 
		else{
			sum += 11;
			sum += A_cnt - 1;
		}

		return sum;
	}
	// A가 없는 경우
	else
		return sum;
}

// 덮여진 카드 예측
int Expect_Card(int card_cnt[14], int open_card) {

	// A ~ K
	int used_total = 0;
	int rest_total = 312;

	// 10, J, Q, K
	int used_ten = 0;
	int rest_ten = 96;

	// 덮여져있는 카드 예측
	int expect_card = 0;

	// 가장 높은 확률
	int max = 0;

	int card_num[11] = { 0, };	// 카드 숫자의 개수, 0번째는 사용 안함

	// 사용된 전체 카드
	for (int i = 1; i < 14; i++)
		used_total += card_cnt[i];

	// 사용된 10, J, Q, K
	for (int i = 10; i < 14; i++)
		used_ten += card_cnt[i];

	// 남은 카드
	rest_total -= used_total;
	rest_ten -= used_ten;

	// 남은 카드의 확률
	for (int i = 1; i < 11; i++) {

		// 10의 개수
		if (i == 10)
			card_num[i] = rest_ten;
		// 그 외
		else
			card_num[i] = 24 - card_cnt[i];
	}

	// 확률이 가장 높은 카드 찾기
	for (int i = 1; i < 11; i++)
		if (max < card_num[i]) {
			max = card_num[i];
			expect_card = i;
		}

	return expect_card;
}


// 카드 카운팅
double Card_Count(int card_cnt[14], int open_card, int hide_card, int target_card)
{
	// A ~ K
	int used_total = 0;
	int rest_total = 312;

	// 10, J, Q, K
	int used_ten = 0;
	int rest_ten = 96;

	int card_num[11] = { 0, };	// 카드 숫자의 개수, 0번째는 사용 안함

	int bust_cnt = 0;	// bust되는 카드의 갯수

	// 사용된 전체 카드
	for (int i = 1; i < 14; i++)
		used_total += card_cnt[i];

	// 사용된 10, J, Q, K
	for (int i = 10; i < 14; i++)
		used_ten += card_cnt[i];

	// 남은 카드
	rest_total -= used_total;
	rest_ten -= used_ten;

	// 남은 카드의 확률, 예상 카드 포함
	for (int i = 1; i < 11; i++) {

		// 10의 개수
		if (i == 10) {
			if (hide_card = i) {
				rest_total--;
				used_total++;
				rest_ten--;
				used_ten++;
			}
			card_num[i] += rest_ten;
		}
		// 그 외
		else {
			if (hide_card = i) {
				rest_total--;
				used_total++;
				card_num[i]--;
			}
			card_num[i] += 24 - card_cnt[i];
		}

	}

	for (int i = 1; i < 11; i++) {

		// bust되는 카드의 갯수
		if (target_card < i) {
			bust_cnt += card_num[i];
		}
	}

	// bust 될 확률
	return (double)bust_cnt / (double)rest_total;
}

// 카드 카운팅 베팅 금액용
double Card_Count(int card_cnt[14])
{
	// A ~ K
	int used_total = 0;
	int rest_total = 312;

	// 2~6
	// 딜러의 카드가 12~16일때 평균적으로 hit로 bust할 확률이 높음
	int used_two_six = 0;
	int rest_two_six = 120;

	// 사용된 전체 카드
	for (int i = 1; i < 14; i++) {
		if (i > 1)
			used_two_six += card_cnt[i];
		else if (i < 7)
			used_two_six += card_cnt[i];

		used_total += card_cnt[i];
	}

	// 남은 카드
	rest_total -= used_total;
	rest_two_six -= used_two_six;

	// 10의 경우는 평균적으로 많고, 2~6 카드가 많을수록 딜러가 bust할 확률이 높으므로 큰 금액 배팅
	return (double)rest_two_six / (double)rest_total;
}


// 베팅 금액 조정
int Betting(int card_cnt[14])
{
	return (1 + Card_Count(card_cnt)) * 100;
}


// player1 전용, hit or stand 확인
bool Hit_Status_Player(int hand[21], int card_cnt[14], int open_card)
{
	int hide_card = Expect_Card(card_cnt, open_card);	// 딜러의 덮여진 카드 예측

	// 예상 딜러 카드
	int expect_dealer[21] = { 0, };

	expect_dealer[0] = hide_card;
	expect_dealer[1] = open_card;

	int p_sum = Sum_Of_Num(hand);	// player의 패의 합
	int d_sum = Sum_Of_Num(expect_dealer);	// dealer의 패의 합

	// bust이거나 black jack인경우
	if (p_sum >= 21)
		return false;
	else if(d_sum > 21)
		return false;

	int p_target_card = 21 - p_sum; // bust까지의 player target 카드
	int d_target_card = 21 - d_sum; // bust까지의 dealer target 카드

	double p_bust = Card_Count(card_cnt, open_card, hide_card, p_target_card);	// player의 버스트될 확률
	double d_bust = Card_Count(card_cnt, open_card, hide_card, d_target_card);	// delaer의 버스트될 확률

	//cout << p_bust << " " << d_bust << endl;

	// player의 패가 11 이하인 경우
	if (p_sum <= 11) {
		return true; // 항상 hit
	}
	// player의 bust확률이 낮으면 hit하는것이 유리
	else if(p_bust <= 0.5) {
		return true;
	}
	// dealer의 bust확률이 높으면 stand가 유리
	else if (d_bust >= 0.5) {
		return false;
	}
	// 그 외에 일반적인 경우
	// dealer의 패가 17미만인 경우
	else if (d_sum < 17) {

			// dealer의 bust 확률이 높고 player bust확률이 낮은 경우
			if ((d_bust > 0.5) & (p_bust < 0.5)) {
				return true;	 // hit
			}
			// dealer의 bust확률이 50% 보다 큰 경우
			else if (d_bust >= 0.5)
				return false;	// stand
			// dealer가 bust확률이 높진 않지만 player의 bust확률이 낮은 경우
			else if(p_bust < 0.5)
				return true;	// hit
			// dealer의 bust확률이 낮고, player의 bust확률이 높은경우
			else 
				return false;	// stand 
	}
	// 17 이상인 경우
	else if(d_sum >= 17)
	{
		// 더 높은 승률을 위해 무조건 hit해야하는 경우
		if (p_sum <= d_sum)
			return true;	// hit
		else
			return false;	// stand
		
	}

}
// dealer, player2 전용, hit or stand 확인
bool Hit_Status_Dealer(int hand[21])
{
	if (Sum_Of_Num(hand) >= 17)
		return false;	//stand
	else
		return true;	//hit
}


// 게임 플레이
void Play_Game(int play_cnt, struct Player* player1, struct Player* player2)
{
	vector<int> deck;	// 덱

	int card_cnt[14] = { 0, };	// A~K의 사용된 개수, 0번은 사용하지 않음

	// 카드를 가장 많이 받는 경우 1이 21장이므로 array size 21
	int p1[21] = { 0, };	// player1의 패
	int p2[21] = { 0, };	// player2의 패
	int dealer[21] = { 0, };	// dealer의 패

	int hide_card;	//  dealer의 숨은 카드

	// 패의 합
	int p1_sum;
	int p2_sum;
	int dealer_sum;

	// 플레이 수 만큼 게임
	for (int i = 0; i < play_cnt; i++) {

		// 배팅 금액 셋팅
		int p1_betting = 100;
		int p2_betting = 100;

		// bust flag 셋팅
		bool p1_is_bust = false;
		bool p2_is_bust = false;

		// black jack flag 셋팅
		bool p1_black_jack = false;
		bool p2_black_jack = false;

		// 패 리셋
		for (int j = 0; j < 21; j++) {
			p1[j] = 0;
			p2[j] = 0;
			dealer[j] = 0;
		}
	
		// 남은 덱이 20%보다 작다면 셔플
		if (deck.size() < 312 * 0.2) {

			// 셔플
			Shuffle(&deck);

			// 사용된 카드 개수 0으로 초기화
			for (int j = 1; j < 14; j++) {
				card_cnt[j] = 0;
			}
		}

		// 플레이어 베팅 금액 설정
		p1_betting = Betting(card_cnt);
		//cout << p1_betting << endl;

		// dealer -> p1 -> p2 -> dealer -> p1 -> p2 순으로 카드 분배

		// 첫 카드 뽑기
		hide_card = Draw(&deck);	// 딜러의 첫 카드는 볼 수 없음
		dealer[0] = hide_card;

		p1[0] = Draw(&deck);
		card_cnt[p1[0]]++;	

		p2[0] = Draw(&deck);
		card_cnt[p2[0]]++;

		// 두번째 카드 뽑기
		dealer[1] = Draw(&deck);
		card_cnt[dealer[1]]++;

		p1[1] = Draw(&deck);
		card_cnt[p1[1]]++;

		p2[1] = Draw(&deck);
		card_cnt[p2[1]]++;

		// black jack 확인
		if (Sum_Of_Num(p1) == 21) {
			p1_black_jack = true;
			//cout << "p1 블랙잭!" << endl;
		}

		if (Sum_Of_Num(p2) == 21) {
			p2_black_jack = true;
		//	cout << "p2 블랙잭!" << endl;
		}

		// player turn

		// p1의 hit or stand
		for (int j = 2; j < 21; j++) {

			// 블랙잭인 경우
			if (p1_black_jack)
				break;

			// hit status 확인
			if (Hit_Status_Player(p1, card_cnt, dealer[1])) {

				// hit
				p1[j] = Draw(&deck);
				card_cnt[p1[j]]++;
			}
			// stand
			else 
				break;
		}

		// p2의 hit or stand
		for (int j = 2; j < 21; j++) {

			// 블랙잭인 경우
			if (p2_black_jack)
				break;

			// hit status 확인
			if (Hit_Status_Dealer(p2)) { 

				// hit
				p2[j] = Draw(&deck);
				card_cnt[p2[j]]++;
			}
			// stand
			else
				break;
		}

		// dealer turn

		// dealer의 카드 오픈
		card_cnt[hide_card]++;
		hide_card = 0;

		// dealer의 카드 카운트
		for (int j = 2; j < 21; j++) {

			// hit status 확인
			if (Hit_Status_Dealer(dealer)) { 

				// hit
				dealer[j] = Draw(&deck);
				card_cnt[dealer[j]]++;
			}
			// stand
			else 
				break;
		}

		// bust 확인
		if ((p1_sum = Sum_Of_Num(p1)) > 21)
			p1_is_bust = true;

		if ((p2_sum = Sum_Of_Num(p2)) > 21)
			p2_is_bust = true;
		
		
		// 승패 결정

		// dealer가 bust인 경우
		if ((dealer_sum = Sum_Of_Num(dealer)) > 21) {

			// p1이 bust가 아니라면 승리
			if (!p1_is_bust) {

				// 블랙잭인 경우
				if (p1_black_jack)
					p1_betting *= 1.5;
				player1->win++;
				player1->money += p1_betting;
			}
			// p1이 bust인 경우 패배
			else {
				player1->lose++;
				player1->money -= p1_betting;
			}

			// p2가 bust가 아니라면 승리
			if (!p2_is_bust) {

				// 블랙잭인 경우
				if (p2_black_jack)
					p2_betting *= 1.5;
				player2->win++;
				player2->money += p2_betting;
			}
			// p2가 bust인 경우 패배
			else {
				player2->lose++;
				player2->money -= p2_betting;
			}
		}
		// dealer가 bust가 아닌 경우
		else {

			// p1이 bust가 아닌 경우에서
			if (!p1_is_bust) {

				// 승리
				if (dealer_sum < p1_sum) {

					// 블랙잭인 경우
					if (p1_black_jack) 
						p1_betting *= 1.5;
					player1->money += p1_betting;
					player1->win++;
				}
				// 패배
				else if (dealer_sum > p1_sum) {
					player1->money -= p1_betting;
					player1->lose++;
				}
				// 무승부
				else 
					player1->draw++;

			}	
			// p1이 bust인 경우 패배
			else {
				player1->lose++;
				player1->money -= p1_betting;
			}

			// p2가 bust가 아닌 경우에서
			if (!p2_is_bust) {

				// 승리
				if (dealer_sum < p2_sum) {
					
					// 블랙잭인 경우
					if (p2_black_jack)
						p2_betting *= 1.5;
					player2->money += p2_betting;
					player2->win++;
				}
				// 패배
				else if (dealer_sum > p2_sum) {
					player2->money -= p2_betting;
					player2->lose++;
				}
				// 무승부
				else
					player2->draw++;
			}	
			// p2가 bust인 경우 패배
			else {
				player2->lose++;
				player2->money -= p2_betting;
			}
		}

		//cout << player1->win << endl;
	}
}


int main()
{
	int play_cnt = 0;	// 플레이 수

	// 플레이 수 입력
	cout << "Play Game : ";
	cin >> play_cnt;

	// 랜덤 시드로 시간 사용
	srand((unsigned int)time(NULL));

	// 플레이어 세팅
	struct Player player1, player2;
	player1.money = 100000;
	player1.win = 0;
	player1.draw = 0;
	player1.lose = 0;

	player2.money = 100000;
	player2.win = 0;
	player2.draw = 0;
	player2.lose = 0;

	// 게임 시작
	Play_Game(play_cnt, &player1, &player2);	

	// 결과 출력
	cout << "-게임 결과-" << endl << endl;
	cout << "[Player1]" << endl
		<< "승리 : " << player1.win << "\t"
		<< "무승부 : " << player1.draw << "\t"
		<< "패배 : " << player1.lose << endl
		<< "승률 : " << (double)player1.win / (double)play_cnt * 100 << "%" << endl
		<< "남은 돈 : " << player1.money << endl;

	cout << endl << endl;

	cout << "[Player2]" << endl
		<< "승리 : " << player2.win << "\t"
		<< "무승부 : " << player2.draw << "\t"
		<< "패배 : " << player2.lose << endl
		<< "승률 : " << (double)player2.win / (double)play_cnt * 100 << "%" << endl
		<< "남은 돈 : " << player2.money << endl;

	return 0;
}