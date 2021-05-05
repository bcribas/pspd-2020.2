

class BCP:
    def __init__(self):
        self.enclosures = []
        self.commands = []
        self.dict_values = {}

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

    def run_bcp(self):
        count = 0
        list_erro = []
        false_enclosures = []
        for i, enclosure in enumerate(self.enclosures):
            subs_enclosure = [self.dict_values[each] for each in enclosure]
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
            print("SAT")
        else:
            false_enclosures.reverse()
            print("[{} clausulas falsas] {}\n[lits] {}".format(count, " ".join(
                list_erro), " ".join(false_enclosures)))

    def analize_commands(self):
        for command in self.commands:
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
            self.run_bcp()

    def run(self):
        self.read_input()
        # print(self.enclosures)
        # self.print_formula()
        self.read_commands()
        self.analize_commands()


if __name__ == "__main__":
    bcp = BCP()
    bcp.run()
