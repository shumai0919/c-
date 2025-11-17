import random

answer = random.randint(1, 10)

print("1〜10の数字を当てて！")

while True:
    try:
        guess = int(input("> "))
    except ValueError:
        print("数字を入力してね！")
        continue

    if guess == answer:
        print("正解！🎉")
        break
    elif guess < answer:
        print("もっと大きいよ")
    else:
        print("もっと小さいよ")
