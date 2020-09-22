#! /usr/bin/env python3

import yaml

with open('simulation-data.yaml') as f:
    data = yaml.load(f, Loader=yaml.FullLoader)
    #print(data)
    
    filename = data['domain']['mesh']['filename']
    initialConditions = data['solver']['initial conditions']
    print("The filename is", filename)
    print("Initial conditions are", initialConditions)