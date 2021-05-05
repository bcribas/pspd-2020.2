import sys
import threading
from itertools import islice


RESULT = []
QUEUE = []
LOCK = threading.Lock()


class ProducerThread(threading.Thread):
    def __init__(self, group=None, target=None, name=None,
                 args=(), kwargs=None, verbose=None, num_splits=None):
        super(ProducerThread, self).__init__()
        self.target = target
        self.name = name
        self.enclosures = []
        self.commands = []
        self.dict_values = {}
        self.result = []
        self.num_splits = num_splits
        self.chuncks = []

    def read_input(self):
        input_len = input()
        len_propositions = int(input_len.split(" ")[1])

        for i in range(len_propositions):
            enclouse = input()
            enclouse = enclouse.split(" ")[:-1]
            self.enclosures.append([int(each) for each in enclouse])

    def read_commands(self):
        global RESULT
        while True:
            try:
                command = input()
                self.commands.append(command.split(" "))
            except EOFError:
                break
        RESULT = ["0" for each in range(len(self.commands))]

    def analize_command(self, command):
        if command[0] == "full":
            for each in command[1:]:
                self.dict_values[int(each)] = True
                self.dict_values[-1 * int(each)] = False
        if command[0] == "flip":
            self.dict_values[
                -1 * int(command[1])
            ] = not self.dict_values[-1 * int(command[1])]
            self.dict_values[
                int(command[1])
            ] = not self.dict_values[int(command[1])]

    def make_chunck(self, it, size):
        it = iter(it)
        return list(iter(lambda: list(islice(it, size)), []))

    def make_split_param(self):
        global QUEUE
        part_num = len(QUEUE)//self.num_splits
        self.chuncks = self.make_chunck(QUEUE, part_num)

    def run(self):
        global QUEUE
        """
        dict_values, index, enclousures
        """
        index = 0
        item = {}
        self.read_input()
        self.read_commands()
        while True:
            if index == len(self.commands):
                break
            self.analize_command(self.commands[index])
            item[0] = self.dict_values.copy()
            item[1] = index
            item[2] = self.enclosures.copy()
            QUEUE.append(item.copy())
            index += 1
        self.make_split_param()
        return


class ConsumerThread(threading.Thread):
    def __init__(self, group=None, target=None, name=None,
                 args=(), kwargs=None, verbose=None, chunck=[]):
        super(ConsumerThread, self).__init__()
        self.target = target
        self.name = name
        self.chunck = chunck
        return

    def run(self):
        global RESULT
        global LOCK
        for item in self.chunck:
            count = 0
            list_erro = []
            false_enclosures = []
            for i, enclosure in enumerate(item[2]):
                subs_enclosure = [item[0][each] for each in enclosure]
                if any(subs_enclosure):
                    pass
                else:
                    count += 1
                    list_erro.append(str(i))

            for key in item[0].keys():
                if not item[0][key]:
                    false_enclosures.append(str(key))

            if count == 0:
                RESULT[item[1]] = "SAT\n"

            else:
                false_enclosures.reverse()
                RESULT[item[1]] = "[{} clausulas falsas]" \
                    " {}\n[lits] {}\n".format(
                    count, " ".join(list_erro), " ".join(false_enclosures))
        return


if __name__ == "__main__":
    num_consumers = int(sys.argv[1])
    p = ProducerThread(name='producer', num_splits=num_consumers)
    p.start()
    p.join()
    cs = []
    for i, chunck in enumerate(p.chuncks):
        cs.append(ConsumerThread(name="consumer_{}".format(i), chunck=chunck))
        cs[i].setDaemon(True)
        cs[i].start()

    for each in cs:
        each.join()

    # for text in RESULT:
    #     print(text, end="")
