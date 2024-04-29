from tkinter import *
from tkinter import ttk
import cv2
from PIL import Image, ImageTk 

class CameraTab:
    def __init__(self, notebook, backend):
        self.cameraFrame = ttk.Frame(notebook)
        self.cameraLabel = ttk.Label(self.cameraFrame, text = 'Camera', font= ('Arial', 40))
        self.cameraLabel.pack(padx=10, pady=10, anchor = 'w')
        self.label_widget = ttk.Label(self.cameraFrame)
        self.label_widget.pack()
        
        self.webcam = cv2.VideoCapture(0)
        self.width, self.height = 800, 600
        self.webcam.set(cv2.CAP_PROP_FRAME_WIDTH, self.width)
        self.webcam.set(cv2.CAP_PROP_FRAME_HEIGHT, self.height)
        self.backend = backend
        self.openCamera()
        
    def openCamera(self):
        _, frame = self.webcam.read()
        frame = self.backend.backend(frame, device = 'cpu')[0]
        opencv_image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGBA)
        captured_image = Image.fromarray(opencv_image)
        photo_image = ImageTk.PhotoImage(image=captured_image)
        self.label_widget.photo_image = photo_image
        self.label_widget.configure(image=photo_image)
        self.label_widget.after(10, self.openCamera)
        # notification = Notify()
        # notification.title = "Priya is annoying"
        # notification.message = "Why is she even born at this point"
        # notification.send()