from tkinter import *
from tkinter import ttk
import tkinter
import sv_ttk
from tkinter.messagebox import showinfo
import os
import cv2
from PIL import Image, ImageTk 

#configure the root window and set the ttk style
root = tkinter.Tk()
root.title('Scene Understanding')
root.geometry('1200x900')
window = ttk.Frame(root)
sv_ttk.use_dark_theme()
style = ttk.Style(root)
style.configure('lefttab.TNotebook', tabposition='w', padding = [10, 0, 0, 0])


#create a notebook(tabs interface)
notebook = ttk.Notebook(window)
#Tab 1: Progress Tab--------------------------------------------------------
progress_frame = ttk.Frame(notebook)
progress_label = ttk.Label(progress_frame, text = 'Progress', font= ('Arial', 40))

#Tab 2: Camera Tab --------------------------------------------------------
camera_frame = ttk.Frame(notebook)
camera_label = ttk.Label(camera_frame, text = 'Camera', font= ('Arial', 40))
label_widget = ttk.Label(camera_frame, padding = [0, 50, 0, 0]) 
label_widget.pack() 
# Define a video capture object 
vid = cv2.VideoCapture(0) 
  
# Declare the width and height in variables 
width, height = 800, 600
  
# Set the width and height 
vid.set(cv2.CAP_PROP_FRAME_WIDTH, width) 
vid.set(cv2.CAP_PROP_FRAME_HEIGHT, height)


def open_camera(): 
    # Capture the video frame by frame 
    _, frame = vid.read() 
    # Convert image from one color space to other 
    opencv_image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGBA) 
    # Capture the latest frame and transform to image 
    captured_image = Image.fromarray(opencv_image) 
    # Convert captured image to photoimage 
    photo_image = ImageTk.PhotoImage(image=captured_image) 
    # Displaying photoimage in the label 
    label_widget.photo_image = photo_image 
    # Configure image in the label 
    label_widget.configure(image=photo_image) 
    # Repeat the same process after every 10 seconds 
    label_widget.after(10, open_camera)
open_camera()

#Tab 3: Settings Tab--------------------------------------------------------
settings_frame = ttk.Frame(notebook)

settings_label = ttk.Label(settings_frame, text = 'Settings', font= ('Arial', 40))
settings_label.pack(padx=10, pady=10, anchor = 'w')

#theme
theme_frame = ttk.Frame(settings_frame)
theme_label = ttk.Label(theme_frame, text = 'Theme', font= ('Arial', 20))
theme_label.pack(padx=10, pady=10, anchor = 'w')

change_theme_button = ttk.Button(theme_frame, text = 'Change Theme', command = sv_ttk.toggle_theme)
change_theme_button.pack(padx=10, pady=10, anchor = 'w')
theme_frame.pack(padx=10, pady=10, anchor = 'w')

#device
device_frame = ttk.Frame(settings_frame)
device_label = ttk.Label(device_frame, text = 'Device', font= ('Arial', 20))
device_label.pack(padx=10, pady=10, anchor = 'w')

device_choices = (('CPU', 'cpu'),
                ('GPU', 'cuda'))
selected_device = tkinter.StringVar()
selected_device.set('cuda')

for text, mode in device_choices:
    ttk.Radiobutton(device_frame, text=text, value=mode, variable=selected_device).pack(anchor=tkinter.W, padx=10)

device_frame.pack(padx=10, pady=10, anchor = 'w')
#--------------------------------------------------------

#Add the frames to the notebook
notebook.add(progress_frame, text = 'Progress')
notebook.add(camera_frame, text = 'Camera')
notebook.add(settings_frame, text = 'Settings')
notebook.pack(expand = True, fill = 'both')
window.pack(expand = True, fill = 'both')
root.mainloop()