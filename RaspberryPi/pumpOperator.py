import time
import RPi.GPIO as GPIO

class pumpOperator:

	_is_running = False
	
	def __init__ (self):
		GPIO.setmode(GPIO.BCM)
		GPIO.setwarnings(False)
		GPIO.setup(5, GPIO.OUT)
		GPIO.setup(6, GPIO.OUT)
		GPIO.setup(13, GPIO.OUT)
		GPIO.setup(20, GPIO.OUT)
		GPIO.setup(19, GPIO.OUT)
		GPIO.setup(26, GPIO.OUT)
		GPIO.output(5, False)
		GPIO.output(6, False)
		GPIO.output(13, False)
		GPIO.output(20, False)
		GPIO.output(19, False)
		GPIO.output(26, False)
		self.decay = 0.001e-3 # Measured from rise and fall times.
	
	def setup (self, frequency, phase):
		self.frequency = frequency
		self.phase = phase
	
	def start (self):
		self._is_running = True
		state = 0
		if self.phase == 1:
			hold = 1/self.frequency/6
			while self._is_running:
				state = self.sixPhaseForward(state, hold)
	
		elif self.phase == 2:
			hold = 1/self.frequency/5
			while self._is_running:
				state = self.fivePhaseForward(state, hold)
	
		elif self.phase == 3:
			hold = 1/self.frequency/4
			while self._is_running:
				state = self.fourPhaseForward(state, hold)
	
		elif self.phase == 4:
			hold = 1/self.frequency/6
			while self._is_running:
				state = self.sixPhaseReverse(state, hold)
	
		elif self.phase == 5:
			hold = 1/self.frequency/5
			while self._is_running:
				state = self.fivePhaseReverse(state, hold)
	
	
	
		elif self.phase == 6:
			hold = 1/self.frequency/4
			while self._is_running:
				state = self.fourPhaseReverse(state, hold)
	
	def stop (self):
		self._is_running = False
		GPIO.output(5, False)
		GPIO.output(6, False)
		GPIO.output(13, False)
		GPIO.output(20, False)
		GPIO.output(19, False)
		GPIO.output(26, False)
		return False
	
	def fourPhaseForward (self, state, hold):
		if state == 0:
			print(state)
			state = 1
			time.sleep(hold)
		elif state == 1:
			print(state)
			state = 2
			time.sleep(hold)
		elif state == 2:
			print(state)
			state = 3
			time.sleep(hold)
		elif state == 3:
			print(state)
			state = 4
			time.sleep(hold)
		elif state == 4:
				print(state)
				state = 1
				time.sleep(hold)
	
		return state
	
	def fivePhaseForward (self, state, hold):
		if state == 0:
			print(state)
			state = 1
			time.sleep(hold)
		elif state == 1:
			print(state)
			state = 2
			time.sleep(hold)
		elif state == 2:
			print(state)
			state = 3
			time.sleep(hold)
		elif state == 3:
			print(state)
			state = 4
			time.sleep(hold)
		elif state == 4:
			print(state)
			state = 5
			time.sleep(hold)
		elif state == 5:
			print(state)
			state = 1
			time.sleep(hold)
	
		return state
	
	def sixPhaseForward (self, state, hold):
		if state == 0:
			self.oneOff(hold)
			self.twoOff(hold)
			self.threeOff(hold)
			time.sleep(hold)
			self.oneOn(hold)
			state = 1
			time.sleep(hold)
		elif state == 1:
			self.twoOn(hold)
			state = 2
			time.sleep(hold)
		elif state == 2:
			self.threeOn(hold)
			state = 3
			time.sleep(hold)
		elif state == 3:
			self.oneOff(hold)
			state = 4
			time.sleep(hold)
		elif state == 4:
			self.twoOff(hold)
			state = 5
			time.sleep(hold)
		elif state == 5:
			self.oneOn(hold)
			state = 6
			time.sleep(hold)
		elif state == 6:
			self.threeOff(hold)
			state = 1
			time.sleep(hold)
	
		return state
	
	def fourPhaseReverse (self, state, hold):
		if state == 0:
			print(state)
			state = 4
			time.sleep(hold)
		elif state == 1:
			print(state)
			state = 4
			time.sleep(hold)
		elif state == 2:
			print(state)
			state = 1
			time.sleep(hold)
		elif state == 3:
			print(state)
			state = 2
			time.sleep(hold)
		elif state == 4:
			print(state)
			state = 3
			time.sleep(hold)
	
		return state
	
	def fivePhaseReverse (self, state, hold):
		if state == 0:
			print(state)
			state = 5
			time.sleep(hold)
		elif state == 1:
			print(state)
			state = 5
			time.sleep(hold)
		elif state == 2:
			print(state)
			state = 1
			time.sleep(hold)
		elif state == 3:
			print(state)
			state = 2
			time.sleep(hold)
		elif state == 4:
			print(state)
			state = 3
			time.sleep(hold)
		elif state == 5:
			print(state)
			state = 4
			time.sleep(hold)
	
		return state
	
	def sixPhaseReverse (self, state, hold):
		if state == 0:
			self.oneOff(hold)
			self.twoOff(hold)
			self.threeOff(hold)
			time.sleep(hold)
			self.oneOn(hold)
			state = 6
			time.sleep(hold)
		elif state == 1:
			self.twoOn(hold)
			state = 6
			time.sleep(hold)
		elif state == 2:
			self.threeOn(hold)
			state = 1
			time.sleep(hold)
		elif state == 3:
			self.oneOff(hold)
			state = 2
			time.sleep(hold)
		elif state == 4:
			self.twoOff(hold)
			state = 3
			time.sleep(hold)
		elif state == 5:
			self.oneOn(hold)
			state = 4
			time.sleep(hold)
		elif state == 6:
			self.threeOff(hold)
			state = 5
			time.sleep(hold)
	
		return state
	
	def oneOn(self, hold):
		GPIO.output(6, False)
		time.sleep(self.decay)
		GPIO.output(5, True)
	
	def oneOff(self, hold):
		GPIO.output(5, False)
		time.sleep(self.decay)
		GPIO.output(6, True)
	
	def twoOn(self, hold):
		GPIO.output(19, False)
		time.sleep(self.decay)
		GPIO.output(13, True)
	
	def twoOff(self, hold):
		GPIO.output(13, False)
		time.sleep(self.decay)
		GPIO.output(19, True)
	
	def threeOn(self, hold):
		GPIO.output(20, False)
		time.sleep(self.decay)
		GPIO.output(26, True)
	
	def threeOff(self, hold):
		GPIO.output(26, False)
		time.sleep(self.decay)
		GPIO.output(20, True)
