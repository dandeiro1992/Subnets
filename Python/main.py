import os
import random

'''
Testy do przeprowadzenia z pliku:
funkcja test() generuje plik large-command-file-test, który zawiera 10 000 dodawań i usunięć następnie wykonuje bench. procedurę powtarza kilka razy.
'''


def generateIPAddress():
    rand1 = random.randint(0, 255)
    rand2 = random.randint(0, 255)
    rand3 = random.randint(0, 255)
    rand4 = random.randint(0, 255)
    return '{}.{}.{}.{}'.format(rand1, rand2, rand3, rand4)


def generateIPAddresses(value):
    IPs = []
    for v in range(value):
        IPs.append(generateIPAddress())
    return IPs


def generatePrefix():
    rand1 = random.randint(0, 32)
    return '{}/{}'.format(generateIPAddress(), rand1)


def generateAddCommand():
    command = 'add {}'.format(generatePrefix())
    return command + "\n"


def generateDelCommand():
    command = 'del {}'.format(generatePrefix())
    return command + "\n"


def generateCheckCommand():
    command = 'check {}'.format(generateIPAddress())
    return command + "\n"


def generateBenchCommand():
    numberOfChecks = 10000000
    numberOfAddresses = 1000
    command = 'bench {} '.format(numberOfChecks)
    for i in range(numberOfAddresses - 1):
        command = command + '{} '.format(generateIPAddress())
    command = command + '{}'.format(generateIPAddress())
    return command + "\n"


def test(filePath, testName):
    with open(filePath + testName, "w") as file:
        file.write(generateAddCommand())
        file.write(generateBenchCommand())
        for i in range(10):
            for j in range(1000):
                file.write(generateAddCommand())
                file.write(generateDelCommand())
            file.write(generateBenchCommand())
    file.close()


def testAdd(filePath, testName, numberOfAdds):
    with open(filePath + testName, "w") as file:
        for i in range(numberOfAdds):
            file.write(generateAddCommand())
    file.close()


def test1(filePath, testName):
    with open(filePath + testName, "a") as file:
        for value in range(10000, 11000, 1000):
            for i in range(10000):
                file.write(generateAddCommand())
            file.write(generateBenchCommand())
    file.close()


if __name__ == '__main__':
    path = os.getcwd() + "/"
    fileName = "large-command-file-test"
    test(path, fileName)
