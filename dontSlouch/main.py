from tkinter import *
from tkinter import ttk
import sv_ttk
from CameraTab import CameraTab
from progressTab import ProgressTab
from SettingsTab import SettingsTab
from backend import Backend
class MainWindow:
    """
    Represents the main window of the 'Don't Slouch' application.

    This class is responsible for creating and managing the main window of the application.
    It contains methods for setting up the window, initializing the backend, and managing the tabs.

    Attributes:
        root (Tk): The root window of the application.
        window (ttk.Frame): The main frame of the window.
        backend (Backend): The backend instance for handling application logic.
        notebook (ttk.Notebook): The notebook widget for managing tabs.
        settingsTab (SettingsTab): The settings tab instance.
        progressTab (ProgressTab): The progress tab instance.
        cameraTab (CameraTab): The camera tab instance.
        currentProgress (int): The current progress of user posture.

    Methods:
        __init__(): Initializes the MainWindow object and sets up the main window.
        resetProgress(): Resets the current progress of user posture.

    """

    def __init__(self):
        """
        Initializes the MainWindow object and sets up the main window.

        This method creates the root window, sets its title and geometry, and creates the main frame.
        It also sets up the style for the window using sv_ttk and initializes the backend.
        Finally, it creates the notebook widget and adds the tabs to it.

        """
        # root window setup
        self.root = Tk()
        self.root.title("Don't Slouch")
        self.root.geometry("1200x900")
        self.window = ttk.Frame(self.root)

        # style for the window using sv_ttk
        sv_ttk.use_dark_theme()
        style = ttk.Style(self.root)
        style.configure("lefttab.TNotebook", tabposition="w", padding=[10, 0, 0, 0])

        # backend
        self.backend = Backend()

        # notebook for the tabs
        self.notebook = ttk.Notebook(self.window)
        self.settingsTab = SettingsTab(self.notebook)
        self.progressTab = ProgressTab(self.notebook, self.resetProgress)
        self.cameraTab = CameraTab(self.notebook, self.backend, self.settingsTab.selected_device, self.progressTab)

        # progress of user posture
        self.currentProgress = 0

        self.notebook.add(self.progressTab.progressFrame, text='Progress')
        self.notebook.add(self.cameraTab.cameraFrame, text='Camera')
        self.notebook.add(self.settingsTab.settingsFrame, text='Settings')
        self.notebook.pack(expand=True, fill='both')
        self.window.pack(expand=True, fill='both')
        self.root.mainloop()

    def resetProgress(self):
        """
        Resets the current progress of user posture.

        This method is called when the user wants to reset the progress of their posture.
        It sets the current progress to 0.

        """
        self.currentProgress = 0
MainWindow()