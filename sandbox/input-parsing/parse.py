#! /usr/bin/env python3

import yaml


class Reader:
    def __init__(self, fileName):
        with open(fileName) as f:
            self.data = yaml.load(f, Loader=yaml.FullLoader)

    def __str__(self):
        meshFile = self.data['domain']['mesh']
        output = "Mesh file: " + meshFile + "\n"

        setting_mechanics = self.data['setting']['mechanics']

        for item in setting_mechanics:
            output += str(item['region']) + "\n"
            output += str(item['type']) + "\n"

        return output


if __name__ == '__main__':
    reader = Reader('simulation-data.yaml')
    print(reader)
