import os
import matplotlib.pyplot as plt
import matplotlib.patches as patches


def plot_rectangle(ax, x1, y1, x2, y2):
   ax.add_patch(
      patches.Rectangle(
         (x1,y1),
         (x2-x1),
         (y2-y1),
         fill = False,
      )
   )

def readFile():
   file = open("placement.txt", 'r')

   fig, ax = plt.subplots(1,2, figsize = (20,6))
   bottom_repeatCount, bottom_rowLength = file.readline().split()
   bottom_repeatCount = int(bottom_repeatCount)
   bottom_rowLength = int(bottom_rowLength)
   ax[0].plot(bottom_rowLength, bottom_repeatCount, 'g*')

   BottomCellNumber = file.readline()
   BottomCellNumber = int(BottomCellNumber)
   for i in range(0, BottomCellNumber):
      cellID, rowID, left_edge, right_edge = file.readline().split()
      cellID = int(cellID)
      rowID = int(rowID)
      left_edge = int(left_edge)
      right_edge = int(right_edge)
      plot_rectangle(ax[0], left_edge, rowID, right_edge + 1, rowID + 1)
      ax[0].text((left_edge + right_edge) / 2, rowID + 0.5, "C" + str(cellID + 1), fontsize = 30, color ="red") 


   top_repeatCount, top_rowLength = file.readline().split()
   top_repeatCount = int(top_repeatCount)
   top_rowLength = int(top_rowLength)
   ax[1].plot(top_rowLength, top_repeatCount, 'g*')

   TopCellNumber = file.readline()
   TopCellNumber = int(TopCellNumber)
   for i in range(0, TopCellNumber):
      cellID, rowID, left_edge, right_edge = file.readline().split()
      cellID = int(cellID)
      rowID = int(rowID)
      left_edge = int(left_edge)
      right_edge = int(right_edge)
      plot_rectangle(ax[1], left_edge, rowID, right_edge + 1, rowID + 1)
      ax[1].text((left_edge + right_edge) / 2, rowID + 0.5, "C" + str(cellID + 1), fontsize = 30, color ="red") 


   plt.savefig('test.svg')


if __name__ == '__main__':

   readFile()