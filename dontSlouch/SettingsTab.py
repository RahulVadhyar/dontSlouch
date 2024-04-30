from tkinter import *
from tkinter import ttk
import sv_ttk

class SettingsTab:
    """
    Represents the settings tab in the application.
    
    """

    def __init__(self, notebook):
        self.settingsFrame = ttk.Frame(notebook, style='Black.TFrame')  # Set tab color to black
        self.settingsLabel = ttk.Label(self.settingsFrame, text='Settings', font=('Helvetica', 40))
        self.settingsLabel.grid(row=0, column=0, padx=10, pady=10, sticky="w")

        # theme
        self.themeFrame = ttk.Frame(self.settingsFrame)
        self.themeLabel = ttk.Label(self.themeFrame, text='Theme', font=('Helvetica', 20))
        self.themeLabel.pack(padx=10, pady=10, anchor='w')

        self.changeThemeButton = ttk.Button(self.themeFrame, text='Change Theme', command=sv_ttk.toggle_theme)
        self.changeThemeButton.pack(padx=10, pady=10, anchor='w')
        self.themeFrame.grid(row=1, column=0, padx=10, pady=10, sticky="w")

        # Device selection
        self.device_frame = ttk.Frame(self.settingsFrame)
        self.device_label = ttk.Label(self.device_frame, text='Device', font=('Helvetica', 20))
        self.device_label.pack(padx=10, pady=10, anchor='w')

        self.device_choices = (('CPU', 'cpu'),
                                ('GPU', 'cuda'))
        self.selected_device = StringVar()
        self.selected_device.set('cpu')

        for text, mode in self.device_choices:
            ttk.Radiobutton(self.device_frame, text=text, value=mode, variable=self.selected_device).pack(anchor=W,
                                                                                                               padx=10)

        self.device_frame.grid(row=2, column=0, padx=10, pady=10, sticky="w")

        # Logo
        self.logo_frame = ttk.Frame(self.settingsFrame)
        self.logo_image = PhotoImage(file='docs/Black White Elegant Monogram Initial Name Logo (1).png')
        self.logo_label = ttk.Label(self.logo_frame, image=self.logo_image)
        self.logo_label.pack(padx=10, pady=10)
        self.logo_frame.grid(row=3, column=2, padx=10, pady=10, sticky="se")  # Bottom right corner
