from tkinter import *
from tkinter import ttk

class ProgressTab:
    """
    Represents a progress tab in the application.

    Args:
        notebook (ttk.Notebook): The parent notebook widget.
        resetProgress (bool): Flag indicating whether to reset the progress.

    Attributes:
        progressFrame (ttk.Frame): The frame widget for the progress tab.
        progressLabel (ttk.Label): The label widget for the progress tab.
        title_label (ttk.Label): The label widget for the title.
        score_label (ttk.Label): The label widget for the score.
        max_score_label (ttk.Label): The label widget for the maximum score.
    """
    def __init__(self, notebook, resetProgress):
        self.progressFrame = ttk.Frame(notebook)
        self.progressLabel = ttk.Label(self.progressFrame, text='Progress', font=('Helvetica', 24, 'bold'))

        self.title_label = ttk.Label(self.progressFrame, text="Sit up straight! Get the high score!",
                                     anchor="center", font=('Helvetica', 24, 'bold'))
        self.title_label.pack(pady=(100, 10), expand=True)
        
        self.score_label = ttk.Label(self.progressFrame, anchor="center", font=('Helvetica', 24, 'bold'))
        self.score_label.pack(expand=True)
        
        self.max_score_label = ttk.Label(self.progressFrame, anchor="center", font=('Helvetica', 24, 'bold'))
        self.max_score_label.pack(expand=True)
        
        self.progressFrame.pack(padx=10, pady=10, anchor='w')
