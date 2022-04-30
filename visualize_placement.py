import matplotlib.pyplot as plt
import matplotlib.patches as patches

fig, ax = plt.subplots()

ax.add_patch(
     patches.Rectangle(
        (1, 1),
        0.5,
        0.5,
        edgecolor = 'blue',
        facecolor = 'red',
        fill=True
     ) )

plt.savefig('test.svg')