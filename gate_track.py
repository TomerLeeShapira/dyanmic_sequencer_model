import sys
import os
from PySide6.QtGui import *
from PySide6.QtCore import *
from PySide6.QtWidgets import *

class Gate_Track(QWidget):

	def __init__(self):
		super(Gate_Track, self).__init__()

		self.number_of_steps = 0
		self.track = QHBoxLayout()

		self.setLayout(self.track)

	# Add a step to the track. Add to the end. 
	def add_step(self, num_steps):

		for i in range(num_steps):
			step = QPushButton()
			step.setObjectName("step_button")
			step.setText(str(i))
			self.track.addWidget(step)

	# Remove a step from the track. Remove from the end. 
	def remove_step(self, num_steps):
		pass