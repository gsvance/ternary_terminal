"""Class and utilities for computing experiments using balanced ternary.

Some references:
https://en.wikipedia.org/wiki/Balanced_ternary

Last modified 5/22/19 by Greg Vance
"""

import random

class Trit:
	"""Represents one physical hardware trit inside of a ternary computer."""
	
	## Initialization method ##
	
	def __init__(self, value=None):
		"""The input value should either be 'T' or be convertable to an int.
		An input of 'T' is considered to be equivalent to passing in -1.
		If value is converted to an int, the int must be one of {-1, 0, +1}.
		When no value is passed, the trit's initial state is randomized.
		"""
		if value is None:
			# Randomize to represent uninitialized computer memory
			# Note that range(-1, 2) == [-1, 0, 1]
			self.state = random.randrange(-1, 2)
		elif value is 'T':
			self.state = -1
		else:
			# Attempt conversion to an int and raise an error on failure
			try:
				v = int(value)
			except:
				raise ValueError("trit could not convert %s to int" % (value))
			# Check the actual value of the int before accepting it
			if -1 <= v <= +1:
				self.state = v
			else:
				raise ValueError("trit recieved invalid int value %d" % (v))
	
	## Representation methods ##
	
	def __str__(self):
		if self.state == -1:
			return 'T'
		else:
			return str(self.state)
	
	def __repr__(self):
		a = str(self)
		if a is 'T':
			a = repr(a)
		return "Trit(%s)" % (a)
	
	## Comparison methods ##
	
	def __eq__(self, other):
		try:
			return self.state == other.state
		except:
			return self.state == other
	
	def __ne__(self, other):
		try:
			return self.state != other.state
		except:
			return self.state != other
	
	def __lt__(self, other):
		try:
			return self.state < other.state
		except:
			return self.state < other
	
	def __gt__(self, other):
		try:
			return self.state > other.state
		except:
			return self.state > other
	
	def __le__(self, other):
		try:
			return self.state <= other.state
		except:
			return self.state <= other
	
	def __ge__(self, other):
		try:
			return self.state >= other.state
		except:
			return self.state >= other
