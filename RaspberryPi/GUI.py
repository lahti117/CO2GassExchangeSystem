# This is the code responsible for the GUI window
# and all the buttons and fields in the GUI

import tkinter as tk

window = tk.Tk()

frame = tk.Frame()

mainLabel = tk.Label(text="CO2 H20 Gas Exchange System Datalogger", master=frame, width=100, height=3)
mainLabel.pack()
frame.pack()
window.mainloop()