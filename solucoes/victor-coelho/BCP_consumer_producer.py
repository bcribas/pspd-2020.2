import sys
import threading


RESULT = []
QUEUE = []
LOCK = threading.Lock()


class ProducerThread(threading.Thread):
    def __init__(self, group=None, target=None, name=None,
                 args=(), kwargs=None, verbose=None):
        super(ProducerThread, self).__init__()
        self.target = target
        self.name = name
        self.enclosures = []
        self.commands = []
        self.dict_values = {}
        self.result = []

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

    def run(self):
        global QUEUE
        """
        dict_values, index, enclousures
        """
        index = 0
        item = {}
        self.read_input()
        self.read_commands()
        # logging.debug(self.commands)
        while True:
            if index == len(self.commands):
                # logging.debug(QUEUE.queue)
                break
            self.analize_command(self.commands[index])
            item[0] = self.dict_values.copy()
            item[1] = index
            item[2] = self.enclosures.copy()
            # logging.debug(
            #     'Putting {} queuesize: {} command {}'.format(
            #         item,
            #         QUEUE.qsize(),
            #         self.commands[index]
            #         ))
            QUEUE.append(item.copy())
            index += 1
        return


class ConsumerThread(threading.Thread):
    def __init__(self, group=None, target=None, name=None,
                 args=(), kwargs=None, verbose=None):
        super(ConsumerThread, self).__init__()
        self.target = target
        self.name = name
        return

    def run(self):
        global RESULT
        global LOCK
        while True:
            if len(QUEUE) != 0:
                item = ()
                with LOCK:
                    try:
                        item = QUEUE.pop()
                    except Exception:
                        break
                # logging.debug(QUEUE.queue)
                # logging.debug('Getting {} {} {} queuesize: {}'.format(
                #      item[0],
                #      item[1],
                #      item[2],
                #      len(QUEUE)
                #  ))
                count = 0
                list_erro = []
                false_enclosures = []
                for i, enclosure in enumerate(item[2]):
                    subs_enclosure = [item[0][each] for each in enclosure]
                    if any(subs_enclosure):
                        # logging.debug(True)
                        pass
                    else:
                        # logging.debug(False)
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
                # logging.debug(RESULT)
            else:
                break
        return


if __name__ == "__main__":
    num_consumers = int(sys.argv[1])
    p = ProducerThread(name='producer')
    p.start()
    cs = []
    for i in range(num_consumers):
        cs.append(ConsumerThread(name="consumer_{}".format(i)))
        cs[i].setDaemon(True)
        cs[i].start()

    for each in cs:
        each.join()
    p.join()

    for text in RESULT:
        print(text, end="")
