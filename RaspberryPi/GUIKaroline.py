# This is the code responsible for the GUI window
# and all the buttons and fields in the GUI

import tkinter as tk 
from tkinter import *

root = tk.Tk()
root.title("CO2 H2O Gas Exchange System Datalogger")

e = tk.Entry(root, width=35, borderwidth=5)
e.grid(row=0, column=0, columnspan=3, padx=10, pady=10)

#e.insert(0,"")

#what happens with on
def myClickOn():
    
	myLabel = Label(root, text="running")
	myLabel.pack()

#what happens with quit
def myClickQuit():
	myLabel = Label(root, text="stopped")
	myLabel.pack()

#what happens with numbers
def myClick1():
    current = e.get()
    e.delete(0, END)
    e.insert(0, str(current) + str("2"))
    myLabel = Label(root, text="1")
    myLabel.pack()

def myClick2():
    current = e.get()
    e.delete(0, END)
    e.insert(0, str(current) + str("2"))
    myLabel = Label(root, text="2")
    myLabel.pack()

def myClick3():
    current = e.get()
    e.delete(0, END)
    e.insert(0, str(current) + str("3"))
    myLabel = Label(root, text="3")
    myLabel.pack()

def myClick4():
    current = e.get()
    e.delete(0, END)
    e.insert(0, str(current) + str("4"))
    myLabel = Label(root, text="4")
    myLabel.pack()

def myClick5():
    current = e.get()
    e.delete(0, END)
    e.insert(0, str(current) + str("5"))
    myLabel = Label(root, text="5")
    myLabel.pack()

def myClick6():
    current = e.get()
    e.delete(0, END)
    e.insert(0, str(current) + str("6"))
    myLabel = Label(root, text="6")
    myLabel.pack()

def myClick7():
    current = e.get()
    e.delete(0, END)
    e.insert(0, str(current) + str("7"))
    myLabel = Label(root, text="7")
    myLabel.pack()

def myClick8():
    current = e.get()
    e.delete(0, END)
    e.insert(0, str(current) + str("8"))
    myLabel = Label(root, text="8")
    myLabel.pack()

def myClick9():
    current = e.get()
    e.delete(0, END)
    e.insert(0, str(current) + str("9"))
    myLabel = Label(root, text="9")
    myLabel.pack()

def myClick0():
    current = e.get()
    e.delete(0, END)
    e.insert(0, str(current) + str("0"))
    myLabel = Label(root, text="0")
    myLabel.pack()

def myClickDot():
    current = e.get()
    e.delete(0, END)
    e.insert(0, str(current) + str("."))
    myLabel = Label(root, text=".")
    myLabel.pack()

def myClickDash():
    current = e.get()
    e.delete(0, END)
    e.insert(0, str(current) + str("-"))
    myLabel = Label(root, text="-")
    myLabel.pack()

def myClickRef():
    current = e.get()
    e.delete(0, END)
    e.insert(0, str(current) + str("Ref/Cal"))
    myLabel = Label(root, text="Ref/Cal")
    myLabel.pack()

def myClickDisplay():
    current = e.get()
    e.delete(0, END)
    e.insert(0, str(current) + str("Display"))
    myLabel = Label(root, text="Display")
    myLabel.pack()

def myClickSys():
    current = e.get()
    e.delete(0, END)
    e.insert(0, str(current) + str("System"))
    myLabel = Label(root, text="System")
    myLabel.pack()

def myClickDta():
    current = e.get()
    e.delete(0, END)
    e.insert(0, str(current) + str("Data I/O"))
    myLabel = Label(root, text="Data I/O")
    myLabel.pack()

def myClickAn():
    current = e.get()
    e.delete(0, END)
    e.insert(0, str(current) + str("Analysis"))
    myLabel = Label(root, text="Analysis")
    myLabel.pack()

def myClickPump():
    current = e.get()
    e.delete(0, END)
    e.insert(0, str(current) + str("Pump"))
    myLabel = Label(root, text="Pump")
    myLabel.pack()

def myClickBack():
    e.delete(0, END)
    myLabel = Label(root, text="<-")
    myLabel.pack()

def myClickEEX():
    current = e.get()
    e.delete(0, END)
    e.insert(0, str(current) + str("EEX"))
    myLabel = Label(root, text="EEX")
    myLabel.pack()

def myClickEnter():
    myLabel = Label(root, text="Enter")
    myLabel.pack()

#define buttons and put on screen

buttonOn = Button(root, text="On", padx=40, pady=20, command=myClickOn)
buttonOn.grid(row=1, column=1)

buttonQuit = Button(root, text="quit", padx=40, pady=20, command=myClickQuit)
buttonQuit.grid(row=1, column=2)

button1 = Button(root, text="1", padx=40, pady=20, command=myClick1)
button1.grid(row=2, column=1)

button2 = Button(root, text="2", padx=40, pady=20, command=myClick2)
button2.grid(row=2, column=2)

button3 = Button(root, text="3", padx=40, pady=20, command=myClick3)
button3.grid(row=2, column=3)

button4 = Button(root, text="4", padx=40, pady=20, command=myClick4)
button4.grid(row=3, column=1)

button5 = Button(root, text="5", padx=40, pady=20, command=myClick5)
button5.grid(row=3, column=2)

button6 = Button(root, text="6", padx=40, pady=20, command=myClick6)
button6.grid(row=3, column=3)

buttonBack = Button(root, text="<-", padx=40, pady=20, command=myClickBack)
buttonBack.grid(row=3, column=4)

button7 = Button(root, text="7", padx=40, pady=20, command=myClick7)
button7.grid(row=4, column=1)

button8 = Button(root, text="8", padx=40, pady=20, command=myClick8)
button8.grid(row=4, column=2)

button9 = Button(root, text="9", padx=40, pady=20, command=myClick9)
button9.grid(row=4, column=3)

buttonEEX = Button(root, text="EEX", padx=40, pady=20, command=myClickEEX)
buttonEEX.grid(row=4, column=4)

button0 = Button(root, text="0", padx=40, pady=20, command=myClick0)
button0.grid(row=5, column=1)

buttonDot = Button(root, text=".", padx=40, pady=20, command=myClickDot)
buttonDot.grid(row=5, column=2)

buttonDash = Button(root, text="-", padx=40, pady=20, command=myClickDash)
buttonDash.grid(row=5, column=3)

buttonEnter = Button(root, text="Enter", padx=40, pady=20, command=myClickEnter)
buttonEnter.grid(row=5, column=4)

buttonRef = Button(root, text="Ref/Cal", padx=40, pady=20, command=myClickRef)
buttonRef.grid(row=6, column=1)

buttonDisplay = Button(root, text="Display", padx=40, pady=20, command=myClickDisplay)
buttonDisplay.grid(row=6, column=2)

buttonSys = Button(root, text="System", padx=40, pady=20, command=myClickSys)
buttonSys.grid(row=6, column=3)

buttonDta = Button(root, text="Data I/O", padx=40, pady=20, command=myClickDta)
buttonDta.grid(row=7, column=1)

buttonAn = Button(root, text="Analysis", padx=40, pady=20, command=myClickAn)
buttonAn.grid(row=7, column=2)

buttonPump = Button(root, text="Pump", padx=40, pady=20, command=myClickPump)
buttonPump.grid(row=7, column=3)

root.mainloop()
