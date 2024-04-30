from tkinter import *
from tkinter import ttk

class ProgressTab:
    def __init__(self, notebook, backend, resetProgress):
        self.progressFrame = ttk.Frame(notebook)
        self.progressLabel = ttk.Label(self.progressFrame, text = 'Progress', font= ('Arial', 40))
        
        self.resetProgressButton = ttk.Button(self.progressFrame, text = 'Reset Progress', command = resetProgress)
        self.resetProgressButton.pack(padx=10, pady=10, anchor = 'w')
        
         # Load the image
        self.image = PhotoImage(file="../yearly.png")

        # Create a label to display the image
        self.imageLabel = ttk.Label(self.progressFrame, image=self.image)
        self.imageLabel.pack(padx=10, pady=10, anchor='w')
        self.progressFrame.pack(padx=10, pady=10, anchor = 'w')
        