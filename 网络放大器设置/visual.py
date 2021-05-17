import matplotlib.pyplot as plt
import numpy as np

time1 = open('data/time1.out')
time2 = open('data/time2.out')
t1 = []
t2 = []
t = []
s1 = 0
s2 = 0

for num in time1:
    s1 += float(num)
    t.append(float(num))
    if len(t) == 20:
        t1.append(t)
        t = []
for num in time2:
    s2 += float(num)
    t.append(float(num))
    if len(t) == 20:
        t2.append(t)
        t = []

time1.close()
time2.close()
t1.append([s1])
t2.append([s2])
bar_width = 0.3

for i in range(5):
    tick_label = [k for k in range(20 * i + 1, 20 * i + 21)]
    x = np.arange(len(tick_label))
    plt.subplot(231 + i)
    plt.bar(x, t1[i], bar_width, color='orangered', label='backtrack')
    plt.bar(x + bar_width, t2[i], bar_width, color='dodgerblue', label='branching_judge')

    plt.legend()
    plt.xticks(x + bar_width / 2, tick_label)

plt.subplot(236)
bar_width = 0.3
tick_label = [101]
x = np.arange(len(tick_label))
plt.bar(x, t1[5], bar_width, color='orangered', label='backtrack')
plt.bar(x + bar_width, t2[5], bar_width, color='dodgerblue', label='branching_judge')
plt.legend()
plt.xticks(x + bar_width / 2, tick_label)
plt.show()