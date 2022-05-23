#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;

struct Player {
	int money;	// ��
	int win;	// �¸�
	int draw;	// ���º�
	int lose;	// �й�
};


// ī�� �̱�
int Draw(vector<int>* deck) {

	int card;	

	// ���� �� ���� ��
	card = deck->back();

	// ���� �� �� ����
	deck->pop_back();

	// ���� ī��
	return card;
}


// ī�� ����
void Shuffle(vector<int>* deck)
{
	// �� ����
	deck->clear();

	// 6���� �� * 4���� ������ �� ä���
	for (int i = 0; i < 6 * 4; i++) {
		// A���� K����
		for (int j = 0; j < 13; j++)
			deck->push_back(j + 1);
	}
	// �������� ���� ����
	random_shuffle(deck->begin(), deck->end());
}


// ī���� ��
int Sum_Of_Num(int hand[21])
{
	int sum = 0;
	int A_cnt = 0;	// A�� ����
	int A_sum = 0;
	bool is_bust = true;

	// �� Ȯ��
	for (int i = 0; i < 21; i++) {

		// ���� ������
		if (hand[i] == 0)
			break;
		// A�� ���
		else if (hand[i] == 1)
			A_cnt++;
		// 10, J(11), Q(12), K(13)�� ���
		else if (hand[i] >= 10)
			sum += 10;
		// 2 ���� 9 ���� ��
		else
			sum += hand[i];
	}

	// A�� �����Ѵٸ� 
	if (A_cnt) {

		// �̹� bust�� ���, A�� 1 or 11 don't care
		if (sum > 21)
			sum += A_cnt;
		// A �� ���� 11�� ����ϴ��� bust�� ��� ������ A=1
		else if (sum + 11 > 21)
			sum += A_cnt;
		// A�� �� �� �̻� 11�� ����ϴ� ���� ���� 
		else{
			sum += 11;
			sum += A_cnt - 1;
		}

		return sum;
	}
	// A�� ���� ���
	else
		return sum;
}

// ������ ī�� ����
int Expect_Card(int card_cnt[14], int open_card) {

	// A ~ K
	int used_total = 0;
	int rest_total = 312;

	// 10, J, Q, K
	int used_ten = 0;
	int rest_ten = 96;

	// �������ִ� ī�� ����
	int expect_card = 0;

	// ���� ���� Ȯ��
	int max = 0;

	int card_num[11] = { 0, };	// ī�� ������ ����, 0��°�� ��� ����

	// ���� ��ü ī��
	for (int i = 1; i < 14; i++)
		used_total += card_cnt[i];

	// ���� 10, J, Q, K
	for (int i = 10; i < 14; i++)
		used_ten += card_cnt[i];

	// ���� ī��
	rest_total -= used_total;
	rest_ten -= used_ten;

	// ���� ī���� Ȯ��
	for (int i = 1; i < 11; i++) {

		// 10�� ����
		if (i == 10)
			card_num[i] = rest_ten;
		// �� ��
		else
			card_num[i] = 24 - card_cnt[i];
	}

	// Ȯ���� ���� ���� ī�� ã��
	for (int i = 1; i < 11; i++)
		if (max < card_num[i]) {
			max = card_num[i];
			expect_card = i;
		}

	return expect_card;
}


// ī�� ī����
double Card_Count(int card_cnt[14], int open_card, int hide_card, int target_card)
{
	// A ~ K
	int used_total = 0;
	int rest_total = 312;

	// 10, J, Q, K
	int used_ten = 0;
	int rest_ten = 96;

	int card_num[11] = { 0, };	// ī�� ������ ����, 0��°�� ��� ����

	int bust_cnt = 0;	// bust�Ǵ� ī���� ����

	// ���� ��ü ī��
	for (int i = 1; i < 14; i++)
		used_total += card_cnt[i];

	// ���� 10, J, Q, K
	for (int i = 10; i < 14; i++)
		used_ten += card_cnt[i];

	// ���� ī��
	rest_total -= used_total;
	rest_ten -= used_ten;

	// ���� ī���� Ȯ��, ���� ī�� ����
	for (int i = 1; i < 11; i++) {

		// 10�� ����
		if (i == 10) {
			if (hide_card = i) {
				rest_total--;
				used_total++;
				rest_ten--;
				used_ten++;
			}
			card_num[i] += rest_ten;
		}
		// �� ��
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

		// bust�Ǵ� ī���� ����
		if (target_card < i) {
			bust_cnt += card_num[i];
		}
	}

	// bust �� Ȯ��
	return (double)bust_cnt / (double)rest_total;
}

// ī�� ī���� ���� �ݾ׿�
double Card_Count(int card_cnt[14])
{
	// A ~ K
	int used_total = 0;
	int rest_total = 312;

	// 2~6
	// ������ ī�尡 12~16�϶� ��������� hit�� bust�� Ȯ���� ����
	int used_two_six = 0;
	int rest_two_six = 120;

	// ���� ��ü ī��
	for (int i = 1; i < 14; i++) {
		if (i > 1)
			used_two_six += card_cnt[i];
		else if (i < 7)
			used_two_six += card_cnt[i];

		used_total += card_cnt[i];
	}

	// ���� ī��
	rest_total -= used_total;
	rest_two_six -= used_two_six;

	// 10�� ���� ��������� ����, 2~6 ī�尡 �������� ������ bust�� Ȯ���� �����Ƿ� ū �ݾ� ����
	return (double)rest_two_six / (double)rest_total;
}


// ���� �ݾ� ����
int Betting(int card_cnt[14])
{
	return (1 + Card_Count(card_cnt)) * 100;
}


// player1 ����, hit or stand Ȯ��
bool Hit_Status_Player(int hand[21], int card_cnt[14], int open_card)
{
	int hide_card = Expect_Card(card_cnt, open_card);	// ������ ������ ī�� ����

	// ���� ���� ī��
	int expect_dealer[21] = { 0, };

	expect_dealer[0] = hide_card;
	expect_dealer[1] = open_card;

	int p_sum = Sum_Of_Num(hand);	// player�� ���� ��
	int d_sum = Sum_Of_Num(expect_dealer);	// dealer�� ���� ��

	// bust�̰ų� black jack�ΰ��
	if (p_sum >= 21)
		return false;
	else if(d_sum > 21)
		return false;

	int p_target_card = 21 - p_sum; // bust������ player target ī��
	int d_target_card = 21 - d_sum; // bust������ dealer target ī��

	double p_bust = Card_Count(card_cnt, open_card, hide_card, p_target_card);	// player�� ����Ʈ�� Ȯ��
	double d_bust = Card_Count(card_cnt, open_card, hide_card, d_target_card);	// delaer�� ����Ʈ�� Ȯ��

	//cout << p_bust << " " << d_bust << endl;

	// player�� �а� 11 ������ ���
	if (p_sum <= 11) {
		return true; // �׻� hit
	}
	// player�� bustȮ���� ������ hit�ϴ°��� ����
	else if(p_bust <= 0.5) {
		return true;
	}
	// dealer�� bustȮ���� ������ stand�� ����
	else if (d_bust >= 0.5) {
		return false;
	}
	// �� �ܿ� �Ϲ����� ���
	// dealer�� �а� 17�̸��� ���
	else if (d_sum < 17) {

			// dealer�� bust Ȯ���� ���� player bustȮ���� ���� ���
			if ((d_bust > 0.5) & (p_bust < 0.5)) {
				return true;	 // hit
			}
			// dealer�� bustȮ���� 50% ���� ū ���
			else if (d_bust >= 0.5)
				return false;	// stand
			// dealer�� bustȮ���� ���� ������ player�� bustȮ���� ���� ���
			else if(p_bust < 0.5)
				return true;	// hit
			// dealer�� bustȮ���� ����, player�� bustȮ���� �������
			else 
				return false;	// stand 
	}
	// 17 �̻��� ���
	else if(d_sum >= 17)
	{
		// �� ���� �·��� ���� ������ hit�ؾ��ϴ� ���
		if (p_sum <= d_sum)
			return true;	// hit
		else
			return false;	// stand
		
	}

}
// dealer, player2 ����, hit or stand Ȯ��
bool Hit_Status_Dealer(int hand[21])
{
	if (Sum_Of_Num(hand) >= 17)
		return false;	//stand
	else
		return true;	//hit
}


// ���� �÷���
void Play_Game(int play_cnt, struct Player* player1, struct Player* player2)
{
	vector<int> deck;	// ��

	int card_cnt[14] = { 0, };	// A~K�� ���� ����, 0���� ������� ����

	// ī�带 ���� ���� �޴� ��� 1�� 21���̹Ƿ� array size 21
	int p1[21] = { 0, };	// player1�� ��
	int p2[21] = { 0, };	// player2�� ��
	int dealer[21] = { 0, };	// dealer�� ��

	int hide_card;	//  dealer�� ���� ī��

	// ���� ��
	int p1_sum;
	int p2_sum;
	int dealer_sum;

	// �÷��� �� ��ŭ ����
	for (int i = 0; i < play_cnt; i++) {

		// ���� �ݾ� ����
		int p1_betting = 100;
		int p2_betting = 100;

		// bust flag ����
		bool p1_is_bust = false;
		bool p2_is_bust = false;

		// black jack flag ����
		bool p1_black_jack = false;
		bool p2_black_jack = false;

		// �� ����
		for (int j = 0; j < 21; j++) {
			p1[j] = 0;
			p2[j] = 0;
			dealer[j] = 0;
		}
	
		// ���� ���� 20%���� �۴ٸ� ����
		if (deck.size() < 312 * 0.2) {

			// ����
			Shuffle(&deck);

			// ���� ī�� ���� 0���� �ʱ�ȭ
			for (int j = 1; j < 14; j++) {
				card_cnt[j] = 0;
			}
		}

		// �÷��̾� ���� �ݾ� ����
		p1_betting = Betting(card_cnt);
		//cout << p1_betting << endl;

		// dealer -> p1 -> p2 -> dealer -> p1 -> p2 ������ ī�� �й�

		// ù ī�� �̱�
		hide_card = Draw(&deck);	// ������ ù ī��� �� �� ����
		dealer[0] = hide_card;

		p1[0] = Draw(&deck);
		card_cnt[p1[0]]++;	

		p2[0] = Draw(&deck);
		card_cnt[p2[0]]++;

		// �ι�° ī�� �̱�
		dealer[1] = Draw(&deck);
		card_cnt[dealer[1]]++;

		p1[1] = Draw(&deck);
		card_cnt[p1[1]]++;

		p2[1] = Draw(&deck);
		card_cnt[p2[1]]++;

		// black jack Ȯ��
		if (Sum_Of_Num(p1) == 21) {
			p1_black_jack = true;
			//cout << "p1 ����!" << endl;
		}

		if (Sum_Of_Num(p2) == 21) {
			p2_black_jack = true;
		//	cout << "p2 ����!" << endl;
		}

		// player turn

		// p1�� hit or stand
		for (int j = 2; j < 21; j++) {

			// ������ ���
			if (p1_black_jack)
				break;

			// hit status Ȯ��
			if (Hit_Status_Player(p1, card_cnt, dealer[1])) {

				// hit
				p1[j] = Draw(&deck);
				card_cnt[p1[j]]++;
			}
			// stand
			else 
				break;
		}

		// p2�� hit or stand
		for (int j = 2; j < 21; j++) {

			// ������ ���
			if (p2_black_jack)
				break;

			// hit status Ȯ��
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

		// dealer�� ī�� ����
		card_cnt[hide_card]++;
		hide_card = 0;

		// dealer�� ī�� ī��Ʈ
		for (int j = 2; j < 21; j++) {

			// hit status Ȯ��
			if (Hit_Status_Dealer(dealer)) { 

				// hit
				dealer[j] = Draw(&deck);
				card_cnt[dealer[j]]++;
			}
			// stand
			else 
				break;
		}

		// bust Ȯ��
		if ((p1_sum = Sum_Of_Num(p1)) > 21)
			p1_is_bust = true;

		if ((p2_sum = Sum_Of_Num(p2)) > 21)
			p2_is_bust = true;
		
		
		// ���� ����

		// dealer�� bust�� ���
		if ((dealer_sum = Sum_Of_Num(dealer)) > 21) {

			// p1�� bust�� �ƴ϶�� �¸�
			if (!p1_is_bust) {

				// ������ ���
				if (p1_black_jack)
					p1_betting *= 1.5;
				player1->win++;
				player1->money += p1_betting;
			}
			// p1�� bust�� ��� �й�
			else {
				player1->lose++;
				player1->money -= p1_betting;
			}

			// p2�� bust�� �ƴ϶�� �¸�
			if (!p2_is_bust) {

				// ������ ���
				if (p2_black_jack)
					p2_betting *= 1.5;
				player2->win++;
				player2->money += p2_betting;
			}
			// p2�� bust�� ��� �й�
			else {
				player2->lose++;
				player2->money -= p2_betting;
			}
		}
		// dealer�� bust�� �ƴ� ���
		else {

			// p1�� bust�� �ƴ� ��쿡��
			if (!p1_is_bust) {

				// �¸�
				if (dealer_sum < p1_sum) {

					// ������ ���
					if (p1_black_jack) 
						p1_betting *= 1.5;
					player1->money += p1_betting;
					player1->win++;
				}
				// �й�
				else if (dealer_sum > p1_sum) {
					player1->money -= p1_betting;
					player1->lose++;
				}
				// ���º�
				else 
					player1->draw++;

			}	
			// p1�� bust�� ��� �й�
			else {
				player1->lose++;
				player1->money -= p1_betting;
			}

			// p2�� bust�� �ƴ� ��쿡��
			if (!p2_is_bust) {

				// �¸�
				if (dealer_sum < p2_sum) {
					
					// ������ ���
					if (p2_black_jack)
						p2_betting *= 1.5;
					player2->money += p2_betting;
					player2->win++;
				}
				// �й�
				else if (dealer_sum > p2_sum) {
					player2->money -= p2_betting;
					player2->lose++;
				}
				// ���º�
				else
					player2->draw++;
			}	
			// p2�� bust�� ��� �й�
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
	int play_cnt = 0;	// �÷��� ��

	// �÷��� �� �Է�
	cout << "Play Game : ";
	cin >> play_cnt;

	// ���� �õ�� �ð� ���
	srand((unsigned int)time(NULL));

	// �÷��̾� ����
	struct Player player1, player2;
	player1.money = 100000;
	player1.win = 0;
	player1.draw = 0;
	player1.lose = 0;

	player2.money = 100000;
	player2.win = 0;
	player2.draw = 0;
	player2.lose = 0;

	// ���� ����
	Play_Game(play_cnt, &player1, &player2);	

	// ��� ���
	cout << "-���� ���-" << endl << endl;
	cout << "[Player1]" << endl
		<< "�¸� : " << player1.win << "\t"
		<< "���º� : " << player1.draw << "\t"
		<< "�й� : " << player1.lose << endl
		<< "�·� : " << (double)player1.win / (double)play_cnt * 100 << "%" << endl
		<< "���� �� : " << player1.money << endl;

	cout << endl << endl;

	cout << "[Player2]" << endl
		<< "�¸� : " << player2.win << "\t"
		<< "���º� : " << player2.draw << "\t"
		<< "�й� : " << player2.lose << endl
		<< "�·� : " << (double)player2.win / (double)play_cnt * 100 << "%" << endl
		<< "���� �� : " << player2.money << endl;

	return 0;
}