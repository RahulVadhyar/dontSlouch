from tkinter import *
from tkinter import ttk
import cv2
from PIL import Image, ImageTk 
from notifypy import Notify

class CameraTab:
    """
    Represents a camera tab in the application.

    Args:
        notebook (ttk.Notebook): The notebook widget to which the camera tab belongs.
        backend: The backend object used for processing camera frames.
        device: The device used for processing camera frames.
        progressTab: The progress tab object used for displaying scores.

    Attributes:
        cameraFrame (ttk.Frame): The frame widget for the camera tab.
        cameraLabel (ttk.Label): The label widget for the camera tab.
        label_widget (ttk.Label): The label widget for displaying the camera frame.
        slouching_label (ttk.Label): The label widget for displaying slouching status.
        showSkeletonButton (ttk.Button): The button widget for toggling skeleton display.
        showSkeleton (bool): Flag indicating whether to show the skeleton or not.
        notificationCounter (int): Counter for displaying notifications.
        progressTab: The progress tab object used for displaying scores.
        score (int): The current score.
        maxScore (int): The maximum score achieved.
        device: The device used for processing camera frames.
        webcam: The webcam object for capturing frames.
        width (int): The width of the captured frames.
        height (int): The height of the captured frames.
        backend: The backend object used for processing camera frames.
        result (str): The result of the backend processing.
        modelValue (int): The model value obtained from the backend processing.
        position (list): The position obtained from the backend processing.
    """

    def __init__(self, notebook, backend, device, progressTab):
        self.cameraFrame = ttk.Frame(notebook)
        self.cameraLabel = ttk.Label(self.cameraFrame, text='Camera', font=('Helvetica', 40))
        self.cameraLabel.pack(padx=10, pady=10, anchor='w')
        
        # Enlarge image window
        self.label_widget = ttk.Label(self.cameraFrame)
        self.label_widget.pack(pady=10)  # Add spacing between image window and slouching label
        
        # Center slouching label horizontally and vertically
        self.slouching_label = ttk.Label(self.cameraFrame, anchor='center', font=('Helvetica', 24, 'bold'))
        self.slouching_label.pack(pady=(10, 5))  # Add spacing between slouching label and max score label
        
        # Center max score label horizontally and vertically
        self.max_score_label = ttk.Label(self.cameraFrame, anchor='center', font=('Helvetica', 24, 'bold'))
        self.max_score_label.pack(pady=(5, 10))  # Add spacing below max score label
        style = ttk.Style()
        style.configure('TButton', font=('Helvetica', 24, 'bold'))
        self.showSkeletonButton = ttk.Button(self.cameraFrame, text='Show Skeleton', command=self.showSkeleton,padding=10,style="TButton")

        self.showSkeletonButton.pack()
        
        self.showSkeleton = True
        self.notificationCounter = 10
        self.progressTab = progressTab
        self.score = 0
        self.maxScore = 0

        self.device = device
        self.webcam = cv2.VideoCapture(0)
        self.width, self.height = 800, 600
        self.webcam.set(cv2.CAP_PROP_FRAME_WIDTH, self.width)
        self.webcam.set(cv2.CAP_PROP_FRAME_HEIGHT, self.height)
        self.backend = backend
        self.openCamera()
        self.result = ""
        self.modelValue = 0
        self.position = []

    def showSkeleton(self):
        """
        Toggles the display of the skeleton on the camera frame.
        """
        self.showSkeleton = not self.showSkeleton

    def openCamera(self):
        """
        Opens the camera and continuously captures frames for processing.
        """
        _, frame = self.webcam.read()
        frame, result, x, res = self.backend.backend(frame, device=self.device.get(), skeleton=self.showSkeleton)
        self.result = result
        self.modelValue = x
        self.position = res

        opencv_image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGBA)
        captured_image = Image.fromarray(opencv_image)
        # Enlarge image
        photo_image = ImageTk.PhotoImage(image=captured_image)

        self.label_widget.photo_image = photo_image
        self.label_widget.configure(image=photo_image)
        self.label_widget.after(10, self.openCamera)

        if self.result == "slouching":
            self.slouching_label.config(text='You are slouching at this position')
            self.score = 0
        else:
            self.slouching_label.config(text='Congrats! You are not slouching')
            self.score += 1
            self.maxScore = max(self.score, self.maxScore)

        self.progressTab.score_label.config(text=f'Score: {self.score}')
        self.progressTab.max_score_label.config(text=f'Max Score: {self.maxScore}')

        if self.result == "slouching" and self.notificationCounter == 0:
            self.notificationCounter = 40
            notification = Notify()
            notification.title = "Don't Slouch!"
            notification.message = f"You are slouching! Please correct your posture. Your max score is {self.maxScore}!"
            notification.send()
        elif self.result == "slouching":
            self.notificationCounter -= 1
