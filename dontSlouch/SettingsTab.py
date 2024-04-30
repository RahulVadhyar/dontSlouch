from tkinter import *
from tkinter import ttk
import sv_ttk
class SettingsTab:
    def __init__(self, notebook):
        self.settingsFrame = ttk.Frame(notebook)
        self.settingsLabel = ttk.Label(self.settingsFrame, text = 'Settings', font= ('Helvetica', 40))
        self.settingsLabel.pack(padx=10, pady=10, anchor = 'w')
        
        #theme
        self.themeFrame = ttk.Frame(self.settingsFrame)
        self.themeLabel = ttk.Label(self.themeFrame, text = 'Theme', font= ('Helvetica', 20))
        self.themeLabel.pack(padx=10, pady=10, anchor = 'w')
        
        self.changeThemeButton = ttk.Button(self.themeFrame, text = 'Change Theme', command = sv_ttk.toggle_theme)
        self.changeThemeButton.pack(padx=10, pady=10, anchor = 'w')
        self.themeFrame.pack(padx=10, pady=10, anchor = 'w')
        
        self.device_frame = ttk.Frame(self.settingsFrame)
        self.device_label = ttk.Label(self.device_frame, text = 'Device', font= ('Helvetica', 20))
        self.device_label.pack(padx=10, pady=10, anchor = 'w')
        
        self.device_choices = (('CPU', 'cpu'),
                        ('GPU', 'cuda'))
        self.selected_device = StringVar()
        self.selected_device.set('cpu')
        
        for text, mode in self.device_choices:
            ttk.Radiobutton(self.device_frame, text=text, value=mode, variable=self.selected_device).pack(anchor=W, padx=10)
            
        self.device_frame.pack(padx=10, pady=10, anchor = 'w')