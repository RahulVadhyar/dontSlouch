from tkinter import *
from tkinter import ttk

class ProgressTab:
    def __init__(self, notebook, resetProgress):
        self.progressFrame = ttk.Frame(notebook)
        self.progressLabel = ttk.Label(self.progressFrame, text = 'Progress', font= ('Helvetica bold', 120))
    
        self.title_label = ttk.Label(self.progressFrame, text = "Sit up straight! Get the high score!",anchor="center",font= ('Helvetica bold', 120))
        self.title_label.pack()
        self.score_label = ttk.Label(self.progressFrame,anchor="center",font= ('Helvetica bold', 120))
        self.score_label.pack()
        self.max_score_label = ttk.Label(self.progressFrame,anchor="center",font= ('Helvetica bold', 120))
        self.max_score_label.pack()
        
        self.progressFrame.pack(padx=10, pady=10, anchor = 'w')
        