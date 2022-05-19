import os
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import random

def plot_rectangle(ax, x1, y1, x2, y2, c):
   ax.add_patch(
      patches.Rectangle(
         (x1,y1),
         (x2-x1),
         (y2-y1),
         fill = False,
         color = c,
      )
   )

def readFile():
   file = open("placement.txt", 'r')

   fig, ax = plt.subplots(1,2, figsize = (100,30))
   bottom_repeatCount, bottom_rowLength, bottom_die_rowHeight = file.readline().split()
   bottom_repeatCount = int(bottom_repeatCount)
   bottom_rowLength = int(bottom_rowLength)
   bottom_die_rowHeight = int(bottom_die_rowHeight);
   ax[0].plot(bottom_rowLength, bottom_repeatCount * bottom_die_rowHeight, 'g.')
   ax[0].plot(0, 0, 'g.')

   BottomCellNumber = file.readline()
   BottomCellNumber = int(BottomCellNumber)
   for i in range(0, BottomCellNumber):
      cellID, rowID, left_edge, right_edge = file.readline().split()
      cellID = int(cellID)
      rowID = int(rowID)
      left_edge = int(left_edge)
      right_edge = int(right_edge)
      plot_rectangle(ax[0], left_edge, rowID * bottom_die_rowHeight, right_edge + 1, (rowID + 1) * bottom_die_rowHeight,'k')
      ax[0].text((left_edge + right_edge) / 2, (rowID + 0.5) * bottom_die_rowHeight, "C" + str(cellID + 1), fontsize = 20, color ="black") 


   top_repeatCount, top_rowLength, top_die_rowHeight = file.readline().split()
   top_repeatCount = int(top_repeatCount)
   top_rowLength = int(top_rowLength)
   top_die_rowHeight = int(top_die_rowHeight)
   ax[1].plot(top_rowLength, top_repeatCount * top_die_rowHeight, 'g.')
   ax[1].plot(0, 0, 'g.')

   TopCellNumber = file.readline()
   TopCellNumber = int(TopCellNumber)
   for i in range(0, TopCellNumber):
      cellID, rowID, left_edge, right_edge = file.readline().split()
      cellID = int(cellID)
      rowID = int(rowID)
      left_edge = int(left_edge)
      right_edge = int(right_edge)
      plot_rectangle(ax[1], left_edge, rowID * top_die_rowHeight, right_edge + 1, (rowID + 1) * top_die_rowHeight, 'k')
      ax[1].text((left_edge + right_edge) / 2, (rowID + 0.5) * top_die_rowHeight, "C" + str(cellID + 1), fontsize = 20, color ="black") 


   #plot the pin which connect to the same net
   numNets = file.readline()
   numNets = int(numNets)

   hexadecimal_alphabets = '0123456789ABCDEF'
   color = ["#" + ''.join([random.choice(hexadecimal_alphabets) for j in range(6)]) for i in range(numNets)]

   for i in range(0, numNets):
      numPins = file.readline()
      numPins = int(numPins)
      for j in range(0, numPins):
         x_cor, y_cor, comeFrom = file.readline().split()
         x_cor = int(x_cor)
         y_cor = int(y_cor)
         comeFrom = int(comeFrom)
         ax[comeFrom].plot(x_cor, y_cor, color = color[i], Marker = '*', MarkerSize = 10)
         ax[(comeFrom+1 )% 2].plot(x_cor, y_cor, color = color[i], Marker = '*', MarkerSize = 10, alpha = 0.1)


   #plot the bbox
   for i in range(0, numNets):
      x_min, x_max, y_min, y_max = file.readline().split()
      x_min = int(x_min)
      x_max = int(x_max)
      y_min = int(y_min)
      y_max = int(y_max)
      plot_rectangle(ax[0], x_min, y_min, x_max, y_max, color[i])
      plot_rectangle(ax[1], x_min, y_min, x_max, y_max, color[i])


   ax[0].grid(True, alpha = 0.3)
   ax[1].grid(True, alpha = 0.3)
   ax[0].set_title("Bottom Die Placement", fontsize = 24)
   ax[1].set_title("Top Die Placement", fontsize = 24)

   plt.savefig('test.svg')


if __name__ == '__main__':

   readFile()