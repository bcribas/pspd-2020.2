import threading
import sys


class BCP:
    def __init__(self, len_thrs):
        self.enclosures = []
        self.commands = []
        self.dict_values = {}
        self.result = []
        self.len_thrs = len_thrs

    def read_input(self):
        input_len = input()
        len_propositions = int(input_len.split(" ")[1])

        for i in range(len_propositions):
            enclouse = input()
            enclouse = enclouse.split(" ")[:-1]
            self.enclosures.append([int(each) for each in enclouse])

    def read_commands(self):
        while 1:
            try:
                command = input()
                # print(command)
                self.commands.append(command.split(" "))
            except EOFError:
                break
        self.result = ["" for each in range(len(self.commands))]

    def print_formula(self):
        len_enclouses = len(self.enclosures)
        for j, enclouse in enumerate(self.enclosures):
            print("(", end=" ")
            len_enclouse = len(enclouse)
            for i, preposition in enumerate(enclouse):
                if preposition < 0:
                    print("¬{}".format(abs(preposition)), end=" ")
                else:
                    print("{}".format(preposition), end=" ")

                if i != len_enclouse - 1:
                    print("∨ ", end="")
                else:
                    pass
            print(")", end=" ")
            if j != len_enclouses - 1:
                print(" ∧ ", end="")
            else:
                print("")

    def run_bcp(self, dict_values, index, enclosures):
        count = 0
        list_erro = []
        false_enclosures = []
        for i, enclosure in enumerate(enclosures):
            subs_enclosure = [dict_values[each] for each in enclosure]
            # print(subs_enclosure)
            if any(subs_enclosure):
                pass
            else:
                count += 1
                list_erro.append(str(i))

        for key in self.dict_values.keys():
            if not self.dict_values[key]:
                false_enclosures.append(str(key))
        if count == 0:
            self.result.insert(index, "SAT\n")
        else:
            false_enclosures.reverse()
            self.result.insert(
                index,
                "[{} clausulas falsas] {}\n[lits] {}\n".format(count, " ".join(
                    list_erro), " ".join(false_enclosures)))

    def run_commands(self, command):
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

    def print_result(self):
        for each in self.result:
            print(each, end="")

    def analize_commands(self):
        index = 0
        while len(self.commands) != index + 1:
            thrs = []
            if index == len(self.commands):
                break
            if len(self.commands) - index >= self.len_thrs:
                for i in range(self.len_thrs):
                    self.run_commands(self.commands[index])
                    thrs.append(
                        threading.Thread(
                            target=self.run_bcp, args=(
                                self.dict_values, index, self.enclosures)))
                    thrs[i].start()
                    index += 1

                for thr in thrs:
                    thr.join()
            else:
                print(index, len(self.commands))
                for i in range(len(self.commands) - index - 1):
                    thrs.append(
                        threading.Thread(
                            target=self.run_bcp, args=(
                                self.dict_values, index, self.enclosures)))
                    thrs[i].start()
                    index += 1

                for thr in range(len(self.commands) - 1 - index):
                    thrs[i].join()

    def run(self):
        self.read_input()
        self.read_commands()
        self.analize_commands()
        # self.print_result()


if __name__ == "__main__":
    len_thrs = int(sys.argv[1])
    bcp = BCP(len_thrs)
    bcp.run()
