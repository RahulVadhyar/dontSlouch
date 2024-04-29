from tkinter import *
from tkinter import ttk

class ProgressTab:
    def __init__(self, notebook, backend, resetProgress):
        self.progressFrame = ttk.Frame(notebook)
        self.progressLabel = ttk.Label(self.progressFrame, text = 'Progress', font= ('Arial', 40))
        
        self.resetProgressButton = ttk.Button(self.progressFrame, text = 'Reset Progress', command = resetProgress)
        self.resetProgressButton.pack(padx=10, pady=10, anchor = 'w')
        self.progressFrame.pack(padx=10, pady=10, anchor = 'w')