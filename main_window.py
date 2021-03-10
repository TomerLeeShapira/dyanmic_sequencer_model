import sys
import os
from PySide6.QtGui import *
from PySide6.QtCore import *
from PySide6.QtWidgets import *
from gate_track import Gate_Track


class Sequencer(QWidget):

	def __init__(self):
		super(Sequencer, self).__init__()

		self.grid = QGridLayout()

		self.new_track = Gate_Track()
		self.new_track.add_step(4)
		self.grid.addWidget(self.new_track)
		self.new_track.add_step(4)

		self.setLayout(self.grid)


class Main_Window(QMainWindow):

	def __init__(self):
		super(Main_Window, self).__init__()
		self.grid = QWidget()
		self.sequencer = Sequencer()
		layout = QVBoxLayout()
		layout.addWidget(self.sequencer)
		self.grid.setLayout(layout)
		self.setCentralWidget(self.grid)

		self.sequencer.new_track.add_step(6)


if __name__ == '__main__':

	app = QApplication(sys.argv)
	ui = Main_Window()
	ui.show()
	sys.exit(app.exec_())