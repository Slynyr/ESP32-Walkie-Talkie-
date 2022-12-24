from configparser import ConfigParser
import os 
from os.path import exists
import shutil
import traceback

def loadPaths(pathtype):
    rawPaths = configData[pathtype]
    return rawPaths.split(',')

def inputPath(pathtype):
    paths = input('Enter the targetpaths for %s\n%s' % (pathtype, cmdPrefix))
    configData[pathtype] = paths
    updateConfig()

def discoverFiles(path):
    files = []
    for i in os.listdir(path):
        i = i.replace('\\', '/')
        i = i.replace(' ', '')
        files.append(i)
    return files

def transferData(pull):
    if pull:
        targetPaths = loadPaths("gitPaths")
        destPaths = loadPaths("kicadPaths")
    else: 
        targetPaths = loadPaths("kicadPaths")
        destPaths = loadPaths("gitPaths")

    if len(targetPaths) == len(destPaths):  
        for i in range(len(targetPaths)):
            tp = targetPaths[i]
            dp = destPaths[i] #I hate that I had to do this
            
            targetFiles = discoverFiles(tp)
            for x in targetFiles:
                print("Copied %s" % x)
                shutil.copy(tp + '\\' + x, dp)

    else:
        print("[Error] Invalid path configuration. Uneven number of paths. Use config command to re-configure your paths.")
        return

def selector():
    userSelection = input(cmdPrefix)

    if userSelection == "config":
        inputPath("kicadPaths")
        inputPath("gitPaths")
    elif userSelection == "pull":
        transferData(True)
    elif userSelection == "push":
        transferData(False)

#config
def loadConfig():
    global configData
    if exists(cwd + '\\config.ini') == False:
        configSetup()
    config.read(cwd + '\\config.ini')
    configData = config['kicadConfig']

    if configData['kicadPaths'] == '0' or configData['gitPaths'] == '0':
        print("SETUP\nInput target folders within kicad using ',' seperator.")
        inputPath("kicadPaths")
        inputPath("gitPaths")

def updateConfig():
    with open (cwd + '/config.ini', 'w') as f:
        config.write(f)
#Config Generation
def configSetup():
    config = ConfigParser()
    config['kicadConfig'] = {
        'kicadPaths': '0',
        'gitPaths': '0'
    }

    with open (cwd + '\\config.ini', 'w') as f:
        config.write(f)

config = ConfigParser()
cwd = os.path.dirname(os.path.realpath(__file__))

cmdPrefix = 'KPM~$ '

try:
    loadConfig()
    while True:
       selector()
except Exception as e:
    print(str(e))
