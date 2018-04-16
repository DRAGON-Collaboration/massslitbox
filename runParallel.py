from subprocess import call
from multiprocessing import Process



def runGeant(execname):
    call(['nice', execname, 'run.mac'])


if __name__ == '__main__':

    jobs = []


    ##
    ## Edit the list of executables here
    ## Each entry in the list is a string specifying the executable name
    ## Example is a bunch of execubales called "dragoni", with 0 <= i <= 9.
    ##
    executables = [ "./dragon0", "./dragon1", "./dragon2", "./dragon3", "./dragon4",
                    "./dragon5", "./dragon6", "./dragon7", "./dragon8", "./dragon9" ]
    
    for e in executables:
            print 'Running simulation: ' + e
            jobs.append(Process(target=runGeant, args=(e,)))

    for j in jobs:
        j.start()

    for j in jobs:
        j.join()

