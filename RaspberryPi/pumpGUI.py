#!/usr/bin/python3

import tkinter as tk
import threading
import pumpOperator

exitFlag = 0

class myThread (threading.Thread):
    def __init__(self, threadID, name, counter, frame):
        threading.Thread.__init__(self)
        global pump
        self._is_running = True
        self.threadID = threadID
        self.name = name
        self.counter = counter
        self.frame = frame

    def run(self):
        # Sanitize input
        global pump
        rate = self.frame.frequencyEntry.get()
        try:
            rate = int(rate)
        except ValueError:
            self.frame.status.config(text="Integers only!")
            return

        self.frame.quitButton.config(state='disabled')
        self.frame.oneButton.config(state='disabled')
        self.frame.twoButton.config(state='disabled')
        self.frame.threeButton.config(state='disabled')
        self.frame.startButton.config(state='disabled')
        self.frame.stopButton.config(state='normal')

        phases = {1 : "6 Phase Forward",
            2 : "5 Phase Forward",
            3 : "4 Phase Forward",
            4 : "6 Phase Reverse",
            5 : "5 Phase Reverse",
            6 : "4 Phase Reverse"}
        phase = int(self.frame.phase.get())
        self.frame.status.config(text="Pumping started at " + str(rate) + "Hz, using " + str(phases[phase]))
        self.frame.indicator.config(bg='red')
        pump.setup(rate, phase)
        pump.start()
        self.frame.indicator.config(bg='green')
        self.frame.status.config(text="Pumping stopped")
        self.frame.quitButton.config(state='normal')
        self.frame.oneButton.config(state='normal')
        self.frame.twoButton.config(state='normal')
        self.frame.threeButton.config(state='normal')
        self.frame.startButton.config(state='normal')
        self.frame.stopButton.config(state='disabled')

    def exit(self):
        global pump
        self._is_running = False
        pump.stop()

class GUI(tk.Frame):
    def __init__(self, master):
        global root
        global oneStatus
        global twoStatus
        global threeStatus
        global pump

        tk.Frame.__init__(self, master)

        w = tk.Message(master, text="Pump Controller", width=200, font='Times 18 bold')
        w.grid(row=1, columnspan=2, sticky="wens", padx=5, pady=5)

        ## Automatic sequence group
        self.group = tk.LabelFrame(self, text="Automatic pump sequence", padx=5, pady=5)
        self.group.grid(row=2, column=1, padx=5, pady=5)
        self.fLabel = tk.Label(self.group, text="Enter Frequency", font='Times 12 bold').grid(row=1, column=1)


        self.frequencyEntry = tk.Entry(self.group, width=20)
        self.frequencyEntry.grid(row=1, column=2, columnspan=2)
        self.frequencyEntry.insert(0, '2')
        self.phase = tk.IntVar()
        self.phase6 = tk.Radiobutton(self.group, text='6 phase', variable=self.phase, value=1)
        self.phase6.grid(row=2, column=1)
        self.phase5 = tk.Radiobutton(self.group, text='5 phase', variable=self.phase, value=2)
        self.phase5.grid(row=3, column=1)
        self.phase4 = tk.Radiobutton(self.group, text='4 phase', variable=self.phase, value=3)
        self.phase4.grid(row=4, column=1)
        self.phase6r = tk.Radiobutton(self.group, text='6 phase reverse', variable=self.phase, value=4)
        self.phase6r.grid(row=2, column=2)
        self.phase5r = tk.Radiobutton(self.group, text='5 phase reverse', variable=self.phase, value=5)
        self.phase5r.grid(row=3, column=2)
        self.phase4r = tk.Radiobutton(self.group, text='4 phase reverse', variable=self.phase, value=6)
        self.phase4r.grid(row=4, column=2)
        self.phase.set(1)
        self.indicator = tk.Frame(self.group, width=40, height=40, bg='green')
        self.indicator.grid(row=2, rowspan=3, column=3)
        self.startButton = tk.Button(self.group, text='Start', command=self.start, underline=3)
        self.startButton.grid(row=5, column=1)
        self.stopButton = tk.Button(self.group, text='Stop', command=self.stop, state='disabled', underline=3)
        self.stopButton.grid(row=5, column=2)
        self.status = tk.Label(self.group, text="Status", width=50)
        self.status.grid(row=6, columnspan=3)

        self.group2 = tk.LabelFrame(self, text="Manual electrode sequence", padx=5, pady=5)
        self.group2.grid(row=3, column=1, sticky="w", padx=5, pady=5)
        self.oneButton = tk.Button(self.group2, text='1 On', command=self.oneToggle, underline=0)
        self.oneButton.grid(row=1, column=1)


        self.twoButton = tk.Button(self.group2, text='2 On', command=self.twoToggle, underline=0)
        self.twoButton.grid(row=1, column=2)
        self.threeButton = tk.Button(self.group2, text='3 On', command=self.threeToggle, underline=0)
        self.threeButton.grid(row=1, column=3)

        self.quitButton = tk.Button(self, text='Quit', command=self.quit)
        self.quitButton.grid(row=3, column=1, sticky="e", padx=5, pady=5)

        master.bind('1', self.oneToggle)
        master.bind('2', self.twoToggle)
        master.bind('3', self.threeToggle)
        master.bind('<KP_1>', self.oneToggle)
        master.bind('<KP_2>', self.twoToggle)
        master.bind('<KP_3>', self.threeToggle)
        master.bind('r', self.start)
        master.bind('p', self.stop)

    def stop(self, event=None):
        if (self.stopButton.cget('state') == 'normal') or (self.stopButton.cget('state') == 'active'):
            self.thread1.exit()

    def start(self, event=None):
        global oneStatus
        global twoStatus
        global threeStatus
        if (self.startButton.cget('state') == 'normal') or (self.startButton.cget('state') == 'active'):
            self.thread1 = myThread(1, "Thread-1", 1, self)
            self.thread1.start()
            self.oneButton.config(text="1 On")
            self.twoButton.config(text="2 On")
            self.threeButton.config(text="3 On")
            oneStatus = False
            twoStatus = False
            threeStatus = False

    def quit(self, event=None):
        root.destroy()

    def oneToggle(self, event=None):
        global pump
        global oneStatus
        if (self.oneButton.cget('state') == 'normal') or (self.oneButton.cget('state') == 'active'):
            if oneStatus:
                pump.oneOff(0)
                oneStatus = False
                self.oneButton.config(text="1 On")
            else:
                pump.oneOn(0)
                oneStatus = True
                self.oneButton.config(text="1 Off")

    def twoToggle(self, event=None):
        global pump
        global twoStatus
        if (self.twoButton.cget('state') == 'normal') or (self.twoButton.cget('state') == 'active'):
            if twoStatus:
                pump.twoOff(0)
                twoStatus = False
                self.twoButton.config(text="2 On")
            else:
                pump.twoOn(0)
                twoStatus = True
                self.twoButton.config(text="2 Off")

    def threeToggle(self, event=None):
        global pump
        global threeStatus
        if (self.threeButton.cget('state') == 'normal') or (self.threeButton.cget('state') == 'active'):
            if threeStatus:
                pump.threeOff(0)
                threeStatus = False
                self.threeButton.config(text="3 On")
            else:
                pump.threeOn(0)
                threeStatus = True
                self.threeButton.config(text="3 Off")

if __name__ == '__main__':
    root=tk.Tk()
    oneStatus = False
    twoStatus = False
    threeStatus = False
    pump = pumpOperator.pumpOperator()

    #root.geometry("500x300")
    root.title("Pump controller")


    win=GUI(root)
    win.grid()
    root.mainloop()