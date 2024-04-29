from tkinter import *
from tkinter import ttk
import sv_ttk
from CameraTab import CameraTab
from progressTab import ProgressTab
from SettingsTab import SettingsTab
from backend import Backend
class MainWindow:
    def __init__(self):
        #root window setup
        self.root = Tk()
        self.root.title("Don't Slouch")
        self.root.geometry("1200x900")
        self.window = ttk.Frame(self.root)

        #style for the window using sv_ttk
        sv_ttk.use_dark_theme()
        style = ttk.Style(self.root)
        style.configure("lefttab.TNotebook", tabposition="w", padding=[10, 0, 0, 0])
        
        #backend
        self.backend = Backend()
        
        #notebook for the tabs
        self.notebook = ttk.Notebook(self.window)  
        self.progressTab = ProgressTab(self.notebook, self.backend, self.resetProgress)
        self.cameraTab = CameraTab(self.notebook, self.backend)
        self.settingsTab = SettingsTab(self.notebook)
        
        #progress of user posture
        self.currentProgress = 0
        
        self.notebook.add(self.cameraTab.cameraFrame, text = 'Camera')
        self.notebook.add(self.settingsTab.settingsFrame, text = 'Settings')
        self.notebook.add(self.progressTab.progressFrame, text = 'Progress')
        self.notebook.pack(expand = True, fill = 'both')
        self.window.pack(expand = True, fill = 'both')
        self.root.mainloop()
        
    def resetProgress(self):
        self.currentProgress = 0
MainWindow()