#include <stdio.h>

struct lifeNum {
    int birth;
    int lifePath;
    int pinna1;
    int pinna2;
    int pinna3;
    int peak1;
};

struct results {
    char type[32];
    char text1[128];
    char text2[128];
    char text3[128];
};

struct results pathType[23] = {
    {"", "", "", ""},
    {"先駆者", "未知の領域を開拓する力がある"},
    {"調停者", "人の心の動きや要求を敏感に察知することができる。"},
    {"表現者", "生み出すアイディアが人を魅了する。"},
    {"管理者", "バラバラなものをまとめたり、アイディアをまとめる力がある。"},
    {"勝負師", "度胸の良さと決断力が光る"},
    {"後継者", "先人から引継ぎ、後進を育てる力がある"},
    {"研究者", "鋭い観察力と飽くなき探求心で才能を発揮する。"},
    {"指揮者", "人を指揮して導く力がある。"},
    {"哲学者", "物事の根本を見抜く優れた力がある。"},
    {"", "", "", ""},
    {"メッセンジャー", "人に気づきを与える。問題を天性の才能で見抜くことができる。"},
    {"", "", "", ""},
    {"", "", "", ""},
    {"", "", "", ""},
    {"", "", "", ""},
    {"", "", "", ""},
    {"", "", "", ""},
    {"", "", "", ""},
    {"", "", "", ""},
    {"", "", "", ""},
    {"", "", "", ""},
    {"政治家・建築家", "パワフルな実行力と設計力を持っている。大局的にものを見るセンスがある。"}
};

struct results lifePeak[23] = {
    {"", "", "", ""},
    {"1", "始まり", "トップの座につく", "トップの座に押し上げる"},
    {"2", "ゆったり", "おだやか", "ご縁による豊かさ"},
    {"3", "軽快", "楽天的な生き方", "好奇心と創造性"},
    {"4", "こつこつ", "生産的なパワー", "努力に対する褒美"},
    {"5", "自由", "自由でエキサイティングな変化", "人生に再挑戦"},
    {"6", "愛と契約に守られる", "絆と制約", "人のために尽くす余裕"},
    {"7", "深い思索", "強力な探求心", "魂の進化"},
    {"8", "パワフルな空気", "目的達成力", "夢や目的を達成"},
    {"9", "恩恵に満ちている", "成熟と完成", "これまでの学びを統合する"},
    {"", "", "", ""},
    {"11", "神聖なパワー", "感受性", "あらゆるチャンスをつかむ"},
    {"", "", "", ""},
    {"", "", "", ""},
    {"", "", "", ""},
    {"", "", "", ""},
    {"", "", "", ""},
    {"", "", "", ""},
    {"", "", "", ""},
    {"", "", "", ""},
    {"", "", "", ""},
    {"", "", "", ""},
    {"22", "生産性に満ちている", "壮大な計画が形に", "自分にかかわるあらゆる人に実りをもたらす"}
};

void digit_sum(int *num) {
    int check = 0;
    while (check == 0) {
        if (*num == 11 || *num == 22) return;
        check = 1;
        int temp = *num;
        *num = 0;
        while (temp > 0) {
            *num += temp % 10;
            temp /= 10;
        }
        if (*num >= 10) check = 0;
    }
}

int main(void) {
    struct lifeNum data = {
        0,
        0,
        0,
        0,
        0,
        0
    };
    int i = 0, temp = 0;
    char sInput[9] = "00000000";
    int check = 0;
    while (check == 0) {
        check = 1;
        data.birth = i = 0;
        scanf("%8s", sInput);
        while (getchar() != '\n');
        while (sInput[i] != '\0') {
            if (!(sInput[i] >= '0' && sInput[i] <= '9')) {
                check = 0;
                break;
            }
            data.birth = data.birth * 10 + (sInput[i] - '0');
            i++;
        }
    }

    i = 7;
    temp = data.birth;
    while (temp > 0) {
        int digit = temp % 10;
        if (i < 4) {
            data.pinna2 += digit;
        }
        if (i > 3 && i < 6) {
            data.pinna1 += digit;
        }
        if (i > 5 && i < 8) {
            data.pinna1 += digit;
            data.pinna2 += digit;
        }
        if (i < 8 && i >= 0) data.lifePath += digit;
        temp /= 10;
        i--;
    }

    digit_sum(&data.lifePath);
    digit_sum(&data.pinna1);
    digit_sum(&data.pinna2);
    data.pinna3 = data.pinna1 + data.pinna2;
    digit_sum(&data.pinna3);
    data.peak1 = 36 - data.lifePath;

    printf("あなたのライフパスナンバーは、%d で、%sタイプです！\n", data.lifePath, pathType[data.lifePath].type);
    printf("あなたは、%s\n", pathType[data.lifePath].text1);
    printf("人生の山場１：0～%d 歳、人生の山場２：%d 歳～%d 歳、人生の山場３：%d 歳～%d 歳\n",
        data.peak1, data.peak1, data.peak1 + 9, data.peak1 + 9, data.peak1 + 18);
    printf("人生の山場１キーワード：%s、人生の山場２キーワード：%s、人生の山場３キーワード：%s\n",
        lifePeak[data.pinna1].text1, lifePeak[data.pinna2].text2, lifePeak[data.pinna3].text3);

    return 0;
}