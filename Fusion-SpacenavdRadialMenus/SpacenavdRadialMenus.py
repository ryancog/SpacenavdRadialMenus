

#Author-
#Description-

import adsk.core, adsk.fusion, adsk.cam, traceback
import threading, random, json, socket, array, math, time

HOST = "127.0.0.1"
PORT = 22222

app = None
ui = None
handlers = []
stopFlag = None
myCustomEvent = 'RadialEventId'
customEvent = None

mysock = None

check = False

def sign(x):
    return 1 if x > 0 else -1 if x < 0 else 0

# The event handler that responds to the custom event being fired.
class RadialEventHandler(adsk.core.CustomEventHandler):
    def __init__(self):
        super().__init__()
    def notify(self, args):
        try:
            global app

            if (args.additionalInfo != ""):
                cmd = ui.commandDefinitions.itemById(args.additionalInfo)
                cmd.execute()
        except:
            if ui:
                ui.messageBox('Failed:\n{}'.format(traceback.format_exc()))


# The class for the new thread.
class RadialThread(threading.Thread):
    def __init__(self, event):
        threading.Thread.__init__(self)
        self.stopped = event

    def run(self):
        global mysock

        mysock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        mysock.connect((HOST, PORT))

        while True:
            data = mysock.recv(255)
            app.fireCustomEvent(myCustomEvent, data.decode())

def run(context):
    global ui
    global app

    try:
        app = adsk.core.Application.get()
        ui  = app.userInterface

        # Register the custom event and connect the handler.
        global customEvent
        customEvent = app.registerCustomEvent(myCustomEvent)

        onThreadEvent = RadialEventHandler()
        customEvent.add(onThreadEvent)
        handlers.append(onThreadEvent)

        # Create a new thread for the other processing.
        global stopFlag
        stopFlag = threading.Event()
        myThread = RadialThread(stopFlag)
        myThread.start()
    except:
        if ui:
            ui.messageBox('Failed:\n{}'.format(traceback.format_exc()))


def stop(context):
    global mysock
    try:
        mysock.close()

        if handlers.count:
            customEvent.remove(handlers[0])

        stopFlag.set()
        app.unregisterCustomEvent(myCustomEvent)
    except:
        if ui:
            ui.messageBox('Failed:\n{}'.format(traceback.format_exc()))
